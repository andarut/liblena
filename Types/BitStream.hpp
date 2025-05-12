#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "Utils.hpp"

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
		DEBUG("value = %llu, size = %llu, %llu / %llu\n", value, size, m_offset, m_buf.size());
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
                ERROR("out of bounce %lu / %zu\n", m_offset, m_buf.size()*8);
            }
			size_t b = m_offset >> 3;
			uint64_t i = 7 - (m_offset & 7);
			v = (v << 1) | ((m_buf[b] >> i) & 1);
			++m_offset;
            
		}
        DEBUG("offset = %lu / %zu, value = %d\n", m_offset, m_buf.size()*8, v);
		return v;
	}

	void write_byte(u8 byte) {
        write_bits(byte, 8);
    }

    void write_bytes(const u8* data, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            write_byte(data[i]);
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
    bs.rewind();
}


#endif // BITSTREAM_H