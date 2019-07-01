
#ifndef IMAGINE_PIXMAPRECT_H
#define IMAGINE_PIXMAPRECT_H

#include <QRect>
#include <QPixmap>

class PixmapRect : private QRect
{
public:
    explicit PixmapRect(const QPixmap& pixmap) : m_pixmap(pixmap)
    {
        const int baseSize = 256;
        
        setX(0);
        setY(0);
        
        const double aspectRatio = pixmap.width() / double(pixmap.height());
        if(aspectRatio > 1.0)        // horizontally oriented
        {
            setWidth(baseSize);
            setHeight(baseSize / aspectRatio);
        }
        else
        {
            setHeight(baseSize);
            setWidth(baseSize * aspectRatio);
        }
    }
    
    PixmapRect(const PixmapRect& other)  : QRect(other.rect())
    {
        setX(other.x());
        setY(other.y());
        setWidth(other.width());
        setHeight(other.height());
        m_pixmap = other.m_pixmap;
    }
    
    PixmapRect& operator=(const PixmapRect& other)
    {
        setX(other.x());
        setY(other.y());
        setWidth(other.width());
        setHeight(other.height());
        m_pixmap = other.m_pixmap;
        return *this;
    }
    
    QPixmap pixmap(void) const { return m_pixmap; }
    
    void setRect(int x, int y, int width, int height) { QRect::setRect(x, y, width, height); }
    
    int width(void) const { return QRect::width(); }
    int height(void) const { return QRect::height(); }
    
    int top(void) const { return QRect::top(); }
    int bottom(void) const { return QRect::bottom(); }
    
    QRect rect() const { return *this; }
    
    bool intersects(const PixmapRect& rect) { return m_pixmap.rect().intersects(rect.rect()); }

private:
    QPixmap m_pixmap;
};

#endif //IMAGINE_PIXMAPRECT_H
