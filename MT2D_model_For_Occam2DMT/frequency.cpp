#include "frequency.h"
#include "ui_frequency.h"

Frequency::Frequency(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frequency)
{
    ui->setupUi(this);
    model = new QStringListModel(this);
    frequencys.clear();
    frequencys<<0.001<<0.00125893<<0.00158489<<0.00199526<<0.00251189<<0.00316228<<0.00398107<<0.00501187<<0.00630957<<0.00794328
             <<0.01<<0.0125893<<0.0158489<<0.0199526<<0.0251189<<0.0316228<<0.0398107<<0.0501187<<0.0630957<<0.0794328
            <<0.1<<0.125893<<0.158489<<0.199526<<0.251189<<0.316228<<0.398107<<0.501187<<0.630957<<0.794328
           <<1<<1.25893<<1.58489<<1.99526<<2.51189<<3.16228<<3.98107<<5.01187<<6.30958<<7.94328
          <<10<<12.5893<<15.8489<<19.9526<<25.1189<<31.6228<<39.8107<<50.1187<<63.0958<<79.4328
         <<100<<125.893<<158.489<<199.526<<251.189<<316.228<<398.107<<501.187<<630.958<<794.328
        <<1000;
    setFrequency(frequencys);
}

Frequency::~Frequency()
{
    delete ui;
}

void Frequency::setFrequency(QList<double> _frequencys)
{
    frelist.clear();
    frequencys = _frequencys;
    int count = frequencys.count();
    for(int i=0;i<count;i++)
    {
        frelist<<QString::number(frequencys.at(i));
    }
    model->setStringList(frelist);
    ui->listView->setModel(model);
    return;
}

void Frequency::on_pushButton_add_clicked()
{
    int row = ui->listView->currentIndex().row() + 1;
    if(row==0)
    {
        row = model->rowCount();
    }
    model->insertRows(row,1);
    QModelIndex index = model->index(row);
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
    return;
}

void Frequency::on_pushButton_edit_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    QModelIndex index = model->index(row);
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
    return;
}

void Frequency::on_pushButton_del_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    model->removeRows(row,1);
    return;
}

void Frequency::on_pushButton_import_clicked()
{
    frequencys.clear();
    QString filename = QFileDialog::getOpenFileName();
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString line;
    while(!file.atEnd())
    {
        line = file.readLine();
        line.remove(QRegExp("[, \t\n]"));
        frequencys<<line.toDouble();
    }
    setFrequency(frequencys);
    return;
}


void Frequency::on_pushButton_export_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    QFile file(filename);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    frequencys.clear();
    int  count = model->rowCount();
    for(int i =0;i<count;i++)
    {
        QModelIndex index = model->index(i);
        QString temp = index.data().toString();
        frequencys.append(temp.toDouble());
    }
    QString line;
    for(int i=0;i<count;i++)
    {
        line = QString::number(frequencys.at(i)) + "\n";
        file.write(line.toLatin1().data());
    }
    return;
}

void Frequency::on_pushButton_sure_clicked()
{
    frequencys.clear();
    int  count = model->rowCount();
    for(int i =0;i<count;i++)
    {
        QModelIndex index = model->index(i);
        QString temp = index.data().toString();
        frequencys.append(temp.toDouble());
    }
    emit sendFrequency(frequencys);
    this->close();
    return;
}
