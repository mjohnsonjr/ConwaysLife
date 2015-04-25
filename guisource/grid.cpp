#include <QtGui>
#include <QMouseEvent>
#include "grid.h"
#include "globals.h"
#include "ui_grid.h"

/* Grid is the Widget contained in the MainWindow's scroll area that contains the cell graphics.
    All of grid's Slots and methods are described below */

Grid::Grid(QWidget *parent):QWidget(parent),ui(new Ui::Grid)
{
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    curColor = Qt::black;
    zoom = zoomfactor;
    ui->setupUi(this);
}

QSize Grid::sizeHint() const
{
        QSize size = zoom * image.size();
        if (zoom >= 3) size += QSize(1, 1);
        return size;
}

/* Sets the current pen color to the specified QColor */
void Grid::setPenColor(const QColor &newColor)
{
        curColor = newColor;
}


void Grid::setIconImage(const QImage &newImage)
{
    if (newImage != image)
    {
        image = newImage.convertToFormat(QImage::Format_ARGB32);
        update();
        updateGeometry();
    }
}

void Grid::setZoomFactor(int newZoom)
{
    if (newZoom < 1) newZoom = 1;

    if (newZoom != zoom)
    {
        zoom = newZoom;
        update();
        updateGeometry();
    }
}

void Grid::paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        if(zoom >= 3)
        {
            painter.setPen(palette().foreground().color()); for (int i = 0; i <= image.width(); ++i)
                painter.drawLine(zoom * i, 0, zoom * i, zoom * image.height());
            for (int j = 0; j <= image.height(); ++j)
                painter.drawLine(0, zoom * j, zoom * image.width(), zoom * j);
        }

    for (int i = 0; i < image.width(); ++i)
    {
        for (int j = 0; j < image.height(); ++j)
        {
            QRect rect = pixelRect(i, j);
            if (!event->region().intersect(rect).isEmpty())
            {
                     QColor color = QColor::fromRgba(image.pixel(i, j));
                     painter.fillRect(rect, color);
            }
        }
    }
}


QRect Grid::pixelRect(int i, int j) const
{
    if (zoom >= 3)
    {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    }
    else
    {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}


/* Sets the pixel to either the current pen color, or opaque at the specified QPoint */
void Grid::setImagePixel(const QPoint &pos, bool opaque)
{
    int i = pos.x()/zoom;
    int j = pos.y()/zoom;

    if (image.rect().contains(i, j))
    {
        if(opaque)
        {
            image.setPixel(i, j, penColor().rgba());
        }
        else
        {
            image.setPixel(i, j, qRgba(0, 0, 0, 0));
        }

        update(pixelRect(i, j));
    }
}


Grid::~Grid()
{
    delete ui;
}
