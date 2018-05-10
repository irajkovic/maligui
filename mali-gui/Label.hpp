#pragma once

#include "Painter.hpp"
#include "Widget.hpp"

#include <QColor>

namespace maligui
{

template<typename TPixel, typename TSize = int>
class Button : public Widget<TPixel>
{
public:
	Button<TPixel, TSize>() : mText("") {
	}

	virtual void onPaint (Painter<TPixel> &painter) override
	{
		painter.fill(mBackgroundColor);
		painter.color(mForegroundColor);
		painter.rect(this->mGeometry);
		painter.write(this->geometry().start(), mText.c_str());
	}

	virtual TPixel backgroundColor ()
	{
		return this->mBackgroundColor;
	}

	virtual void backgroundColor (const TPixel color)
	{
		this->mBackgroundColor = color;
	}

	virtual TPixel foregroundColor ()
	{
		return this->mForegroundColor;
	}

	virtual void foregroundColor (const TPixel color)
	{
		this->mForegroundColor = color;
	}

	virtual TPixel borderColor ()
	{
		return this->mBorderColor;
	}

	virtual void borderColor (const TPixel color)
	{
		this->mBorderColor = color;
	}

	virtual void text (std::string t)
	{
		this->mText = t;
	}

private:
	TPixel mBackgroundColor;
	TPixel mForegroundColor;
	TPixel mBorderColor;
	std::string mText;
};

} /* namespace maligui */
