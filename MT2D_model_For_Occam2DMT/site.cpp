#include "site.h"
#include "ui_site.h"

Site::Site(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Site)
{
    ui->setupUi(this);
    freModel = new QStandardItemModel(this);
    sitesModel = new QStandardItemModel(this);
    sitesModel->setHorizontalHeaderLabels(QStringList()<<tr("Site Name")<<tr("Off Set"));
    ui->tableView->setModel(sitesModel);
    ui->listView->setModel(freModel);
}

Site::~Site()
{
    delete ui;
}

void Site::refreshSites()
{
    sitesNames.clear();
    offSets.clear();
    int count = sitesModel->rowCount();
    for(int i=0;i<count;i++)
    {
        QModelIndex index = sitesModel->index(i,0);
        sitesNames<<index.data().toString();
        index = sitesModel->index(i,1);
        offSets<<QString(index.data().toString()).toDouble();
    }
    return;
}

void Site::refreshFrequencies()
{
    frequencies.clear();
    int  count = freModel->rowCount();
    for(int i =0;i<count;i++)
    {
        QModelIndex index = freModel->index(i,0);
        QString temp = index.data().toString();
        frequencies.append(temp.toDouble());
    }
    return;
}

void Site::refreshDatas()
{
    int pcount = 0;
    QVector < QVector <DataBlock> > temp;
    if(ui->checkBox_1->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_2->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_3->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_4->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_5->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_6->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_7->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_8->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_9->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_10->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_11->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_12->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_13->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_14->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_15->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_16->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_17->isChecked())
    {
        pcount++;
    }
    if(ui->checkBox_18->isChecked())
    {
        pcount++;
    }
    int ppcount = pcount;
    dataBlocks.resize(sitesNames.count() * frequencies.count() * pcount);
    pcount = 0;
    if(ui->checkBox_1->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 1;
            }
        }
        pcount++;
    }
    if(ui->checkBox_2->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 2;
            }
        }
        pcount++;
    }
    if(ui->checkBox_3->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 3;
            }
        }
        pcount++;
    }
    if(ui->checkBox_4->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 4;
            }
        }
        pcount++;
    }
    if(ui->checkBox_5->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 5;
            }
        }
        pcount++;
    }
    if(ui->checkBox_6->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 6;
            }
        }
        pcount++;
    }
    if(ui->checkBox_7->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 7;
            }
        }
        pcount++;
    }
    if(ui->checkBox_8->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 8;
            }
        }
        pcount++;
    }
    if(ui->checkBox_9->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 9;
            }
        }
        pcount++;
    }
    if(ui->checkBox_10->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 10;
            }
        }
        pcount++;
    }
    if(ui->checkBox_11->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 11;
            }
        }
        pcount++;
    }
    if(ui->checkBox_12->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 12;
            }
        }
        pcount++;
    }
    if(ui->checkBox_13->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 13;
            }
        }
        pcount++;
    }
    if(ui->checkBox_14->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 14;
            }
        }
        pcount++;
    }
    if(ui->checkBox_15->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 15;
            }
        }
        pcount++;
    }
    if(ui->checkBox_16->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 16;
            }
        }
        pcount++;
    }
    if(ui->checkBox_17->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 17;
            }
        }
        pcount++;
    }
    if(ui->checkBox_18->isChecked())
    {
        for(int i=0;i<sitesNames.count();i++)
        {
            for(int j=0;j<frequencies.count();j++)
            {
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].SITE = i + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].FREQ = j + 1;
                dataBlocks[ppcount * ((i * frequencies.count()) + j) + pcount].TYPE = 18;
            }
        }
        pcount++;
    }
}

void Site::on_pushButton_add_site_clicked()
{
    int row = ui->tableView->currentIndex().row() + 1;
    if(row==0)
    {
        row = sitesModel->rowCount();
    }
    sitesModel->insertRow(row);
    QModelIndex index = sitesModel->index(row,0);
    ui->tableView->setCurrentIndex(index);
    ui->tableView->edit(index);
    return;
}

void Site::on_pushButton_del_site_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    sitesModel->removeRow(row);
    return;
}

void Site::on_pushButton_edit_site_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    sitesModel->removeRow(row);
    return;
}

void Site::on_pushButton_sure_site_clicked()
{
    refreshSites();
    emit setSites(sitesNames,offSets);
    return;
}

void Site::on_pushButton_export_site_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Site File"),
                                                    QDir::currentPath(),
                                                    tr("Dat (*.dat *.txt);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    refreshSites();
    qint32 count = sitesNames.count();
    QString line;
    for(int i=0;i<count;i++)
    {
        line = sitesNames[i] + "\t" + QString::number(offSets[i]) +"\n";
        file.write(line.toLatin1().data());
    }
    return;
}

void Site::on_pushButton_import_site_clicked()
{
    sitesNames.clear();
    offSets.clear();
    QString filename = QFileDialog::getOpenFileName(this, tr("Site File"),
                                                    QDir::currentPath(),
                                                    tr("Text (*.txt *.dat);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QString line;
    QStringList linelist;
    while(!file.atEnd())
    {
        line = file.readLine();
        linelist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        sitesNames<<QString(linelist[0]);
        offSets<<QString(linelist[1]).toDouble();
    }
    file.close();
    initSites(sitesNames,offSets);
    return;
}

void Site::on_pushButton_add_Fre_clicked()
{
    int row = ui->listView->currentIndex().row() + 1;
    if(row==0)
    {
        row = freModel->rowCount();
    }
    freModel->insertRows(row,1);
    QModelIndex index = freModel->index(row,0);
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
    return;
}

void Site::on_pushButton_del_Fre_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    freModel->removeRows(row,1);
    return;
}

void Site::on_pushButton_edit_Fre_clicked()
{
    int row = ui->listView->currentIndex().row();
    if(row==-1)
    {
        return;
    }
    QModelIndex index = freModel->index(row,0);
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
    return;
}

void Site::on_pushButton_sure_Fre_clicked()
{
    refreshFrequencies();
    emit setFrequency(frequencies);
    return;
}

void Site::on_pushButton_export_Fre_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Fre File"),
                                                    QDir::currentPath(),
                                                    tr("Dat (*.dat *.txt);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    refreshFrequencies();
    QString line;
    qint32 count = frequencies.count();
    for(int i=0;i<count;i++)
    {
        line = QString::number(frequencies.at(i)) + "\n";
        file.write(line.toLatin1().data());
    }
    return;
}

void Site::on_pushButton_import_Fre_clicked()
{
    frequencies.clear();
    QString filename = QFileDialog::getOpenFileName(this, tr("Fre File"),
                                                    QDir::currentPath(),
                                                    tr("Text (*.txt *dat);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QString line;
    while(!file.atEnd())
    {
        line = file.readLine();
        line.remove(QRegExp("[, \t\n]"));
        frequencies<<line.toDouble();
    }
    file.close();
    initFrequency(frequencies);
    return;
}

void Site::on_pushButton_import_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Data File"),
                                                    QDir::currentPath(),
                                                    tr("dat (*.dat *.txt);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    QStringList sitesNames1;
    QVector < qreal > offSets1;
    QVector < qreal > frequencies1;
    QVector < DataBlock > dataBlocks1;
    QString line;
    line = file.readLine();
    line = file.readLine();
    line = file.readLine();
    line.remove("\n");
    line.remove("SITES:");
    qint32 SITES = line.toInt();
    sitesNames1.clear();
    for(int i=0;i<SITES;i++)
    {
        line = file.readLine();
        line.remove("\n");
        sitesNames1<<line;
    }
    line = file.readLine();
    offSets1.clear();
    for(int i=0;i<SITES;i++)
    {
        line = file.readLine();
        line.remove("\n");
        offSets1<<line.toDouble();
    }
    line = file.readLine();
    line.remove("\n");
    line.remove("FREQUENCIES:");
    qint32 FREQUENCIESCOUNT = line.toInt();
    frequencies1.clear();
    for(int i=0;i<FREQUENCIESCOUNT;i++)
    {
        line = file.readLine();
        line.remove("\n");
        frequencies1<<line.toDouble();
    }
    line = file.readLine();
    qDebug()<<line;
    line.remove("\n");
    line.remove("DATA BLOCKS:");
    qint32 DATABLOCKS = line.toInt();
    line = file.readLine();
    QStringList stringlist;
    dataBlocks1.clear();
    DataBlock tempDataBlock;
    for(int i=0;i<DATABLOCKS;i++)
    {
        line = file.readLine();
        stringlist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        qDebug()<<i<<stringlist;
        tempDataBlock.SITE=QString(stringlist[0]).toInt();
        tempDataBlock.FREQ=QString(stringlist[1]).toInt();
        tempDataBlock.TYPE=QString(stringlist[2]).toInt();
        tempDataBlock.DATUM=QString(stringlist[3]).toDouble();
        tempDataBlock.ERR=QString(stringlist[4]).toDouble();
        dataBlocks1<<tempDataBlock;
    }
    file.close();
    initDataInformation(sitesNames1,offSets1,frequencies1,dataBlocks1);
    return;
}

void Site::on_pushButton_export_clicked()
{
    refreshFrequencies();
    refreshSites();
    refreshDatas();
    qint32 scount = sitesNames.count();
    qint32 fcount = frequencies.count();
    qint32 dcount = dataBlocks.count();
    QString filename = QFileDialog::getSaveFileName(this, tr("Data File"),
                                                    QDir::currentPath(),
                                                    tr("Dat (*.dat);;All (*)"));
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QString text;
    text = "FORMAT:           OCCAM2DMTDATA_1.0\n";
    text += "DESCRIPTION:      Data File Write By Wang\n";
    text += "SITES:            " + QString::number(scount) + "\n";
    for(int i=0;i<scount;i++)
    {
        text += sitesNames[i] + "\n";
    }
    text += "OFFSETS (M):\n";
    for(int i=0;i<scount;i++)
    {
        text += QString::number(offSets[i]) + "\n";
    }
    text += "FREQUENCIES:      " + QString::number(fcount) + "\n";
    for(int i=0;i<fcount;i++)
    {
        text += QString::number(frequencies[i]) + "\n";
    }
    text += "DATA BLOCKS:      " + QString::number(dcount) + "\n";
    text += "SITE FREQ TYPE DATUM  ERR\n";
    for(int i=0;i<dcount;i++)
    {
//        qDebug()<<dataBlocks[i].SITE
//               <<dataBlocks[i].FREQ
//              <<dataBlocks[i].TYPE
//             <<dataBlocks[i].DATUM
//            <<dataBlocks[i].ERR;
        text += Q2F(dataBlocks[i].SITE,3)
                + Q2F(dataBlocks[i].FREQ,4)
                + Q2F(dataBlocks[i].TYPE,4)
                + Q2F(dataBlocks[i].DATUM,"f",8,4)
                + Q2F(dataBlocks[i].ERR,"f",8,4)
                + "\n";
    }
    file.write(text.toLatin1().data());
    file.close();
    return;
}

void Site::on_pushButton_sure_clicked()
{
    refreshFrequencies();
    refreshSites();
    refreshDatas();
    emit setDataInformation(sitesNames,offSets,frequencies,dataBlocks);
    this->hide();
    return;
}

void Site::initFrequency(QVector < qreal > _frequencies)
{
    frequencies.clear();
    frequencies = _frequencies;
    int count = frequencies.count();
    freModel->clear();
    freModel->setRowCount(count);
    for(int i=0;i<count;i++)
    {
        QStandardItem* temp = new QStandardItem(QString::number(frequencies[i]));
        freModel->setItem(i,temp);
    }
    return;
}

void Site::initSites(QStringList _sitenames,QVector < qreal > _offsets)
{
    sitesNames.clear();
    sitesNames = _sitenames;
    offSets.clear();
    offSets = _offsets;
    int count = sitesNames.count();
    sitesModel->clear();
    sitesModel->setHorizontalHeaderLabels(QStringList()<<tr("Site Name")<<tr("Off Set"));
    sitesModel->setRowCount(count);
    sitesModel->setColumnCount(2);
    for(int i=0;i<count;i++)
    {
        QStandardItem* temp1 = new QStandardItem(sitesNames[i]);
        sitesModel->setItem(i,0,temp1);
        QStandardItem* temp2 = new QStandardItem(QString::number(offSets[i]));
        sitesModel->setItem(i,1,temp2);
    }
    return;
}

void Site::initDataBlocks(QVector < DataBlock > _datablocks)
{
    dataBlocks.clear();
    dataBlocks = _datablocks;
    return;
}

void Site::initDataInformation(QStringList _sitenames,
                         QVector < qreal > _offsets,
                         QVector < qreal > _frequencies,
                         QVector < DataBlock > _datablocks)
{
    initFrequency(_frequencies);
    initSites(_sitenames,_offsets);
    initDataBlocks(_datablocks);
    return;
}

void Site::on_pushButton_Reset_clicked()
{
    QVector < qint32 > numbers;
    numbers.clear();
    if(ui->checkBox_1->isChecked())
    {
        numbers<<1;
    }
    if(ui->checkBox_2->isChecked())
    {
        numbers<<2;
    }
    if(ui->checkBox_3->isChecked())
    {
        numbers<<3;
    }
    if(ui->checkBox_4->isChecked())
    {
        numbers<<4;
    }
    if(ui->checkBox_5->isChecked())
    {
        numbers<<5;
    }
    if(ui->checkBox_6->isChecked())
    {
        numbers<<6;
    }
    if(ui->checkBox_7->isChecked())
    {
        numbers<<7;
    }
    if(ui->checkBox_8->isChecked())
    {
        numbers<<8;
    }
    if(ui->checkBox_9->isChecked())
    {
        numbers<<9;
    }
    if(ui->checkBox_10->isChecked())
    {
        numbers<<10;
    }
    if(ui->checkBox_11->isChecked())
    {
        numbers<<11;
    }
    if(ui->checkBox_12->isChecked())
    {
        numbers<<12;
    }
    if(ui->checkBox_13->isChecked())
    {
        numbers<<13;
    }
    if(ui->checkBox_14->isChecked())
    {
        numbers<<14;
    }
    if(ui->checkBox_15->isChecked())
    {
        numbers<<15;
    }
    if(ui->checkBox_16->isChecked())
    {
        numbers<<16;
    }
    if(ui->checkBox_17->isChecked())
    {
        numbers<<17;
    }
    if(ui->checkBox_18->isChecked())
    {
        numbers<<18;
    }
    int dcount = numbers.count();
    if(dcount==0)
    {
        return;
    }
    dataBlocks.clear();
    refreshFrequencies();
    refreshSites();
    int fcount = frequencies.count();
    int scount = sitesNames.count();
    for(int i=0;i<scount;i++)
    {
        for(int j=0;j<fcount;j++)
        {
            for(int k=0;k<dcount;k++)
            {
                DataBlock tempdata;
                tempdata.SITE = i+1;
                tempdata.FREQ = j+1;
                tempdata.TYPE = numbers[k];
                tempdata.DATUM = 0.0;
                tempdata.ERR = 0.0;
                dataBlocks<<tempdata;
            }
        }
    }
    qDebug()<<dataBlocks.count();
    return;
}

void Site::on_pushButton_import_Data_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Data File"),
                                                    QDir::currentPath(),
                                                    tr("Dat (*.dat)"));
    QString lineName = fileName;
    qint32 index = lineName.lastIndexOf("/");
    lineName.remove(0,index+1);
    lineName.chop(4);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }
    dataBlocks.clear();
    refreshFrequencies();
    refreshSites();
    offSets.clear();
    sitesNames.clear();
    frequencies.clear();
    DataBlock tempData;
    QString line;
    QStringList linelist;
    qreal offset,frequency;
    while(!file.atEnd())
    {
        line = file.readLine();
        linelist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        offset = QString(linelist.at(0)).toDouble() * 1000;
        frequency = QString(linelist.at(1)).toDouble();
        if(!offSets.contains(offset))
        {
            offSets<<offset;
        }
        if(!frequencies.contains(frequency))
        {
            frequencies<<frequency;
        }
        tempData.SITE = offSets.indexOf(offset) + 1;
        tempData.FREQ = frequencies.indexOf(frequency) + 1;
        tempData.TYPE = 1;
        tempData.DATUM = log10(QString(linelist.at(2)).toDouble());
        tempData.ERR = QString(linelist.at(4)).toDouble();
        dataBlocks<<tempData;
        tempData.TYPE = 2;
        tempData.DATUM = QString(linelist.at(3)).toDouble();
        tempData.ERR = QString(linelist.at(5)).toDouble();
        dataBlocks<<tempData;
        tempData.TYPE = 5;
        tempData.DATUM = log10(QString(linelist.at(6)).toDouble());
        tempData.ERR = QString(linelist.at(8)).toDouble();
        dataBlocks<<tempData;
        tempData.TYPE = 6;
        tempData.DATUM = QString(linelist.at(7)).toDouble();
        tempData.ERR = QString(linelist.at(8)).toDouble();
        dataBlocks<<tempData;
    }
    file.close();
    qreal offsetMax = offSets.at(0);
    qreal offsetMin = offSets.at(0);
    for(int i=1;i<offSets.count();i++)
    {
        if(offsetMax<offSets.at(i))
        {
            offsetMax = offSets.at(i);
        }
        if(offsetMin>offSets.at(i))
        {
            offsetMin = offSets.at(i);
        }
    }
    qreal offsetOffset = (offsetMax + offsetMin)/2;
    for(int i=0;i<offSets.count();i++)
    {
        sitesNames<<lineName + QString::number(i+1);
        offSets[i] = offSets[i] - offsetOffset;
    }
    initDataInformation(sitesNames,offSets,frequencies,dataBlocks);
}
