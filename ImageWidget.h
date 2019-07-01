
#ifndef NIXIMAGE_IMAGEWIDGET_H
#define NIXIMAGE_IMAGEWIDGET_H

#include <QWidget>
#include "PixmapRect.h"

class ImageWidget : public QWidget
{
public:
	ImageWidget(QWidget *pParent);
	
	void setImagesFolder(const QString& folderPath);

protected:
	virtual void paintEvent(QPaintEvent *pEvent) override;
	virtual void resizeEvent(QResizeEvent *pEvent) override;

protected slots:
	void onImagesLoaded(void);

private:
    QPixmap m_pixmap;
    bool m_isCollage = false;
	QFuture<bool> m_loadFuture;
	QFutureWatcher<bool> m_loadFutureWatcher;

private:
    bool loadImages(const QString& folderPath);
    QPixmap createCollage(const QList<QPixmap>& pixmapList);
    
    const QList<PixmapRect> pack(const QList<PixmapRect>& rects);
    QPixmap paintCollage(const QList<PixmapRect>& rects);
    int calculateHeight(const QList<PixmapRect>& rects);
};


#endif //NIXIMAGE_IMAGEWIDGET_H
