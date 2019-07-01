#include "globals.h"
#include "ImageWidget.h"
#include "level.h"

ImageWidget::ImageWidget(QWidget *pParent)
{
    connect(&m_loadFutureWatcher, &QFutureWatcher<bool>::finished, this, &ImageWidget::onImagesLoaded);
}

void ImageWidget::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent);
    
    QPainter painter(this);
    
    painter.fillRect(rect(), Qt::black);
    
    if(m_pixmap.isNull())
        return;
 
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    
    if(!m_isCollage)
    {
        const double aspectRatio = m_pixmap.width() / double(m_pixmap.height());
        if (aspectRatio == 0.0)
            return;
        if (aspectRatio > 1.0)       // horizontal image
        {
            width = this->width();
            height = width / aspectRatio;
        
            if (height < this->height())
            {
                y += (this->height() - height) / 2;
            }
        }
        else                        // vertical image
        {
            height = this->height();
            width = height * aspectRatio;
        
            if (width < this->width())
            {
                x += (this->width() - width) / 2;
            }
        }
    }
    else
    {
        width = m_pixmap.width();
        height = m_pixmap.height();
    }
    
	painter.drawPixmap(x, y, width, height, m_pixmap);
}

void ImageWidget::resizeEvent(QResizeEvent *pEvent)
{
	m_pixmap = QPixmap(pEvent->size());
	m_pixmap.fill(QColor(Qt::black));
	update();
}

void ImageWidget::setImagesFolder(const QString& folderPath)
{
	//m_loadFuture = QtConcurrent::run(&this->m_pixmap, static_cast<bool(QPixmap::*)(const QString&, const char*, Qt::ImageConversionFlags)>(&QPixmap::load), filePath, nullptr, nullptr);
    m_loadFuture = QtConcurrent::run(this, &ImageWidget::loadImages, folderPath);
	m_loadFutureWatcher.setFuture(m_loadFuture);
}

void ImageWidget::onImagesLoaded(void)
{
	update();
}

bool ImageWidget::loadImages(const QString& folderPath)
{
    qDebug() << "Loading images from " << folderPath;
    
    m_isCollage = false;
    
    QFileInfo fileInfo(folderPath);
    if(fileInfo.isFile())
    {
        return m_pixmap.load(fileInfo.absoluteFilePath());
    }
    else if(fileInfo.isDir())
    {
        QList<QPixmap> pixmapList;
        for(const QFileInfo& fileInfo : QDir(fileInfo.absoluteFilePath()).entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
        {
            QPixmap pixmap;
            if(pixmap.load(fileInfo.absoluteFilePath()))
            {
                qDebug() << "Loaded " << fileInfo.absoluteFilePath();
                pixmapList << pixmap;
            }
        }
        m_pixmap = createCollage(pixmapList);
    }
    else
    {
        m_pixmap = QPixmap();
    }
    
    return false;
}

QPixmap ImageWidget::createCollage(const QList<QPixmap>& pixmapList)
{
    m_isCollage = true;
    
    QList<PixmapRect> rectList;
    for(const QPixmap& pixmap : pixmapList)
    {
        PixmapRect rect(pixmap);
        rectList << rect;
    }
    rectList = pack(rectList);
    return paintCollage(rectList);
}

static bool decreasingHeightComparision(const PixmapRect& r1, const PixmapRect& r2)
{
    return r1.height() >= r2.height();
}

const QList<PixmapRect> ImageWidget::pack(const QList<PixmapRect>& rects)
{
    QList<PixmapRect> unpacked = rects;
    qSort(unpacked.begin(), unpacked.end(), decreasingHeightComparision);
    
    QList<Level> levels;
    Level level(0, unpacked[0].height(), 0, unpacked[0].width(), width(), height());
    QList<PixmapRect> packed;
    
    packed.push_back(level.put(unpacked[0]));
    levels.push_back(level);
    
    for (int i = 1; i < unpacked.size(); i++)
    {
        int found = -1;
        int min = width();
        for (int j = 0; j < levels.size(); j++)
        {
            if (levels[j].floorFeasible(unpacked[i]))
            {
                if (levels[j].getSpace() < min)
                {
                    found = j;
                    min = levels[j].getSpace();
                }
            }
        }
        if (found > -1)
        { // floor-pack on existing level
            packed.push_back(levels[found].put(unpacked[i]));
        }
        else
        {
            int found = -1;
            int min = width();
            for (int j = 0; j < levels.size(); j++)
            {
                if (levels[j].ceilingFeasible(unpacked[i], packed))
                {
                    if (levels[j].getSpace(false) < min)
                    {
                        found = j;
                        min = levels[j].getSpace(false);
                    }
                }
            }
            if (found > -1)
            { // ceiling-pack on existing level
                packed.push_back(levels[found].put(unpacked[i], false));
            }
            else
            { // a new level
                Level newLevel(levels.last().bottom + levels.last().height,
                                         unpacked[i].height(), 0, unpacked[i].width(), width(), height());
                packed.push_back(newLevel.put(unpacked[i]));
                levels.push_back(newLevel);
            }
        }
    }
    return packed;
}

QPixmap ImageWidget::paintCollage(const QList<PixmapRect>& rects)
{
    const int width = this->width();
    const int height = calculateHeight(rects);
    
    QPixmap pixmap(width, height);
    QPainter painter(&pixmap);
    
    for(const PixmapRect& rect : rects)
    {
        painter.drawPixmap(rect.rect(), rect.pixmap());
    }
    
    return pixmap;
}

int ImageWidget::calculateHeight(const QList<PixmapRect>& rects)
{
    int min = 0, max = 0;
    for(const PixmapRect& rect : rects)
    {
        if(rect.top() < min)
            min = rect.top();
        if(rect.bottom() > max)
            max = rect.bottom();
    }
    
    return max - min;
}

