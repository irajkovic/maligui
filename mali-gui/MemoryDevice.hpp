#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "Device.hpp"

namespace maligui
{

template<typename TPixel, typename TSize = int>
class MemoryDevice : public Device<TPixel, TSize>
{
public:
	MemoryDevice(TSize width,
	             TSize height) :
		Device<TPixel, TSize>{width, height},
		mBuffer{std::vector<TPixel>(this->mSize)}
	{
	}

	inline virtual void set (TSize offset, TPixel t) override
	{
		mBuffer[offset] = t;
	}

	inline virtual TPixel get (TSize offset) const override
	{
		return mBuffer[offset];
	}

private:
	std::vector<TPixel> mBuffer;
};

} /* namespace maligui */
