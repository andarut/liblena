#ifndef IMAGE_H
#define IMAGE_H

#include "Utils.hpp"
#include "Logger.hpp"

#include "ImageCh.h"

template <typename T>
class Image {
public:

	Image() = default;
	Image(u64 _width, u64 _height, const std::vector<ImageCh<T>>& _data) : m_data(_data){};
	Image(u64 _width, u64 _height, u8 _numberOfChannels) {
		m_data.resize(_numberOfChannels);
		for (u64 k = 0; k < _numberOfChannels; k++) {
			m_data[k] = ImageCh<T>(_width, _height);
		}
	}

	ImageCh<T>& operator[](u64 i)             { return m_data[i]; }
	const ImageCh<T>& operator[](u64 i) const { return m_data[i]; }

	const std::vector<ImageCh<T>>& data() const { return m_data; }

    u8 nChls() const { return m_data.size(); }

	/* sum size of all channel's data */
	u64 size() const {
		u64 size = 0;
		for (auto& chl : m_data)
			size += chl.size();
		return size;
	}

	friend bool operator==(const Image& im1, const Image& im2)
	{
		return im1.nChls() == im2.nChls() && \
			im1.size()  == im2.size() && \
			im1.width() == im2.width() && \
			im1.data()  == im2.data();
	}

	/* from first channel (assumes all channel equal in size) */
	u64 width() const {
    	if (m_data.empty()) {
			return 0;
		}
		return m_data[0].width();
	}

	u64 height() const {
		if (m_data.empty()) {
			return 0;
		}
		return m_data[0].height();
	}

	bool empty() const { return m_data.empty(); }

private:
    std::vector<ImageCh<T>> m_data;
};


template <typename T>
inline void print(const Image<T>& im) {
	PRINT("=== IMAGE PRINT ===\n");
	PRINT("chls = %d\n", im.nChls());
	PRINT("size = %lld\n", im.size());
	PRINT("width = %lld\n", im.width());
	PRINT("height = %lld\n", im.height());
	PRINT("data = \n");
	for (u8 i = 0; i < im.nChls(); i++) {
		print("\t", im[i]);
	}
}

#endif // IMAGE_H