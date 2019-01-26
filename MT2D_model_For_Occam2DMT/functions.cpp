#include "functions.h"

double distance(QPointF _p1,QPointF _p2)
{
    return sqrt((_p1.x()-_p2.x())*(_p1.x()-_p2.x())+(_p1.y()-_p2.y())*(_p1.y()-_p2.y()));
}

double VHdistance(QPointF _p1,QPointF _p2)
{
//    if(fabs(_p1.x()-_p2.x())>fabs(_p1.y()-_p2.y()))
//    {
//        return fabs(_p1.x()-_p2.x());
//    }
//    else
//    {
//        return fabs(_p1.y()-_p2.y());
//    }
    return GetMax(fabs(_p1.x()-_p2.x()),fabs(_p1.y()-_p2.y()));
}

double Hdistance(QPointF _p1,QPointF _p2)
{
    return fabs(_p1.y()-_p2.y());
}

double Vdistance(QPointF _p1,QPointF _p2)
{
    return fabs(_p1.x()-_p2.x());
}

double minDistance(QVector < QPointF > _p1,QPointF _p2)
{
    double result = 1E10;
    int count = _p1.count();
    for(int i=0;i<count;i++)
    {
        double temp = VHdistance(_p1[i],_p2);
        if(result>temp)
        {
            result = temp;
        }
    }
    return result;
}


bool onlineAbove(QVector < QPointF > _p1,QPointF _p2)
{
    int count = _p1.count();
    int num = -1;
    for(int i=0;i<count;i++)
    {
        if(_p2.x()>_p1[i].x())
        {
            num = i;
        }
    }
    if(num==-1)
    {
        if(_p2.y()>_p1[0].y())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(num==count-1)
    {
        if(_p2.y()>_p1.last().y())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        QPointF _p3 = _p1[num];
        QPointF _p4 = _p1[num+1];
        if(_p2.y()>_p3.y()&&_p2.y()>_p4.y())
        {
            return true;
        }
        else if(_p2.y()<_p3.y()&&_p2.y()<_p4.y())
        {
            return false;
        }
        else if(_p2.y()>(_p4.y()-_p3.y())/(_p4.x()-_p3.x())*(_p2.x()-_p3.x())+_p3.y())
        {
//            qDebug()<<_p2<<_p3<<_p4<<"true";
            return true;
        }
        else
        {
            return false;
        }
    }
}

QString Q2F(double temp, QString ef, int a, int b)
{
    QString string;
    if(ef=="f"||ef=="F")
    {
        string = QString::number(temp,'f',b);
    }
    else if(ef=="e"||ef=="E")
    {
        string = QString::number(temp,'e',b);
    }
    int length = string.count();
    if(a-length<=0)
    {
        string.clear();
        for(int i=0;i<a;i++)
        {
            string += "*";
        }
        return string;
    }
    for(int i=0;i<a-length;i++)
    {
        string.push_front(" ");
    }
    return string;
}

QString Q2F(int temp, int a)
{
    QString string = QString::number(temp);
    int length = string.count();
    if(a-length<=0)
    {
        string.clear();
        for(int i=0;i<a;i++)
        {
            string += "*";
        }
        return string;
    }
    for(int i=0;i<a-length;i++)
    {
        string.push_front(" ");
    }
    return string;
}

template <typename T>T GetMax(T a,T b)
{
    return (a>b)? a : b;
}

template <typename T>T GetMax(QVector <T> a)
{
    int n = a.count();
    if(n == 1)
    {
        return a.at(0);
    }
    T ans = a.at(0);
    for(int i=1;i<n;i++)
    {
        ans = (ans>a.at(i))? ans : a.at(i);
    }
    return ans;
}

template <typename T>
T GetMin(T a,T b)
{
    return (a<b)? a : b;
}

template <typename T>
T GetMin(QVector <T> a)
{
    int n = a.count();
    if(n == 1)
    {
        return a.at(0);
    }
    T ans = a.at(0);
    for(int i=1;i<n;i++)
    {
        ans = (ans<a.at(i))? ans : a.at(i);
    }
    return ans;
}

bool sameNumber(qreal a,qreal b)
{
    if(a==0&&b==0)
    {
        return true;
    }
    if(fabs(a-b)/(fabs(a)+fabs(b))<0.001)
    {
        return true;
    }
    return false;
}

bool inUnit(QRectF _unit,QPointF _pointA,QPointF _pointB)
{
    int xx1,yy1,xx2,yy2,x1,x2,y1,y2;
    if(_unit.width()<0)
    {
        xx1 = _unit.x() + _unit.width();
        xx2 = _unit.x();
    }
    else
    {
        xx2 = _unit.x() + _unit.width();
        xx1 = _unit.x();
    }
    if(_unit.height()<0)
    {
        yy1 = _unit.y() + _unit.height();
        yy2 = _unit.y();
    }
    else
    {
        yy2 = _unit.y() + _unit.height();
        yy1 = _unit.y();
    }
    if(_pointA.x()<_pointB.x())
    {
        x1 = _pointA.x();
        x2 = _pointB.x();
    }
    else
    {
        x2 = _pointA.x();
        x1 = _pointB.x();
    }
    if(_pointA.y()<_pointB.y())
    {
        y1 = _pointA.y();
        y2 = _pointB.y();
    }
    else
    {
        y2 = _pointA.y();
        y1 = _pointB.y();
    }
    if(xx2<x1||yy2<y1||xx1>x2||yy1>y2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

qreal log10Rho(qreal _rho)
{
    if(_rho<1e-8)
    {
        return 18;
    }
    else
    {
        return log10(_rho);
    }
}

qreal pow10Rho(qreal _rho)
{
    if(_rho==18)
    {
        return 0;
    }
    else
    {
        return pow(10,_rho);
    }
}

