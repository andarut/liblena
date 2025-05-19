#ifndef TYPES_H
#define TYPES_H

#include "ImageChannel.hpp"
#include "Image.hpp"
#include "BitStream.hpp"

template <typename T>
void print(const ImageChannel<T>& ch) {
	INFO("width x height = %llu x %llu\n", ch.width(), ch.height());
	INFO("data size = %llu\n", ch.size());
	DEBUG("data = \n");
	if (ch.size() != ch.width() * ch.height()) {
		for (u64 i = 0; i < ch.size(); i++)
			DEBUG("%hhu ", ch(0, i));
		return;
	}
	for (u64 i = 0; i < ch.height(); i++) {
		for (u64 j = 0; j < ch.width(); j++) {
			if (std::is_same<T, f64>::value)
				DEBUG("%6.02lf ", (f64)ch(i, j));
			else if (std::is_same<T, s16>::value)
				DEBUG("%4d ", (s16)ch(i, j));
			
			else DEBUG("%hhu ", ch(i, j));
		}
		DEBUG("\n");
	}
}

#endif // TYPES_H