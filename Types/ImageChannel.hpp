#ifndef IMAGECHANNEL_H
#define IMAGECHANNEL_H

#include "Utils.hpp"
#include "Logger.hpp"

template <typename T>
class ImageChannel {
public:
	ImageChannel() = default;
	
	ImageChannel(u64 width, u64 height) : \
		m_width((assert(width > 0), width)), m_height((assert(height > 0), height)) {
		DEBUG("init reserve %llu x %llu\n", width, height);
		m_data.reserve(width * height);
	}

	ImageChannel(u64 width, u64 height, std::vector<T> data) : \
		m_width((assert(width > 0), width)), m_height((assert(height > 0), height)), m_data((assert(data.size() > 0), data)) {
			DEBUG("init %llu x %llu, %llu\n", width, height, data.size());
			assert(width*height == data.size());
		}

	void resize(u64 width, u64 height) {
		DEBUG("resize %llu x %llu\n", width, height);
		assert(width > 0);
		assert(height > 0);
		m_width = width;
		m_height = height;
		m_data.resize(width * height);
	}

	u64 size() const   { return m_data.size();  }
	bool empty() const { return m_data.empty(); }
	
	T& operator()(u64 i, u64 j)             { return m_data[m_width * i + j]; }
	const T& operator()(u64 i, u64 j) const { return m_data[m_width * i + j]; }

	friend bool operator==(const ImageChannel<T>& channel1, const ImageChannel<T>& channel2) {
		return (channel1.m_width  == channel2.m_width) && \
			   (channel1.m_height == channel2.m_height) && \
			   (channel1.m_data   == channel2.m_data);
	}

	inline u64 width() const           { return m_width;  }
	inline u64 height() const          { return m_height; }
	inline std::vector<T> data() const { return m_data;   }

private:
    u64 m_width, m_height;
	std::vector<T> m_data;
};

#endif // IMAGECHANNEL_H