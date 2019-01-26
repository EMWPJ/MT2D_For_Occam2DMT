#ifndef COLORSCALE_H
#define COLORSCALE_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>

typedef struct
{
    QVector < double > value;
    QVector < int > red;
    QVector < int > green;
    QVector < int > blue;
    QVector < int > alpha;
}ColorMap;

namespace Ui {
class ColorScale;
}

class ColorScale : public QWidget
{
    Q_OBJECT

public:
    explicit ColorScale(QWidget *parent = 0);
    ~ColorScale();

private:
    Ui::ColorScale *ui;
    ColorMap colorMap;
    double value;

protected:
    void paintEvent(QPaintEvent *);
    void mouseDoubleClickEvent(QMouseEvent *e);

public slots:
    void setColorMap(ColorMap _colorMap);
    void setCurrentValue(double _value);

signals:
    void setColorValue(double _value);

};

#endif // COLORSCALE_H
