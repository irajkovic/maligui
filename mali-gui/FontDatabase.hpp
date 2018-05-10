#pragma once

#include "mali-gui/Font.hpp"
#include "mali-gui/fonts/autogen.h"

namespace maligui
{

class FontDatabase
{

public:
	static inline const Font *get (const std::string &name = "", int size = 0)
	{
		if (size == 0) {
			return &_FONTS_[0];
		}

		for (const auto &font : _FONTS_) {
			if (font.name() == name && font.size() == size) {
				return &font;

				break;
			}
		}

		return &_FONTS_[0];
	}

};

} /* namespace maligui */
