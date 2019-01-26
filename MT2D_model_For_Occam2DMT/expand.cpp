#include "Expand.h"
#include "ui_Expand.h"

Expand::Expand(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Expand)
{
    ui->setupUi(this);
}

Expand::~Expand()
{
    delete ui;
}

void Expand::on_pushButton_sure_clicked()
{
    double dz = ui->lineEdit_dz->text().toDouble();
    double dy = ui->lineEdit_dy->text().toDouble();
    int zadd = ui->lineEdit_zadd->text().toInt();
    int yadd = ui->lineEdit_yadd->text().toInt();
    int zcount = model.count();
    int ycount = model[0].count();
    QVector< unit > add;
    qreal width,height;
    //向上扩展
//    for(int i=0;i<zadd;i++)
//    {
//        add.clear();
//        for(int j=0;j<ycount;j++)
//        {
//            unit temp = model.at(0).at(j);
//            height = temp.height();
//            temp.setY(temp.y() - temp.height());
//            temp.setHeight(height * dz);
//            add<<temp;
//        }
//        model.push_front(add);
//    }
    //向下扩展
    for(int i=0;i<zadd;i++)
    {
        add.clear();
        for(int j=0;j<ycount;j++)
        {
            unit temp = model.last().at(j);
            height = temp.height();
            temp.setY(temp.y() + temp.height());
            temp.setHeight(height * dz);
            add<<temp;
        }
        model.push_back(add);
    }
    //向左扩展
    zcount = model.count();
    for(int i=0;i<zcount;i++)
    {
        for(int j=0;j<yadd;j++)
        {
            unit temp = model.at(i).at(0);
            width = temp.width();
            temp.setX(temp.x() - temp.width());
            temp.setWidth(width * dy);
            model[i].push_front(temp);
        }
    }
    //向右扩展
    zcount = model.count();
    for(int i=0;i<zcount;i++)
    {
        for(int j=0;j<yadd;j++)
        {
            unit temp = model.at(i).last();
            width = temp.width();
            temp.setX(temp.x() + temp.width());
            temp.setWidth(width * dy);
            model[i].push_back(temp);
        }
    }
    emit expandModel(model);
    this->close();
}

void Expand::initialModel(QVector < QVector< unit > > _model)
{
    model = _model;
}
