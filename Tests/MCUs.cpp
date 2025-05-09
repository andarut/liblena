#include <gtest/gtest.h>

#include "MCUs.hpp"

inline const auto TEST_ch = ImageChannel<u8>(8, 8, {
	255, 255, 255, 255, 255, 255, 255, 255,
	154, 154, 154, 154, 154, 154, 154, 154, 
	153, 153, 153, 153, 153, 153, 153, 153,
	102, 102, 102, 102, 102, 102, 102, 102, 
	255, 255, 255, 255, 255, 255, 255, 255,
	154, 154, 154, 154, 154, 154, 154, 154, 
	153, 153, 153, 153, 153, 153, 153, 153,
	102, 102, 102, 102, 102, 102, 102, 102, 
});

TEST(TEST_MCUs, _8x8) {
	auto _MCUs = MCUs(TEST_ch, {8, 8});
	for (auto& _MCU : _MCUs) {
		EXPECT_EQ(_MCU.width(), 8);
		EXPECT_EQ(_MCU.height(), 8);
	}
}