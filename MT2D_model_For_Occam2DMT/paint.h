#ifndef PAINT_H
#define PAINT_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QPoint>
#include <QPointF>
#include <QLine>
#include <QLineF>
#include <QRect>
#include <QRectF>
#include <QVector>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QPixmap>
#include <QImage>
#include <QAction>
#include <QMenu>
#include <QClipboard>
#include <QTransform>
#include "colorscale.h"

typedef struct
{
    QVector < QPointF > data;
    QPen pen;
}point;

typedef struct
{
    QVector < QLineF > data;
    QPen pen;
}line;

typedef struct
{
    QVector < QRectF > data;
    QPen pen;
    QBrush brush;
}rectangle;

namespace Ui {
class paint;
}

class paint : public QWidget
{
    Q_OBJECT

public:
    explicit paint(QWidget *parent = nullptr);
    ~paint();

private:
    Ui::paint *ui;
    QVector < point > points;
    QVector < line > lines;
    QVector < rectangle > rects;
    QRectF boundary;
    QPointF origin;
    QPointF initialPoint,offset;
    QPointF currentPoint,beforePoint;
    ColorMap colorMap;
    bool pressed,ctrl,alt;
    QPointF pointA,pointB;
    QMatrix OriginalMatrix,TemporaryMatrix,TransformMatrix,CurrentMatrix;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:
    void setBoundary(QRectF _rect);
    void setdatas(QVector < point > _points,QVector < line > _lines,QVector < rectangle > _rects);
    void setColorMap(ColorMap _colorMap);

signals:
    void sendPoint(QPointF _point);
    void sendPoint(QPointF _pointA,QPointF _pointB);

};

#endif // PAINT_H
