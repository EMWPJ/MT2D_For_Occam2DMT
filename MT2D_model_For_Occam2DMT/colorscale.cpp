#include "colorscale.h"
#include "ui_colorscale.h"
#include <QDebug>

ColorScale::ColorScale(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorScale)
{
    ui->setupUi(this);
    value = 50;
}

ColorScale::~ColorScale()
{
    delete ui;
}

void ColorScale::paintEvent(QPaintEvent *)
{
    QPainter draw(this);
    draw.setRenderHint(QPainter::Antialiasing, true);
    int xmin = this->geometry().x();
    int xmax = this->geometry().x() + this->geometry().width();
    int ymin = this->geometry().y();
    int ymax = this->geometry().y() + this->geometry().height();
    QLinearGradient linearGradient(xmin, ymax, xmin, ymin);
    int count = colorMap.value.count();
    for(int i=0;i<count;i++)
    {
        linearGradient.setColorAt(colorMap.value.at(i)/100,QColor(colorMap.red.at(i),colorMap.green.at(i),colorMap.blue.at(i),colorMap.alpha.at(i)));
    }
    draw.setPen(QPen(QBrush(linearGradient),this->geometry().width()));
    draw.drawLine(xmin, ymax, xmin, ymin);
    draw.setPen(QPen(Qt::black, 6, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    int y = this->geometry().y() + this->geometry().height() - this->geometry().height() * value / 100;
    draw.drawLine(xmin-3,y,xmax-3,y);
}

void ColorScale::setColorMap(ColorMap _colorMap)
{
    colorMap = _colorMap;
    repaint();
}

void ColorScale::mouseDoubleClickEvent(QMouseEvent *e)
{
    double y = e->y();
    value = (this->geometry().y()+this->geometry().height()-y)/this->geometry().height()*100;
    repaint();
    emit setColorValue(value);
    e->accept();
}


void ColorScale::setCurrentValue(double _value)
{
    value = _value;
    repaint();
}
