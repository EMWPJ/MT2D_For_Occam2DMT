#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define AirRho 18
#include <QPointF>
#include <QString>
#include <QVector>
#include <QRectF>
#include <QDebug>
double VHdistance(QPointF _p1,QPointF _p2);
double distance(QPointF _p1,QPointF _p2);
double minDistance(QVector < QPointF > _p1,QPointF _p2);
bool onlineAbove(QVector < QPointF > _p1,QPointF _p2);
QString Q2F(double temp, QString ef, int a, int b);
QString Q2F(int temp, int a);
template <typename T>T GetMax(T a,T b);
template <typename T>T GetMax(QVector <T> a);
template <typename T>T GetMin(T a,T b);
template <typename T>T GetMin(QVector <T> a);
bool sameNumber(qreal a,qreal b);
qreal log10Rho(qreal _rho);
qreal pow10Rho(qreal _rho);
bool inUnit(QRectF _unit, QPointF _pointA, QPointF _pointB);

#endif // FUNCTIONS_H
