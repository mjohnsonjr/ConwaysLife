#ifndef GRID_H
#define GRID_H

#include <QColor>
#include <QImage>
#include <QWidget>

namespace Ui {
class Grid;
}

class Grid : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(QImage iconImage READ iconImage WRITE setIconImage)
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

public:
    explicit Grid(QWidget *parent = 0);
    void setPenColor(const QColor &newColor);
    QColor penColor() const { return curColor; }
    void setZoomFactor(int newZoom);
    int zoomFactor() const { return zoom; }
    void setIconImage(const QImage &newImage);
    QImage iconImage() const { return image; }
    QSize sizeHint() const;
    void setImagePixel(const QPoint &pos, bool opaque);
    ~Grid();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::Grid *ui;
    QRect pixelRect(int i, int j) const;
    QColor curColor;
    QImage image;
    int zoom;


};

#endif
