#include <functional>
#include <memory>

#include "Widget.hpp"

namespace maligui
{

template <typename TPixel, typename TSize = int>
/**
 * @brief Arranges children widgets of the container into the grid.
 * Widgets will be added left to right, top to bottom. Once all child widgets
 * are arranged, the method will return, even if the number of widgets is less
 * than numHor x numVer.
 * @param container     Widget containing child widgets that will be arranged
 *                      into the grid.
 * @param numHor        Number of horizontal widgets (i.e. columns)
 * @param numVer        Number of rows of widgets
 * @param paddingHor    Horizontal padding, in pixels.
 * @param paddingVer    Vertical padding, in pixels.
 */
void makeGrid(Widget<TPixel, TSize>* container,
              int numHor = 0,
              int numVer = 0,
              TSize paddingHor = 0,
              TSize paddingVer = 0
        )
{
    Rectangle<TSize> geometry = container->geometry();

    TSize stepHor = (geometry.width - paddingHor * (numHor - 1)) / numHor;
    TSize stepVer = (geometry.height - paddingVer * (numVer - 1)) / numVer;
    TSize curY = geometry.y;

    size_t childrenCnt = container->childrenCount();
    size_t childIndex = 0;

    for (int i=0; i<numVer; ++i) {

        TSize curX = geometry.x;

        for (int j=0; j<numHor; ++j) {

            if (childIndex == childrenCnt) {
                goto exit;
            }

            auto widget = container->childAt(childIndex);
            widget->geometry(Rectangle<int>{curX, curY, stepHor, stepVer});

            ++childIndex;
            curX += stepHor + paddingHor;
        }

        curY += stepVer + paddingVer;
    }

    exit: ;
}

} /* namespace maligui */
