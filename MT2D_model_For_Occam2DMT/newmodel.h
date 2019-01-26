#ifndef NEWMODEL_H
#define NEWMODEL_H

#include <QWidget>
#include <QVector>
#include <QDebug>
#include "unit.h"
#include "functions.h"

namespace Ui {
class NewModel;
}

class NewModel : public QWidget
{
    Q_OBJECT

public:
    explicit NewModel(QWidget *parent = 0);
    ~NewModel();

private:
    Ui::NewModel *ui;

signals:
    void setModel(QVector< QVector < unit > > _model,QRectF _boundary,double _rho);
private slots:
    void on_pushButton_sure_clicked();
};

#endif // NEWMODEL_H
