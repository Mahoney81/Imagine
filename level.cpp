
#include "level.h"

const PixmapRect Level::put(const PixmapRect& rect, bool f, bool leftJustified)
{
    PixmapRect newRect(rect.pixmap());
    
    if (f)
    {
        if (leftJustified)
        {
            newRect.setRect(floor, windowHeight - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
        }
        else
        {
            // 'ceiling' is used for right-justified rectangles packed on the floor
            newRect.setRect(windowWidth - (ceiling + rect.width()),
                            windowHeight - (bottom + rect.height() + 1),
                            rect.width(), rect.height());
            ceiling += rect.width();
        }
        floor += rect.width();
    }
    else
    {
        newRect.setRect(windowWidth - (ceiling + rect.width()),
                        windowHeight - (bottom + height + 1),
                        rect.width(), rect.height());
        ceiling += rect.width();
    }
    
    return newRect;
}

bool Level::ceilingFeasible(const PixmapRect& rect, const QList<PixmapRect>& existing)
{
    PixmapRect testRect(rect.pixmap());
    testRect.setRect(windowWidth - (ceiling + rect.width()),
                     windowHeight - (bottom + height + 1),
                     rect.width(), rect.height());
    bool intersected = false;
    for (int i = 0; i < existing.size(); i++)
    {
        if (testRect.intersects(existing[i]))
        {
            intersected = true;
            break;
        }
    }
    bool fit = rect.width() <= (windowWidth - ceiling - initW);
    return fit && !intersected;
}

bool Level::floorFeasible(const PixmapRect &rect)
{
    return rect.width() <= (windowWidth - floor);
}

int Level::getSpace(bool f)
{
    if (f)
    {
        return windowWidth - floor;
    }
    else
    {
        return windowHeight - ceiling - initW;
    }
}
