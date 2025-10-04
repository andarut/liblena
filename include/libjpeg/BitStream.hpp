#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "Utils.hpp"
#include "Logger.hpp"

inline std::string bit_string(u32 value, u8 size) {
	std::string s;
	s.reserve(size);
	for (int8_t i = size - 1; i >= 0; --i) {
        s.push_back(((value >> i) & 1) ? '1' : '0');
    }
	return s;
}

class BitStream {
public:
	BitStream(u64 bytes=10000024): m_buf(bytes), m_offset(0) {}

	void write_bits(uint32_t value, uint64_t size) {
		// DEBUG("value = %llu, size = %llu, %llu / %llu\n", value, size, m_offset, m_buf.size());
        uint64_t final_offset = m_offset + size;
        uint64_t needed_bytes = (final_offset + 7) >> 3;
        if (needed_bytes > m_buf.size()) {
            m_buf.resize(needed_bytes);
        }
        while (size--) {
            size_t byte_idx = m_offset >> 3;
            uint64_t bit_idx = 7 - (m_offset & 7);
            m_buf[byte_idx] &= ~(1 << bit_idx);
            m_buf[byte_idx] |= ((value >> size) & 1) << bit_idx;
            ++m_offset;
        }
    }

    friend bool operator==(const BitStream& bs1, const BitStream& bs2) {
        if (bs1.bits_size() != bs2.bits_size()) {
            ERROR("Different sizes\n");
            return false;
        }
        auto size = bs1.bytes_size();
        auto bs1_buf = bs1.getBuf();
        auto bs2_buf = bs2.getBuf();
        for (int i = 0; i < bs1.bytes_size(); i++) {
            if (bs1_buf[i] != bs2_buf[i]) {
                ERROR("Different byte on offset = %d, %02X != %02X\n", i, bs1_buf[i], bs2_buf[i]);
                return false;
            }
        }
        return true;
    }

    u32 peek_bits(u64 size) const {
        uint32_t v = 0;
        u64      temp = m_offset;
        while (size--) {
            size_t b = temp >> 3;
            u64    i = 7 - (temp & 7);
            v = (v << 1) | ((m_buf[b] >> i) & 1);
            ++temp;
        }
        return v;
    }

	u32 read_bits(u64 size) {
		uint32_t v = 0;
		while (size--) {
            if (m_offset == m_buf.size()*8) {
                // ERROR("out of bounce %lu / %zu\n", m_offset, m_buf.size()*8);
            }
			size_t b = m_offset >> 3;
			uint64_t i = 7 - (m_offset & 7);
			v = (v << 1) | ((m_buf[b] >> i) & 1);
			++m_offset;
            
		}
        // DEBUG("offset = %lu / %zu, value = %d\n", m_offset, m_buf.size()*8, v);
		return v;
	}

	void write_u8(u8 byte) {
        write_bits(byte, 8);
    }

    void write_bytes(const u8* data, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            write_u8(data[i]);
        }
    }

	u8 read_byte() {
        return static_cast<u8>(read_bits(8));
    }

	size_t read_bytes(u8* out, size_t count) {
        size_t i = 0;
        for (; i < count && (bits_size() + 8) <= m_buf.size() * 8; ++i) {
            out[i] = read_byte();
        }
        return i;
    }

    void write_u16(uint16_t value) {
        write_u64(static_cast<uint64_t>(value), 16);
    }


    void write_u64(uint64_t value, uint64_t size = 64) {
        // DEBUG("value = %llu, size = %llu, %llu / %llu\n", value, size, m_offset, m_buf.size());
        uint64_t final_offset = m_offset + size;
        uint64_t needed_bytes = (final_offset + 7) >> 3;
        if (needed_bytes > m_buf.size()) {
            m_buf.resize(needed_bytes);
        }
        while (size--) {
            size_t byte_idx = m_offset >> 3;
            uint64_t bit_idx = 7 - (m_offset & 7);
            m_buf[byte_idx] &= ~(1 << bit_idx); // Clear the bit
            m_buf[byte_idx] |= ((value >> size) & 1) << bit_idx; // Set the bit
            ++m_offset;
        }
    }
    uint64_t read_u64(uint64_t size=64) {
        uint64_t v = 0;
        while (size--) {
            if (m_offset >= m_buf.size() * 8) {
                // ERROR("Out of bounds: %lu / %zu\n", m_offset, m_buf.size() * 8);
            }
            size_t b = m_offset >> 3;
            uint64_t i = 7 - (m_offset & 7);
            v = (v << 1) | ((m_buf[b] >> i) & 1); // Shift and OR the bit
            ++m_offset;
        }
        // DEBUG("offset = %lu / %zu, value = %llu\n", m_offset, m_buf.size() * 8, v);
        return v;
    }

    u32 nextBit() {
        return read_bits(1);
    }

	u64 bits_size() const { return m_offset; }
	u64 bytes_size() const { return (m_offset + 7) >> 3; }

	void fwrite(const std::string& filename) const {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) throw std::runtime_error("Failed to open file for writing: " + filename);
        ofs.write(reinterpret_cast<const char*>(m_buf.data()),
                  static_cast<std::streamsize>(bytes_size()));
    }

	void fread(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        if (!ifs) throw std::runtime_error("Failed to open file for reading: " + filename);
        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        m_buf.resize(static_cast<size_t>(size));
        if (!ifs.read(reinterpret_cast<char*>(m_buf.data()), size)) {
            throw std::runtime_error("Failed to read entire file: " + filename);
        }
        m_offset = size * 8;
    }

	void rewind() { m_offset = 0; }
    void set_offset(u64 offset) { m_offset = offset; }

    std::vector<u8> getBuf() const { return m_buf; }

private:
	std::vector<u8> m_buf;
	u64 m_offset;
};

inline void print(BitStream& bs) {
    u64 totalBits = bs.bits_size();
    bs.rewind();
    for (uint64_t i = 0; i < totalBits; ++i) {
        printf("%d", bs.read_bits(1));
        if ((i + 1) % 8 == 0 && (i + 1) != totalBits)
            printf(" ");
    }
    printf("\n");
    bs.set_offset(totalBits);
}


#endif // BITSTREAM_H