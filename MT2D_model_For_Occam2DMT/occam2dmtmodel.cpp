#include "occam2dmtmodel.h"
#include "ui_occam2dmtmodel.h"

Occam2DMTModel::Occam2DMTModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Occam2DMTModel)
{
    ui->setupUi(this);
    modelpaint = new paint();
    connect(this,SIGNAL(setBoundary(QRectF)),
            modelpaint,SLOT(setBoundary(QRectF)));
    connect(this,SIGNAL(setdatas(QVector<point>,QVector<line>,QVector<rectangle>)),
            modelpaint,SLOT(setdatas(QVector<point>,QVector<line>,QVector<rectangle>)));
    connect(modelpaint,SIGNAL(sendPoint(QPointF)),
            this,SLOT(sendPoint(QPointF)));
    connect(modelpaint,SIGNAL(sendPoint(QPointF,QPointF)),
            this,SLOT(sendPoint(QPointF,QPointF)));
    ui->stackedWidget_paint->addWidget(modelpaint);
    ui->stackedWidget_paint->setCurrentWidget(modelpaint);
    colorscale = new ColorScale();
    connect(this,SIGNAL(setColorMap(ColorMap)),
            colorscale,SLOT(setColorMap(ColorMap)));
    connect(colorscale,SIGNAL(setColorValue(double)),
            this,SLOT(setColorValue(double)));
    connect(this,SIGNAL(setCurrentValue(double)),
            colorscale,SLOT(setCurrentValue(double)));
    ui->stackedWidget_color->addWidget(colorscale);
    ui->stackedWidget_color->setCurrentWidget(colorscale);
    expand = new Expand();
    connect(this,SIGNAL(initialModel(QVector<QVector<unit> >)),
            expand,SLOT(initialModel(QVector<QVector<unit> >)));
    connect(expand,SIGNAL(expandModel(QVector<QVector<unit> >)),
            this,SLOT(expandModel(QVector<QVector<unit> >)));
    frequency = new Frequency();
    connect(this,SIGNAL(setFrequency(QList<double>)),
            frequency,SLOT(setFrequency(QList<double>)));
    connect(frequency,SIGNAL(sendFrequency(QList<double>)),
            this,SLOT(sendFrequency(QList<double>)));
    newmodel = new NewModel();
    connect(newmodel,SIGNAL(setModel(QVector<QVector<unit> >,QRectF,double)),
            this,SLOT(setModel(QVector<QVector<unit> >,QRectF,double)));
    points.clear();
    lines.clear();
    rectangles.clear();
    status.clear();
    colorMap.value.clear();
    colorMap.red.clear();
    colorMap.green.clear();
    colorMap.blue.clear();
    colorMap.alpha.clear();
    colorMap.value<<0<<20<<40<<60<<80<<100;
    colorMap.red<<153<<0<<0<<255<<255<<255;
    colorMap.green<<102<<0<<255<<255<<102<<0;
    colorMap.blue<<255<<255<<0<<0<<0<<0;
    colorMap.alpha<<255<<255<<255<<255<<255<<255;
    emit setColorMap(colorMap);
    currentValue = 2;
    valueMin = -4;
    valueMax = 8;
    frequencys.clear();
    frequencys<<0.001<<0.00125893<<0.00158489<<0.00199526<<0.00251189<<0.00316228<<0.00398107<<0.00501187<<0.00630957<<0.00794328
             <<0.01<<0.0125893<<0.0158489<<0.0199526<<0.0251189<<0.0316228<<0.0398107<<0.0501187<<0.0630957<<0.0794328
            <<0.1<<0.125893<<0.158489<<0.199526<<0.251189<<0.316228<<0.398107<<0.501187<<0.630957<<0.794328
           <<1<<1.25893<<1.58489<<1.99526<<2.51189<<3.16228<<3.98107<<5.01187<<6.30958<<7.94328
          <<10<<12.5893<<15.8489<<19.9526<<25.1189<<31.6228<<39.8107<<50.1187<<63.0958<<79.4328
         <<100<<125.893<<158.489<<199.526<<251.189<<316.228<<398.107<<501.187<<630.958<<794.328
        <<1000;
    viewpoint.clear();
}

Occam2DMTModel::~Occam2DMTModel()
{
    delete ui;
}

void Occam2DMTModel::on_pushButton_newModel_clicked()
{
    newmodel->show();
    status.clear();
    return;
}

void Occam2DMTModel::on_pushButton_addrow_clicked()
{
    status = "AddRow";
    return;
}

void Occam2DMTModel::on_pushButton_delrow_clicked()
{
    status = "DelRow";
    return;
}

void Occam2DMTModel::on_pushButton_addcol_clicked()
{
    status = "AddCol";
    return;
}

void Occam2DMTModel::on_pushButton_delcol_clicked()
{
    status = "DelCol";
    return;
}

void Occam2DMTModel::on_pushButton_value_clicked()
{
    status = "SetValue";
    return;
}

void Occam2DMTModel::setModel(QVector<QVector<unit> > _model, QRectF _boundary, double _backgroundResistivity)
{
    model.clear();
    model = _model;
    boundary = _boundary;
    backgroundResistivity = _backgroundResistivity;
    emit setBoundary(boundary);
    paintModel();
    return;
}

void Occam2DMTModel::paintModel()
{
    if(!YZcount())
    {
        return;
    }
    rectangles.clear();
    rectangle rectangles1;
    for(int i=0;i<zcount;i++)
    {
        for(int j=0;j<ycount;j++)
        {
            QRectF tempRect(model[i][j].x(),model[i][j].y(),model[i][j].width(),model[i][j].height());
            rectangles1.data.clear();
            rectangles1.data.push_back(tempRect);
            rectangles1.pen = QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            QColor color;
            if(sameNumber(model[i][j].value,AirRho))
            {
                color = QColor(240,255,255,255);
            }
            else
            {
                double value;
                value = (model[i][j].value-valueMin)/(valueMax-valueMin)*100;
                if(value<=0)
                {
                    color = QColor(colorMap.red.first(),colorMap.green.first(),colorMap.blue.first(),colorMap.alpha.first());
                }
                else if(value>=100)
                {
                    color = QColor(colorMap.red.last(),colorMap.green.last(),colorMap.blue.last(),colorMap.alpha.last());
                }
                else
                {
                    int num = -1;
                    for(int j=0;j<colorMap.value.count();j++)
                    {
                        if(value>colorMap.value.at(j)&&value<colorMap.value.at(j+1))
                        {
                            num = j;
                            break;
                        }
                    }
                    double d1 = (value-colorMap.value.at(num))/(colorMap.value.at(num+1)-colorMap.value.at(num));
                    if(d1<0)
                    {
                        d1 = 0;
                    }
                    if(d1>1)
                    {
                        d1 = 1;
                    }
                    double d2 = 1 - d1;
                    qint32 r = (int)(colorMap.red.at(num)*d2+colorMap.red.at(num+1)*d1);
                    qint32 g = (int)(colorMap.green.at(num)*d2+colorMap.green.at(num+1)*d1);
                    qint32 b = (int)(colorMap.blue.at(num)*d2+colorMap.blue.at(num+1)*d1);
                    qint32 a = (int)(colorMap.alpha.at(num)*d2+colorMap.alpha.at(num+1)*d1);
                    if(r<0)
                    {
                        r = 0;
                    }
                    else if(r>255)
                    {
                        r = 255;
                    }
                    if(g<0)
                    {
                        g = 0;
                    }
                    else if(g>255)
                    {
                        g = 255;
                    }
                    if(b<0)
                    {
                        b = 0;
                    }
                    else if(b>255)
                    {
                        b = 255;
                    }
                    if(a<0)
                    {
                        a = 0;
                    }
                    else if(a>255)
                    {
                        a = 255;
                    }
                    color = QColor(r,g,b,a);
                }
            }
            rectangles1.brush = QBrush(color,Qt::SolidPattern);
            rectangles<<rectangles1;
        }
    }
    //    qDebug()<<"rectangles"<<rectangles.count();
    emit setdatas(points,lines,rectangles);
    return;
}

void Occam2DMTModel::sendPoint(QPointF _point)
{
    if(status.isEmpty())
    {
        return;
    }
    else if(status == "AddRow")
    {
        addRow(_point);
        return;
    }
    else if(status == "DelRow")
    {
        delRow(_point);
        return;
    }
    else if(status == "AddCol")
    {
        addCol(_point);
        return;
    }
    else if(status == "DelCol")
    {
        delCol(_point);
        return;
    }
    else if(status == "SetValue")
    {
        setValue(_point);
        return;
    }
    else
    {
        return;
    }
    return;
}

void Occam2DMTModel::sendPoint(QPointF _pointA, QPointF _pointB)
{
    if(!YZcount())
    {
        return;
    }
    if(status == "SetValue")
    {
        for(int i=0;i<zcount;i++)
        {
            for(int j=0;j<ycount;j++)
            {
                if(inUnit(QRectF(model[i][j].x(),model[i][j].y(),model[i][j].width(),model[i][j].height()),_pointA,_pointB))
                {
                    model[i][j].value = currentValue;
                }
            }
        }
    }
    paintModel();
    return;
}

void Occam2DMTModel::addRow(QPointF _point)
{
    if(!YZcount())
    {
        return;
    }
    int num = -1;
    for(int i=0;i<zcount;i++)
    {
        unit temp = model[i][0];
        if((temp.y()-_point.y())*(temp.y()+temp.height()-_point.y())<0)
        {
            num = i;
            break;
        }
    }
    if(num == -1)
    {
        return;
    }
    QVector < unit > temp1;
    temp1.clear();
    for(int i=0;i<ycount;i++)
    {
        unit temp = model[num][i];
        temp.setY(temp.y()+temp.height()/2);
        temp.setHeight(model[num][i].height()/2);
        temp1<<temp;
        model[num][i].setHeight(model[num][i].height()/2);
    }
    model.insert(num+1,temp1);
    paintModel();
    return;
}

void Occam2DMTModel::delRow(QPointF _point)
{
    if(!YZcount())
    {
        return;
    }
    int num = 1;
    double distance = fabs(_point.y()-model[0][1].y());
    for(int i=2;i<zcount-1;i++)
    {
        unit temp = model[i][0];
        if(distance>fabs(_point.y()-temp.y()))
        {
            num = i;
            distance = fabs(_point.y()-temp.y());
        }
    }
    for(int i=0;i<ycount;i++)
    {
        model[num-1][i].setHeight(model[num-1][i].height()+model[num][i].height());
    }
    model.removeAt(num);
    paintModel();
    return;
}

void Occam2DMTModel::addCol(QPointF _point)
{
    if(!YZcount())
    {
        return;
    }
    int num = -1;
    for(int i=0;i<ycount;i++)
    {
        unit temp = model[0][i];
        if((temp.x()-_point.x())*(temp.x()+temp.width()-_point.x())<0)
        {
            num = i;
            break;
        }
    }
    if(num == -1)
    {
        return;
    }
    for(int i=0;i<zcount;i++)
    {
        unit temp = model[i][num];
        temp.setX(temp.x()+temp.width()/2);
        temp.setWidth(model[i][num].width()/2);
        model[i][num].setWidth(model[i][num].width()/2);
        model[i].insert(num+1,temp);
    }
    paintModel();
    return;
}

void Occam2DMTModel::delCol(QPointF _point)
{
    if(!YZcount())
    {
        return;
    }
    int num = 1;
    double distance = fabs(_point.x()-model[0][0].x());
    for(int i=2;i<ycount-1;i++)
    {
        unit temp = model[0][i];
        if(distance>fabs(_point.x()-temp.x()))
        {
            num = i;
            distance = fabs(_point.x()-temp.x());
        }
    }
    for(int i=0;i<zcount;i++)
    {
        model[i][num-1].setWidth(model[i][num-1].width()+model[i][num].width());
        model[i].removeAt(num);
    }
    paintModel();
    return;
}

void Occam2DMTModel::setValue(QPointF _point)
{
    if(!YZcount())
    {
        return;
    }
    int num = -1;
    int num1 = -1;
    for(int i=0;i<zcount;i++)
    {
        unit temp = model[i][0];
        if((temp.y()-_point.y())*(temp.y()+temp.height()-_point.y())<0)
        {
            num = i;
            break;
        }
    }
    for(int i=0;i<ycount;i++)
    {
        unit temp = model[0][i];
        if((temp.x()-_point.x())*(temp.x()+temp.width()-_point.x())<0)
        {
            num1 = i;
            break;
        }
    }
    if(num == -1)
    {
        return;
    }
    if(num1 == -1)
    {
        return;
    }
    model[num][num1].value = currentValue;
    paintModel();
    return;
}

bool Occam2DMTModel::YZcount()
{
    zcount = model.count();
    if(zcount<=0)
    {
        qDebug()<<"zcount"<<zcount;
        return false;
    }
    ycount = model[0].count();
    if(ycount<=0)
    {
        qDebug()<<"ycount"<<ycount;
        return false;
    }
    nodescount = (ycount+1)*(zcount+1);
    unitscount = ycount*zcount;
    return true;
}

void Occam2DMTModel::setColorValue(double _value)
{
    currentValue = _value/100*(valueMax-valueMin)+valueMin;
    ui->lineEdit_value->setText(QString::number(currentValue));
    return;
}

void Occam2DMTModel::expandModel(QVector<QVector<unit> > _model)
{
    model.clear();
    model = _model;
    paintModel();
    return;
}

void Occam2DMTModel::sendFrequency(QList<double> _frequencys)
{
    frequencys.clear();
    for(int i=0;i<_frequencys.count();i++)
    {
        frequencys.append(_frequencys[i]);
    }
    return;
}

void Occam2DMTModel::on_lineEdit_value_editingFinished()
{
    currentValue = ui->lineEdit_value->text().toDouble();
    emit setCurrentValue((currentValue - valueMin)/(valueMax - valueMin) * 100);
    return;
}

void Occam2DMTModel::on_pushButton_expand_clicked()
{
    emit initialModel(model);
    expand->show();
    return;
}

void Occam2DMTModel::initModelFromOccam(QVector < QVector < unit > > _model)
{
    model = _model;
    boundary = QRectF(-10000,1000,20000,-21000);
    emit setBoundary(boundary);
    paintModel();
    return;
}

void Occam2DMTModel::on_pushButton_sure_clicked()
{
    emit setModelFromModeling(model);
    this->hide();
}

void Occam2DMTModel::on_pushButton_colorScale_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open clr File"),
                                                    "C:/Program Files/Golden Software/Surfer 13/ColorScales",
                                                    tr("ColorScales (*.clr)"));
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString line;
    QStringList linelist;
    line = file.readLine();
    colorMap.value.clear();
    colorMap.red.clear();
    colorMap.green.clear();
    colorMap.blue.clear();
    colorMap.alpha.clear();
    while(!file.atEnd())
    {
        line = file.readLine();
        linelist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        line = linelist.at(0);
        colorMap.value<<line.toDouble();
        line = linelist.at(1);
        colorMap.red<<line.toInt();
        line = linelist.at(2);
        colorMap.green<<line.toInt();
        line = linelist.at(3);
        colorMap.blue<<line.toInt();
        if(linelist.count()<5)
        {
            colorMap.alpha<<255;
        }
        else
        {
            line = linelist.at(4);
            colorMap.alpha<<line.toInt();
        }
    }
    file.close();
    emit setColorMap(colorMap);
    paintModel();
    return;
}

void Occam2DMTModel::on_pushButton_RhoRange_clicked()
{
    bool ok;
    qreal rhoMax = QInputDialog::getDouble(this, tr("Set RhoMax(log10)"),
                                           tr("RhoMax:"), 4, -10, 10, 1, &ok);
    if(ok)
    {
        valueMax = rhoMax;
    }
    qreal rhoMin = QInputDialog::getDouble(this, tr("Set RhoMin(log10)"),
                                           tr("RhoMin:"), 0, -10, 10, 1, &ok);
    if(ok)
    {
        valueMin = rhoMin;
    }
    paintModel();
    return;
}

void Occam2DMTModel::on_pushButton_export_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Model File"),
                                                    QDir::currentPath(),
                                                    tr("Model (*.mod)"));
    QFile file(fileName);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.close();
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    qint32 rowCount = model.count();
    if(rowCount<=0)
    {
        file.close();
        return;
    }
    qint32 colCount = model[0].count();
    if(colCount<=0)
    {
        file.close();
        return;
    }
    QString string = Q2F(rowCount + 1,10) + Q2F(colCount + 1,10) + tr("\n");
    for(int i=0;i<rowCount;i++)
    {
        string += Q2F(model[i][0].y(),"f",20,5);
        if(i%10==9)
        {
            string += tr("\n");
        }
    }
    string += Q2F(model.last().at(0).y()+model.last().at(0).height(),"f",20,5) + tr("\n");
    for(int i=0;i<colCount;i++)
    {
        string += Q2F(model[0][i].x(),"f",20,5);
        if(i%10==9)
        {
            string += tr("\n");
        }
    }
    string += Q2F(model.at(0).last().x()+model.at(0).last().width(),"f",20,5) + tr("\n");
    for(int i=0;i<rowCount;i++)
    {
        for(int j=0;j<colCount;j++)
        {
            string += Q2F(model[i][j].value,"f",6,2);
        }
        string += tr("\n");
    }
    file.write(string.toLatin1().data());
    file.close();
    return;
}

void Occam2DMTModel::on_pushButton_import_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Model File"),
                                                    QDir::currentPath(),
                                                    tr("Model (*.mod)"));

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QString string = file.readAll();
    file.close();
    QStringList stringList = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
    int index = 0;
    string = stringList.at(index);
    index++;
    qint32 rowCount = string.toInt() - 1;
    string = stringList.at(index);
    index++;
    qint32 colCount = string.toInt() - 1;
    QVector < qreal > y,z;
    y.clear();
    z.clear();
    for(int i=0;i<rowCount+1;i++)
    {
        string = stringList.at(index);
        index++;
        z<<string.toDouble();
    }
    for(int i=0;i<colCount+1;i++)
    {
        string = stringList.at(index);
        index++;
        y<<string.toDouble();
    }
    model.clear();
    QVector < unit > tempUnits;
    unit tempUnit;
    for(int i=0;i<rowCount;i++)
    {
        tempUnits.clear();
        for(int j=0;j<colCount;j++)
        {
            string = stringList.at(index);
            index++;
            tempUnit.value = string.toDouble();
            tempUnit.setRect(y[j],z[i],y[j+1]-y[j],z[i+1]-z[i]);
            tempUnits<<tempUnit;
        }
        model<<tempUnits;
    }
    paintModel();
    return;
}

void Occam2DMTModel::on_pushButton_importViewPoints_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Import ViewPoint File"),
                                                      QDir::currentPath(),
                                                      tr("point (*.txt)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QString string = file.readLine();
    QStringList stringlist;
    point temp;
    temp.data.clear();
    temp.pen = QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPointF temppoint;
    viewpoint.clear();
    while(!file.atEnd())
    {
        string = file.readLine();
        stringlist = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        string = stringlist.at(0);
        temppoint.setX(string.toDouble());
        string = stringlist.at(1);
        temppoint.setY(string.toDouble());
        temp.data.push_back(temppoint);
        viewpoint<<temppoint;
    }
    temp.data = sortPoint(temp.data);
    file.close();
    points.clear();
    points<<temp;
    paintModel();
    return;
}
