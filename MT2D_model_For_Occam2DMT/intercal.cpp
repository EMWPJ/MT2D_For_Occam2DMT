#include "intercal.h"

QVector <QPointF> sortPoint(QVector <QPointF> data)
{
    QVector <QPointF> temp;
    temp.clear();
    if(data.count()<=0)
    {
        return temp;
    }
    QList < double > Ys;
    Ys.clear();

    for(int i=0;i<data.count();i++)
    {
        Ys << data[i].x();
    }
    qSort(Ys.begin(),Ys.end());

    for(int i=0;i<Ys.count();i++)
    {
        for(int m=0;m<data.count();m++)
        {
            if(Ys[i]==data[m].x())
            {
                temp << data[m];
                data.remove(m);
            }
        }
    }
    return temp;
}

QVector <double> sortDouble(QVector <double> data)
{
    QVector <double> temp;
    temp.clear();
    if(data.count()<=0)
    {
        return temp;
    }

    QList < double > Ys;
    Ys.clear();

    for(int i=0;i<data.count();i++)
    {
        Ys << data[i];
    }

    qSort(Ys.begin(),Ys.end());

    for(int i=0;i<Ys.count();i++)
    {
        temp << Ys.at(i);
    }

    return temp;
}
QVector<QPointF> secDerv(QVector<QPointF> info)//二阶求导
{
    QVector<QPointF> sp;
    int n = info.count();
    if(n<=0)
    {
        return sp;
    }
    QVector < double > x,y,y2;
    x.resize(n);
    y.resize(n);
    y2.resize(n);
    for(int i=0;i<n;i++)
    {
        x[i] = info[i].x();
        y[i] = info[i].y();
    }

    y2[0] = 0;

    double u[5000];
    u[0] = 0;

    for(int i=1;i<n-2;i++)
    {
        double sig = (x[i]-x[i-1])/(x[i+1]-x[i-1]);
        double p = sig*y2[i-1]+2;
        y2[i] = (sig-1.0)/p;
        //u(i)=(6.*((y(i+1)-y(i))/(x(i+1)-x(i))-(y(i)-y(i-1))/(x(i)-x(i-1)))/(x(i+1)-x(i-1))-sig*u(i-1))/p
        u[i] = (6.0*((y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/(x[i]-x[i-1]))/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }

    double qn = 0;
    double un = 0;
    y2[n-1] = (un-qn*u[n-2])/(qn*y2[n-2]+1.0);

    for(int i=n-2;i>=0;i--)
    {
        y2[i] = y2[i]*y2[i+1]+u[i];
    }

    for(int i=0;i<n;i++)
    {
        QPointF temp;
        temp.setY(y2[i]);
        temp.setX(info[i].x());
        sp << temp;
    }

    return sp;
}

double splint(QVector<QPointF> org,QVector<QPointF> sp,double Y)
{
    double y = 0;
    int n = org.count();
    if(n<=0)
    {
        return y;
    }

    QVector < double > xa,ya,y2a;
    xa.resize(n);
    ya.resize(n);
    y2a.resize(n);


    for(int i=0;i<n;i++)
    {
        xa[i] = org[i].x();
        ya[i] = org[i].y();
        y2a[i] = sp[i].y();
    }

    int klo = 1;
    int khi = n;

    while(1)
    {
        double aaa = 0;
        if(khi-klo>1)
        {
            aaa = 2.0;
            int k = (khi+klo)/2;
            if(xa[k-1]>Y)
            {
                khi = k;
            }
            else
            {
                klo = k;
            }
            if(aaa<=1)
            {
                break;
            }
        }
        if(aaa<=1)
        {
            break;
        }
    }

    double h = xa[khi-1]-xa[klo-1];

    if(h==0)
    {
        return y;
    }

    double a = (xa[khi-1]-Y)/h;
    double b = (Y-xa[klo-1])/h;

    y = a*ya[klo-1]+b*ya[khi-1]+((pow(a,3)-a)*y2a[klo-1]+(pow(b,3)-b)*y2a[khi-1])*(pow(h,2))/6;

    return y;
}

QVector<QPointF> spline(QVector<QPointF> info, QVector<double> Ys)
{
    QVector< QPointF > Zs;
    Zs.clear();
    if(info.count()<=0)
    {
        return Zs;
    }
    if(Ys.count()<=0)
    {
        return Zs;
    }

    info = sortPoint(info);
    Ys = sortDouble(Ys);
    QVector< QPointF > buff = secDerv(info);
    for(int i=0;i<Ys.count();i++)
    {
        QPointF temp;
        if(Ys[i]<info[0].x())
        {
            temp.setX(Ys[i]);
            temp.setY(info[0].y());
        }
        else if(Ys[i]>info[info.count()-1].x())
        {
            temp.setX(Ys[i]);
            temp.setY(info[info.count()-1].y());
        }
        else
        {
            temp.setX(Ys[i]);
            temp.setY(splint(info,buff,Ys.at(i)));
        }
        Zs << temp;
//        qDebug()<<"temp"<<i<<temp.x()<<temp.y();
    }
    return Zs;
}
