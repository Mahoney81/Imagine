
#ifndef IMAGINE_LEVEL_H
#define IMAGINE_LEVEL_H

#include "PixmapRect.h"

class Level
{
public:
    Level(int b, int h, int f, int w, int ww, int wh) : bottom(b),
                                                        height(h),
                                                        floor(f),
                                                        initW(w),
                                                        windowWidth(ww),
                                                        windowHeight(wh),
                                                        ceiling(0)
    {}
    
    const PixmapRect put(const PixmapRect& rect, bool f = true, bool leftJustified = true);
    bool ceilingFeasible(const PixmapRect& rect, const QList<PixmapRect>& existing);
    bool floorFeasible(const PixmapRect& rect);
    int getSpace(bool f = true);
    
    int bottom;
    int height;
    int floor;
    int initW;
    int ceiling;
    int windowWidth;
    int windowHeight;
};

#endif //IMAGINE_LEVEL_H
