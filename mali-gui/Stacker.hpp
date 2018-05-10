#include <stack>
#include <functional>

#include "MemoryDevice.hpp"
#include "Widget.hpp"
#include "Painter.hpp"

namespace maligui
{

template<typename TPixel, typename TSize = int>
/**
 * @brief The Stacker class provides basic functionality needed to transition
 * between the independent widgets. It is also a single way to communicate to
 * the underlying device. Only a single widget can be active (on the top of the
 * stacker). That widget (and consequenctially, it's children) will receive the
 * press events. When a new widget is pushed to the stacker, it will be
 * considered active, and the press events will be delivered to that new widget
 * only. Any previously pushed widgets will be hidden behind, and will be shown
 * only after pop is called to remove the last added window. As the name
 * suggests, stacker is LIFO, so last added widget is the first one that will be
 * poped.
 */
class Stacker
{
public:
	Stacker(std::shared_ptr<Device<TPixel>> device) :
		mDevice(device)
	{
		device->registerEventHandler(std::bind(&Stacker::onClick, this,
		                                       std::placeholders::_1));
	}

	void push (Widget<TPixel, TSize> *widget)
	{
		mStack.push(widget);
		widget->initPainter(mDevice);
		widget->onPaint();
	}

	void refresh ()
	{
		mStack.top()->onPaint();
	}

	void onClick (Point<TSize> point)
	{
		mStack.top()->propagateClick(point);
	}

	inline TSize width () const
	{
		return mDevice->width();
	}

	inline TSize height () const
	{
		return mDevice->height();
	}

	inline TSize size () const
	{
		return mDevice->size();
	}

private:
	std::stack<Widget<TPixel, TSize> *> mStack;
	std::shared_ptr<Device<TPixel>> mDevice;
};

} /* namespace maligui */
