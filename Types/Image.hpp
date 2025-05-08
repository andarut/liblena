/*
#ifndef IMAGE_H
#define IMAGE_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageChannel.hpp"

template <typename T>
class Image {
public:
	Image() = default;

	Image(u64 _width, u64 _height, std::vector<ImageChannel<T>> _data) : \
		width(_width), height(_height), numberOfChannels(_data.size()), data(_data) {}

	Image(u64 _width, u64 _height, u8 _numberOfChannels) : \
		width(_width), height(_height), numberOfChannels(_numberOfChannels) {
		data.resize(_numberOfChannels);
		for (u64 k = 0; k < _numberOfChannels; k++) {
			data[k] = RawChannelData<T>(_width, _height);
			data[k].resize(_width, _height);
		}
	}

	Image(const std::initializer_list<T> _data) : \
		width(1), height(1), numberOfChannels(_data.size()) {
		if (_data.size() == 0) {
			width = 0;
			height = 0;
		}
		data.reserve(_data.size());
		for (auto& d : _data)
			data.push_back(RawChannelData<T>(1, 1, {d}));
	}

	void print() const {
		printf("print width = %lld\n", width);
		printf("print height = %lld\n", height);
		printf("print data size = %zu\n", data.size());
		for (u64 i = 0; i < height; i++) {
			for (u64 j = 0; j < width; j++) {
				printf("(%03hhu %03hhu %03hhu) ", data[0](i, j), data[1](i, j), data[2](i, j));
			}
			printf("\n");
		}
	}

	ImageChannel<T>& operator[](u64 i)             { return m_data[i]; }
	const ImageChannel<T>& operator[](u64 i) const { return m_data[i]; }
	
	// friend bool operator==(const RawImageData<T>& data1, const RawImageData<T>& data2) {
	// 	return (data1.width == data2.width) && \
	// 		   (data1.height == data2.height) && \
	// 		   (data1.numberOfChannels == data2.numberOfChannels) && \
	// 		   (data1.data == data2.data);
	// }

    u8 chls_n() { return m_chls.size(); }

	u64 size() const {
		u64 size = 0;
		for (auto& chl : m_chls)
			size += (*this)[k]->size();
		return size;
	}

	bool empty() const { return m_chls.empty(); }

private:
    std::vector<ImageChannel> m_chls;
};

#endif // IMAGE_H


*/