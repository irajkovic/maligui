#pragma once

#include <memory>
#include <vector>

#include "Point.hpp"

namespace maligui
{

template<typename TPixel, typename TSize = int>
/**
 * @brief The Device class wraps input/output functionalities of a hardware
 * device and serves as a Hardware Abstraction Layer. It handles output via call
 * to the setXY() method, while eventHandler can be registered to receive the
 * input in an asynchronous manner. The only supported input at this moment is
 * the "spatial" input (click/press/touch type of an event).
 *
 * This is an abstract base class. The actuall device implementation only needs
 * to:
 * 1) Implement pure virtual methods such as setXY() and get().
 * 2) Call onPress() method whenever a press to the screen area is registered.
 *
 * @see DeviceEmulator class for an example of implementation.
 */
class Device
{
protected:
	/**
	 * @brief EventHandler functor signature. It must be a callable object that
	 * takes a parameter of type Point<TSize> and has no return value.
	 */
	typedef std::function<void (Point<TSize> )> EventHandler;

	/**
	 * @brief mWidth Width of the device display, in pixels.
	 */
	TSize mWidth;

	/**
	 * @brief mHeight Height of the device display, in pixels.
	 */
	TSize mHeight;

	/**
	 * @brief mSize Size of the device, in pixels.
	 */
	TSize mSize;

	/**
	 * @brief mHandler Functor object used to pass the user input to.
	 */
	EventHandler mHandler;

public:
	Device(TSize width,
	       TSize height) :
		mWidth{width},
		mHeight{height},
		mSize{width * height}
	{
	}

	inline TSize width () const
	{
		return mWidth;
	}

	inline TSize height () const
	{
		return mHeight;
	}

	inline TSize size () const
	{
		return mSize;
	}

	inline virtual void setXY (TSize x, TSize y, TPixel t) = 0;

	inline virtual void setXYBlended (TSize x, TSize y, TPixel color, uint8_t intensity) = 0;

	inline virtual TPixel getXY (TSize x, TSize y) = 0;

	inline virtual TPixel get (TSize offset) const = 0;

	inline void registerEventHandler (EventHandler handler)
	{
		mHandler = handler;
	}

	inline virtual void onPress (Point<TSize> point)
	{
		if (mHandler) {
			mHandler(point);
		}
	}

};

} /* namespace maligui */
