#pragma once

#include <vector>
#include <functional>

#include "Painter.hpp"
#include "Rectangle.hpp"

namespace maligui
{

template<typename TPixel, typename TSize = int>

/**
 * @brief The Widget class defines a base class for all Gui components.
 * Objects of the Widget class can contain other Widgets, creating a hierarchy
 * of displayable components. The Widget class handles events such as click /
 * pres by propagating these events recursively throughout the hierarchy.
 */
class Widget
{
typedef std::function<bool (Widget<TPixel, TSize> *, Point<TSize> )> EventHandler;

public:
	/**
	 * @brief Widget
	 */
	Widget(Rectangle<TSize> geometry =
		{
			0, 0, 0, 0
		}) : mGeometry { geometry }
	{ }

	/**
	 * @brief Propagates click to child widgets. Event will be propagated only
	 * to the children which contain the coordinates of the click event.
	 * Child's event handling will be performed first (as child lies on top of
	 * the parent). By default, the event will then be propagated to the
	 * parent and further up the hierarchy, up to the top level widget. This
	 * behaviour can be prevent by returning false from the onClick() in some
	 * of the child objects.
	 * @param p
	 * @return
	 */
	bool propagateClick (const Point<TSize> &p)
	{
		bool handle = true;

		for (const auto &child : mChildren) {
			if (child->geometry().contains(p)) {
				handle = child->propagateClick(p);
				break;
			}
		}
		if (handle) {
			handle = onClick(p);
		}

		return handle;
	}

	/**
	 * @brief Adds a new child to the widget. Widget will own the child, so
	 * rvalue reference is required.
	 * @param child An rvalue reference to the child widget.
	 */
	void addChild (std::unique_ptr<Widget<TPixel>> &&child)
	{
		mChildren.push_back(std::move(child));
	}

	/**
	 * @brief Returns child at the given position. Children are ordered
	 * sequentially, with index 0 pointing to the first child.
	 * @param i Position of the child.
	 * @return Raw (non-owning) pointer to the child widget.
	 */
	Widget<TPixel> *childAt (int i)
	{
		return mChildren.at(i).get();
	}

	/**
	 * @brief Returns number of children.
	 * @return Number of children.
	 */
	size_t childrenCount ()
	{
		return mChildren.size();
	}

	Rectangle<TSize> geometry ()
	{
		return mGeometry;
	}

	void geometry (Rectangle<TSize> g)
	{
		mGeometry = g;
	}

	virtual bool onClick (const Point<TSize> &p)
	{
		if (mClickHandler) {
			return mClickHandler(this, p);
		}
		return true;
	}

	virtual void onClickHandler (EventHandler handler)
	{
		mClickHandler = handler;
	}

	virtual void initPainter (std::shared_ptr<Device<TPixel, TSize>> device)
	{
		// Each widget has it's own painter, so it can change the painter
		// properties without affecting other widgets.
		mPainter = std::make_unique<Painter<TPixel>>(device, geometry());
		// Let child widgets create their own painter objects.
		for (auto &child : mChildren) {
			child->initPainter(device);
		}
	}

	virtual void onPaint ()
	{
		for (auto &child : mChildren) {
			child->onPaint();
		}
	}

	virtual unsigned int id ()
	{
		return mId;
	}

	virtual void id (unsigned int id)
	{
		mId = id;
	}

	virtual Painter<TPixel> *getPainter ()
	{
		return mPainter.get();
	}

protected:
	//Painter<TPixel> &painter;
	unsigned int mId;
	EventHandler mClickHandler;
	Rectangle<TSize> mGeometry;
	std::unique_ptr<Painter<TPixel>> mPainter;
	std::vector<std::unique_ptr<Widget>> mChildren;

};

} /* namespace maligui */
