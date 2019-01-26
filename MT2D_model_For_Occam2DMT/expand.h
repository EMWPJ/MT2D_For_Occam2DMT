#ifndef Expand_H
#define Expand_H

#include <QWidget>
#include <QDebug>
#include <QVector>

#include "unit.h"

namespace Ui {
class Expand;
}

class Expand : public QWidget
{
    Q_OBJECT

public:
    explicit Expand(QWidget *parent = 0);
    ~Expand();

private slots:
    void on_pushButton_sure_clicked();

private:
    Ui::Expand *ui;
    QVector < QVector< unit > > model;

public slots:
    void initialModel(QVector < QVector< unit > > _model);

signals:
    void expandModel(QVector < QVector< unit > > model);
};

#endif // Expand_H
