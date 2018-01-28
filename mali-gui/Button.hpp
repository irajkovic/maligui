#pragma once

#include <QColor>

#include "Alignment.hpp"
#include "Painter.hpp"
#include "Widget.hpp"
#include "FontDatabase.hpp"

namespace maligui
{

template <typename TPixel, typename TSize = int>
/**
 * @brief The Button class widget implements the traditional push button widget.
 * It consists of a label and is rendered as a rectangle of the given
 * dimensions. It uses Widget parent class onClick handler to call the click
 * handler in an asynchronous way.
 */
class Button : public Widget<TPixel>
{
public:

    /**
     * @brief Constructs a new button object with an empty label.
     */
    Button<TPixel, TSize>() : Widget<TPixel, TSize>(), mText("") {}

    /**
     * @brief Renders the button.
     */
    virtual void onPaint() override
    {
        if (this->mPainter) {
            this->mPainter->fill(mBackgroundColor);
            this->mPainter->color(mForegroundColor);
            this->mPainter->rect(this->mGeometry);
            this->mPainter->setFont(mFont);
            this->mPainter->write(mText.c_str(),
                                  mHorizontalAlign,
                                  mVerticalAlign);
        }

        Widget<TPixel>::onPaint();
    }

    /**
     * @brief backgroundColor
     * @return Background color.
     */
    virtual TPixel backgroundColor()
    {
        return this->mBackgroundColor;
    }

    virtual void backgroundColor(const TPixel color)
    {
        this->mBackgroundColor = color;
    }

    virtual TPixel foregroundColor()
    {
        return this->mForegroundColor;
    }

    virtual void foregroundColor(const TPixel color)
    {
        this->mForegroundColor = color;
    }

    virtual TPixel borderColor()
    {
        return this->mBorderColor;
    }

    virtual void borderColor(const TPixel color)
    {
        this->mBorderColor = color;
    }

    virtual void text(const std::string &text)
    {
        this->mText = text;

        onPaint();
    }

    const std::string& text()
    {
        return this->mText;
    }

    void setFont(const std::string& name, int size)
    {
        mFont = FontDatabase::get(name, size);
    }

    void horizontalAlign(align::Horizontal align)
    {
        this->mHorizontalAlign = align;
    }

    void verticalAlign(align::Vertical align)
    {
        this->mVerticalAlign = align;
    }

private:

    TPixel mBackgroundColor;
    TPixel mForegroundColor;
    TPixel mBorderColor;
    std::string mText;
    const Font * mFont = nullptr;
    align::Horizontal mHorizontalAlign = align::Horizontal::CENTER;
    align::Vertical mVerticalAlign = align::Vertical::CENTER;
};

} /* namespace maligui */
