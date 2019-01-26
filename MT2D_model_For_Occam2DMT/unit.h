#ifndef UNIT_H
#define UNIT_H

#include <QRectF>


class unit : public QRectF
{
public:
    unit();
    qreal value;
    bool editable;
};

#endif // UNITS_H
