#pragma once

#include "Alignment.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "Device.hpp"

#include "fonts/bebas_neue_bold_17.hpp"

namespace maligui
{

template <typename TPixel, typename TSize = int>
/**
 * @brief The Painter class allows drawing of the basic geometric shapes to the
 * part of the screen defined by the widget's geometry. It also supports more
 * advanced features, such as text painting.
 */
class Painter
{
public:
    Painter(std::shared_ptr<Device<TPixel, TSize>> surface,
            Rectangle<TSize> geometry)
        : mDevice(surface),
          mGeometry(geometry)
    {
    }

    inline void color(TPixel color)
    {
        mColor = color;
    }

    inline TPixel color()
    {
        return mColor;
    }

    inline void point(TSize x, TSize y) {
        mDevice->setXY(x, y, mColor);
    }

    /**
     * @brief fill Fills the entire widget geometry with the provided color.
     * @param color
     */
    inline void fill(const TPixel &color)
    {
        for (auto i = 0 ; i < mGeometry.width ; ++i) {
            for (auto j = 0 ; j < mGeometry.height ; ++ j) {
                mDevice->setXY(mGeometry.x + i, mGeometry.y + j, color);
            }
        }
    }

    inline void fill()
    {
        fill(mColor);
    }

    /**
     * Bresenham line drawing.
     * https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm
     */
    inline void line(TSize x1, TSize y1, TSize x2, TSize y2) {
        TSize dx = abs(x2-x1);
        TSize dy = abs(y2-y1);
        TSize sx = x1 < x2 ? TSize{1} : TSize{-1};
        TSize sy = y1 < y2 ? TSize{1} : TSize{-1};
        TSize err1 = (dx > dy ? dx : -dy) / 2;
        TSize err2;

        for (;;) {
            point(x1, y1);
            if (x1 == x2 && y1 == y2) {
                break;
            }
            err2 = err1;
            if (err2 > -dx) {
                err1 -= dy;
                x1 += sx;
            }
            if (err2 < dy) {
                err1 += dx;
                y1 += sy;
            }
        }
    }

    inline void rect(const Rectangle<TSize> &rect)
    {
        line(rect.x, rect.y, rect.x + rect.width - 1, rect.y);
        line(rect.x, rect.y + rect.height - 1, rect.x + rect.width - 1, rect.y + rect.height - 1);
        line(rect.x, rect.y, rect.x, rect.y + rect.height - 1);
        line(rect.x + rect.width - 1, rect.y, rect.x + rect.width - 1, rect.y + rect.height - 1);
    }

    inline int writeWidth(const std::string &text)
    {
        int width = 0;

        for (auto ch : text) {
            char ind = ch - 32;
            int *geometry = bebas_neue_bold_17_map[ind];
            width += geometry[FONT_WIDTH] + 2;
        }

        return width;
    }

    TSize getHorizontalyAlignedPosition(const std::string &text,
                                    align::Horizontal horizontalAlign)
    {
        TSize x = 0;

        switch(horizontalAlign) {
        case align::Horizontal::LEFT:
            x = mGeometry.x;
            break;
        case align::Horizontal::CENTER:
            x = mGeometry.x + (mGeometry.width - writeWidth(text)) / 2;
            break;
        case align::Horizontal::RIGHT:
            x = mGeometry.x + mGeometry.width - writeWidth(text);
            break;
        }

        return x;
    }

    TSize getVerticalyAlignedPosition(align::Vertical verticalAlign)
    {
        TSize y = 0;

        switch(verticalAlign) {
        case align::Vertical::TOP:
            y = mGeometry.y;
            break;
        case align::Vertical::CENTER:
            y = mGeometry.y + (mGeometry.height - FONT_HEIGHT) / 2;
            break;
        case align::Vertical::BOTTOM:
            y = mGeometry.y + mGeometry.height - FONT_HEIGHT;
            break;
        }

        return y;
    }


    inline void write(const std::string &text,
                      align::Horizontal horizontalAlign = align::Horizontal::CENTER,
                      align::Vertical verticalAlign = align::Vertical::CENTER)
    {

        TSize destX = getHorizontalyAlignedPosition(text, horizontalAlign);
        TSize destY = getVerticalyAlignedPosition(verticalAlign);

        for (const auto &ch : text) {
            char ind = ch - 32;
            int *geometry = bebas_neue_bold_17_map[ind];

            int x = geometry[FONT_X];
            int y = geometry[FONT_Y];
            int w = geometry[FONT_WIDTH];
            int h = geometry[FONT_HEIGHT];

            for (int j = 0; j < h ; ++j) {
                for (int i = 0; i < w ; ++i) {
                    bool set = bebas_neue_bold_17_xpm[j+y][i+x] == '.';
                    if (set) {
                        point(destX + i, destY + j);
                    }
                }
            }

            destX += w + 2;
        }
    }

    inline Device<TPixel, TSize>* device() { return &mDevice; }

private:
    std::shared_ptr<Device<TPixel, TSize>> mDevice;
    Rectangle<TSize> mGeometry;
    TPixel mColor;
};

} /* namespace maligui */
