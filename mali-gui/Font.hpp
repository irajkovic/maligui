#pragma once

#include <vector>
#include <memory>

namespace maligui
{

class Character
{
public:
    Character(int width, std::vector<uint8_t> pixmap) :
        mWidth(width),
        mPixmap(pixmap)
    {
    }

    inline int width()
    {
        return mWidth;
    }

    typename std::vector<uint8_t>::const_iterator begin()
    {
        return mPixmap.begin();
    }

    typename std::vector<uint8_t>::const_iterator end()
    {
        return mPixmap.end();
    }

private:
    int mWidth;
    std::vector<uint8_t> mPixmap;
};

class Font
{
public:
    Font(const std::string &name,
         int size,
         int height,
         int from,
         int to,
         std::vector<Character> characters) :
        mName(name),
        mSize(size),
        mHeight(height),
        mFrom(from),
        mTo(to),
        mCharacters(characters)
    {
    }

    /**
     * @brief character provides access to the font characters.
     * @param ch ASCII code for the character.
     * @retval character object for the asked character, if found in font.
     * @retval empty character object, if not found.
     */
    inline Character character(uint8_t ch) const
    {
        if (ch >= mFrom) {
            uint8_t ind = ch - mFrom;
            if (ind < mCharacters.size()) {
                return mCharacters.at(ind);
            }
        }

        return Character(0, {});
    }

    inline int height() const
    {
        return mHeight;
    }

    inline const std::string& name() const
    {
        return mName;
    }

    inline int size() const
    {
        return mSize;
    }

private:
    std::string mName;
    int mSize;
    int mHeight;
    uint8_t mFrom;
    uint8_t mTo;
    std::vector<Character> mCharacters;

};

}
