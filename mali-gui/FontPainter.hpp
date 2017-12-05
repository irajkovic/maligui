
#include "fonts/bebas_neue_bold_17.hpp"

namespace maligui
{

template <typename TPixel, typename TSize = int>
class FontPainter
{
public:
    FontPainter(Device<TPixel, TSize> &device) : mDevice(device) {}

    void write(const Point<TSize> point, char *text)
    {
        char *ch = text;
        while (ch != '\0') {
            char ind = *ch - 32;
            int *geometry = bebas_neue_bold_17_map[ind];

            int x = geometry[FONT_X];
            int y = geometry[FONT_Y];
            int w = geometry[FONT_WIDTH];
            int h = geometry[FONT_HEIGHT];

            TSize destX = point.x;
            TSize destY = point.y;

            for (int j = 0; j < h ; ++j) {
                for (int i = 0; i < w ; ++i) {
                    bool set = bebas_neue_bold_17_xpm[j+y][i+x];
                    device.setXYt(destX + i, destY + j);
                }
            }
        }
    }

private:
    Device<TPixel, TSize> &mDevice;

};

} /* namespace maligui */
