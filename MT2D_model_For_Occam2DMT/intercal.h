#ifndef INTERCAL_H
#define INTERCAL_H
#include <QVector>
#include <QPointF>
QVector <QPointF> sortPoint(QVector <QPointF> data);
QVector <double> sortDouble(QVector <double> data);
QVector<QPointF> secDerv(QVector<QPointF> info);//二阶求导
double splint(QVector<QPointF> org, QVector<QPointF> sp, double Y);
QVector<QPointF> spline(QVector<QPointF> info, QVector<double> Ys);//三次样条插值

#endif // INTERCAL_H
