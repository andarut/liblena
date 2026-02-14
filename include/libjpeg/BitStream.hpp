#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "Utils.hpp"
#include "Logger.hpp"
#include <cstdint>

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
	BitStream(u64 bytes=0): m_buf(bytes), m_offset(0) {}

  void write_bits(uint64_t value, uint64_t bit_count) {
    uint64_t final_offset = m_offset + bit_count;
    uint64_t needed_bytes = (final_offset + 7) >> 3;
    
    if (needed_bytes > m_buf.size()) {
      DEBUG("resizing to: %d\n", needed_bytes);
      m_buf.resize(needed_bytes);
    }
    
    while (bit_count--) {
        size_t byte_idx = m_offset >> 3;
        DEBUG("byte_idx: %d\n", byte_idx);
        uint64_t bit_idx = 7 - (m_offset & 7);
        m_buf[byte_idx] &= ~(1 << bit_idx);
        m_buf[byte_idx] |= ((value >> bit_count) & 1) << bit_idx;
        ++m_offset;
    }
    DEBUG("finish write_bits\n");
  }

    friend bool operator==(const BitStream& bs1, const BitStream& bs2) {
      // TODO: отдельно хранить размер и позцию
      // if (bs1.bits_size() != bs2.bits_size()) {
      //     ERROR("Different sizes: %d != %d\n", bs1.bits_size(), bs2.bits_size());
      //     return false;
      // }
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

    template<typename T>
    requires std::is_integral_v<T>
    void write(T value) {
      write_bits(static_cast<uint64_t>(value), sizeof(T) * 8);
    }

    void write(float value) {
      write_bits(std::bit_cast<uint32_t>(value), 32);
    }

    void write(double value) {
      write_bits(std::bit_cast<uint64_t>(value), 64);
    }

    template<typename T>
    void write(const std::vector<T>& arr) {
      for(u64 i = 0; i < arr.size(); i++) {
        write(arr[i]);
      }
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

	int readFromFile(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        if (!ifs) {
            ERROR("Failed to open file for reading: %s\n", filename.c_str());
            return -1;
        }
        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        m_buf.resize(static_cast<size_t>(size));
        if(!ifs.read(reinterpret_cast<char*>(m_buf.data()), size)) {
            ERROR("Failed to read entire file: %s\n", filename.c_str());
            return -2;
        }
        m_offset = 0;

        return 0;
    }

	void rewind() { m_offset = 0; }
    void set_offset(u64 offset) { m_offset = offset; }

    std::vector<u8> getBuf() const { return m_buf; }

    int read_bits(uint64_t& value, uint64_t bitSize) {
      value = 0;
      DEBUG("bitSize = %zu\n", bitSize);
      while (bitSize--) {
        if (m_offset >= m_buf.size() * 8) {
          ERROR("Out of bounds: %lu / %zu\n", m_offset, m_buf.size() * 8);
          return 1;
        }
        size_t b = m_offset >> 3;
        uint64_t i = 7 - (m_offset & 7);
        value = (value << 1) | ((m_buf[b] >> i) & 1); // Shift and OR the bit
        ++m_offset;
      }
      DEBUG("offset = %lu / %zu\n", m_offset, m_buf.size() * 8);
      return 0;
    }

    template<typename T>
    requires std::is_integral_v<T>
    int read(T& value) {
      uint64_t val;
      auto res = read_bits(val, sizeof(T) * 8);
      RETURN_IF_ERROR(res, "Error in read_bits\n");
      value = static_cast<T>(val);
      return 0;
    }

    int read(float& value) {
      uint64_t val;
      auto res = read_bits(val, 32);
      RETURN_IF_ERROR(res, "Error in read_bits\n");
      value = std::bit_cast<float>(static_cast<uint32_t>(val));
      return 0;
    }

    int read(double& value) {
      uint64_t val;
      auto res = read_bits(val, 64);
      RETURN_IF_ERROR(res, "Error in read_bits\n");
      value = std::bit_cast<double>(val);
      return 0;
    }

    template<typename T>
    int read(std::vector<T>& arr) {
      for(u64 i = 0; i < arr.size(); i++) {
        auto res = read(arr[i]);
        RETURN_IF_ERROR(res, "Error in read_bits\n");
      }
      return 0;
    }
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
