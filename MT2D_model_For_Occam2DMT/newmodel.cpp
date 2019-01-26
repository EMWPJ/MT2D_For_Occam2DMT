#include "newmodel.h"
#include "ui_newmodel.h"

NewModel::NewModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewModel)
{
    ui->setupUi(this);
}

NewModel::~NewModel()
{
    delete ui;
}


void NewModel::on_pushButton_sure_clicked()
{
    qreal rho = ui->lineEdit_rho->text().toDouble();
    qreal ymin = ui->lineEdit_Ymin->text().toDouble();
    qreal ymax = ui->lineEdit_Ymax->text().toDouble();
    qint32 ycount = ui->lineEdit_Ycount->text().toInt();
    qreal zmin = ui->lineEdit_Zmin->text().toDouble();
    qreal zmax = ui->lineEdit_Zmax->text().toDouble();
    qint32 zcount = ui->lineEdit_Zcount->text().toInt();
    qreal dy = (ymax-ymin)/ycount;
    qreal dz = (zmin-zmax)/zcount;
    QVector < QVector < unit > > model;
    model.clear();
    model.resize(zcount);
    unit temp;
    for(int i=0;i<zcount;i++)
    {
        model[i].clear();
        for(int j=0;j<ycount;j++)
        {
            temp.setX(ymin+dy*j);
            temp.setY(zmax+i*dz);
            if(zmax+i*dz<=0)
            {
                temp.value = rho;
            }
            else
            {
                temp.value = AirRho;
            }
            temp.setWidth(dy);
            temp.setHeight(dz);
            model[i]<<temp;
        }
    }
    emit setModel(model,QRectF(ymin,zmax,ymax-ymin,zmin-zmax),rho);
    this->hide();
}
