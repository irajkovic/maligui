#pragma once

#include "Point.hpp"

namespace maligui
{

template<typename TSize = int>
struct Rectangle {
	TSize x;
	TSize y;
	TSize width;
	TSize height;

	inline bool contains (const Point<TSize> &p)
	{
		return p.x >= x &&
		       p.x <= x + width &&
		       p.y >= y &&
		       p.y <= y + height;
	}

	Point<TSize> start ()
	{
		return Point<TSize> { x, y };
	}

};

} /* namespace maligui */
