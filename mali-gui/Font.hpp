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

    inline Character character(char ch) const
    {
        return mCharacters.at(ch - mFrom);
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
    int mFrom;
    int mTo;
    std::vector<Character> mCharacters;

};

}
