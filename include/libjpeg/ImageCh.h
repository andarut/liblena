#ifndef IMAGECHANNEL_H
#define IMAGECHANNEL_H

#include "Utils.hpp"
#include "Logger.hpp"

template <typename T>
class ImageCh {
public:
	ImageCh() = default;
	
	ImageCh(u64 width, u64 height) :
		m_width(width), m_height(height) {
		DEBUG("allocated %llu x %llu\n", width, height);
		m_data.resize(width * height);
	}

	ImageCh(u64 width, u64 height, std::vector<T>&& data) : \
		m_width(width),
		m_height(height),
		m_data(std::move(data)) {
	}

	template<typename OtherT>
    ImageCh(ImageCh<OtherT>&& other) {
		m_width = other.width();
		m_height = other.height();
        m_data.reserve(other.size());
        for (auto& v : other.data()) {
            m_data.push_back(static_cast<T>(v));
        }
    }

	inline u64 size() const   { return m_data.size();  }
	inline bool empty() const { return m_data.empty(); }
	
	T& operator()(u64 i, u64 j)             { return m_data[m_width * i + j]; }
	const T& operator()(u64 i, u64 j) const { return m_data[m_width * i + j]; }

	friend bool operator==(const ImageCh<T>& ch1, const ImageCh<T>& ch2) {
		return (ch1.m_width  == ch2.m_width) && \
			   (ch1.m_height == ch2.m_height) && \
			   (ch1.m_data   == ch2.m_data);
	}

	inline u64 width() const           { return m_width;  }
	inline u64 height() const          { return m_height; }
	inline const std::vector<T>& data() const { return m_data;   }

private:
    u64 m_width, m_height;
	std::vector<T> m_data;
};

template <typename T>
inline void print(const std::string& prefix, const ImageCh<T>& ch) {
	PREFIX_PRINT(prefix, "=== IMAGE_CH PRINT ===\n");
	PREFIX_PRINT(prefix, "size = %lld\n", ch.size());
	PREFIX_PRINT(prefix, "width = %lld\n", ch.width());
	PREFIX_PRINT(prefix, "height = %lld\n", ch.height());
	PREFIX_PRINT(prefix, "data = \n");
	for (u64 i = 0; i < ch.height(); i++) {
		PREFIX_PRINT(prefix, "\t");
		for (u64 j = 0; j < ch.width(); j++) {
			PRINT("%3d ", ch(i, j));
		}
		PRINT("\n");
	}
}

template <typename T>
inline void printDiff(const std::string& prefix, const ImageCh<T>& ch, const ImageCh<T>& ref) {
	PREFIX_PRINT(prefix, "=== IMAGE_CH DIFF PRINT ===\n");
	PREFIX_PRINT(prefix, "size = %lld, ref size = %lld\n", ch.size(), ref.size());
	PREFIX_PRINT(prefix, "width = %lld, ref width = %lld\n", ch.width(), ref.width());
	PREFIX_PRINT(prefix, "height = %lld, ref height = %lld\n", ch.height(), ref.height());

	if (ch.size() != ref.size()) return;
	if (ch.width() != ref.width()) return;
	if (ch.height() != ref.height()) return;

	PREFIX_PRINT(prefix, "data = \n");
	for (u64 i = 0; i < ch.height(); i++) {
		PREFIX_PRINT(prefix, "\t");
		for (u64 j = 0; j < ch.width(); j++) {
			if (ch(i, j) != ref(i, j)) {
				PRINT_RED("%3d ", ch(i, j));
				PRINT_GREEN("(%3d)", ref(i, j));
			} else {
				PRINT("%3d ", ch(i, j));
			}
		}
		PRINT("\n");
	}
}


#endif // IMAGECHANNEL_H