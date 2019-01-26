#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <QWidget>
#include <QVector>
#include <QStringListModel>
#include <QList>
#include <QListView>
#include <QFile>
#include <QFileDialog>

namespace Ui {
class Frequency;
}

class Frequency : public QWidget
{
    Q_OBJECT

public:
    explicit Frequency(QWidget *parent = 0);
    ~Frequency();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_import_clicked();

    void on_pushButton_sure_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::Frequency *ui;
    QList < double > frequencys;
    QStringList frelist;
    QStringListModel *model;

signals:
    void sendFrequency(QList < double > _frequencys);

public slots:
    void setFrequency(QList < double > _frequencys);
};

#endif // FREQUENCY_H
