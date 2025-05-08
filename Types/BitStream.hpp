#ifndef BITSTREAM_H
#define BITSTREAM_H

#include "Utils.hpp"

struct BitStream {
	std::vector<u8> buffer;
	u64 offset = 0;
  
	BitStream(u64 bytes=1024): buffer(bytes) {}

	void write_bits(uint32_t value, u64 size) {
		while (size--) {
			size_t byte_idx = offset >> 3;
			u64 bit_idx = 7 - (offset & 7);
			buffer[byte_idx] &= ~(1 << bit_idx);
			buffer[byte_idx] |= ((value >> size) & 1) << bit_idx;
			++offset;
		}
	}

	u32 read_bits(u64 size) {
		u32 v = 0;
		while (size--) {
			u64 byte_idx = offset >> 3;
			u64 bit_idx  = 7 - (offset & 7);
			v = (v << 1) | ((buffer[byte_idx] >> bit_idx) & 1);
			++offset;
		}
		return v;
	}

	u64 bits_size() { return offset; }
	u64 bytes_size() { return (offset + 7) >> 3; }

	std::string to_bit_string() const {
		std::string s;
		s.reserve(offset);
		for (size_t i = 0; i < offset; ++i) {
			u64 byte_idx = i >> 3;
			u64 bit_idx = 7 - (i & 7);
			u8 bit = (buffer[byte_idx] >> bit_idx) & 1;
			s.push_back(bit ? '1' : '0');
		}
		return s;
	}

	void print(std::ostream &os = std::cout) const {
		os << to_bit_string() << std::endl;
	}
};

#endif // BITSTREAM_H