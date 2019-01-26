#ifndef SITE_H
#define SITE_H

#include <QWidget>
#include <QVector>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include "functions.h"


typedef struct
{
    qint32 SITE;
    qint32 FREQ;
    qint32 TYPE;
    qreal DATUM;
    qreal ERR;
}DataBlock;

namespace Ui {
class Site;
}

class Site : public QWidget
{
    Q_OBJECT

public:
    explicit Site(QWidget *parent = 0);
    ~Site();

private slots:

    void on_pushButton_add_site_clicked();

    void on_pushButton_del_site_clicked();

    void on_pushButton_edit_site_clicked();

    void on_pushButton_sure_site_clicked();

    void on_pushButton_export_site_clicked();

    void on_pushButton_import_site_clicked();

    void on_pushButton_add_Fre_clicked();

    void on_pushButton_del_Fre_clicked();

    void on_pushButton_edit_Fre_clicked();

    void on_pushButton_sure_Fre_clicked();

    void on_pushButton_export_Fre_clicked();

    void on_pushButton_import_Fre_clicked();

    void on_pushButton_import_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_sure_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_import_Data_clicked();

private:
    Ui::Site *ui;
    QStringList sitesNames;
    QVector < qreal > offSets;
    QVector < qreal > frequencies;
    QVector < DataBlock > dataBlocks;
    QStandardItemModel* sitesModel;
    QStandardItemModel* freModel;

signals:
    void setFrequency(QVector < qreal > _frequencies);
    void setSites(QStringList _sitenames,QVector < qreal > _offsets);
    void setDataBlocks(QVector < DataBlock > _datablocks);
    void setDataInformation(QStringList _sitenames,
                            QVector < qreal > _offsets,
                            QVector < qreal > _frequencies,
                            QVector < DataBlock > _datablocks);

public slots:
    void initFrequency(QVector < qreal > _frequencies);
    void initSites(QStringList _sitenames,QVector < qreal > _offsets);
    void initDataBlocks(QVector < DataBlock > _datablocks);
    void initDataInformation(QStringList _sitenames,
                             QVector < qreal > _offsets,
                             QVector < qreal > _frequencies,
                             QVector < DataBlock > _datablocks);

private:
    void refreshSites();
    void refreshFrequencies();
    void refreshDatas();

};

#endif // SITE_H
