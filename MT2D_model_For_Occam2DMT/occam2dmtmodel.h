#ifndef OCCAM2DMTMODEL_H
#define OCCAM2DMTMODEL_H

#include <QWidget>

#include "colorscale.h"
#include "expand.h"
#include "frequency.h"
#include "functions.h"
#include "intercal.h"
#include "paint.h"
#include "newmodel.h"
#include "unit.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QString>
#include <QStringList>
#include <QKeyEvent>
#include <QProcess>

namespace Ui {
class Occam2DMTModel;
}

class Occam2DMTModel : public QWidget
{
    Q_OBJECT

public:
    explicit Occam2DMTModel(QWidget *parent = 0);
    ~Occam2DMTModel();

private:
    Ui::Occam2DMTModel *ui;
    paint* modelpaint;
    ColorScale* colorscale;
    ColorMap colorMap;
    Expand* expand;
    Frequency* frequency;
    QVector < QVector< unit > > model;
    QRectF boundary;
    QVector < point > points;
    QVector < line > lines;
    QVector < rectangle > rectangles;
    QString status;
    double currentValue;
    double valueMin;
    double valueMax;
    double backgroundResistivity;
    QVector < double > frequencys;
    QVector < QPointF > viewpoint;
    QVector < QPointF > topographypoint;
    NewModel *newmodel;
    qint32 zcount,ycount,nodescount,unitscount;

protected:
    void paintModel();

private:
    bool YZcount();
    void addRow(QPointF _point);
    void delRow(QPointF _point);
    void addCol(QPointF _point);
    void delCol(QPointF _point);
    void setValue(QPointF _point);


public slots:
    void setModel(QVector < QVector< unit > > _model,QRectF _boundary,double _backgroundResistivity);
    void initModelFromOccam(QVector < QVector < unit > > _model);
    void sendPoint(QPointF _point);
    void sendPoint(QPointF _pointA,QPointF _pointB);
    void setColorValue(double _value);
    void expandModel(QVector < QVector< unit > > _model);
    void sendFrequency(QList < double > _frequencys);

signals:
    void setColorMap(ColorMap _colorMap);
    void setCurrentValue(double _value);
    void initialModel(QVector < QVector< unit > > _model);
    void setFrequency(QList < double > _frequencys);
    void setBoundary(QRectF _rect);
    void setdatas(QVector < point > _points,QVector < line > _lines,QVector < rectangle > _rects);
    void setModelFromModeling(QVector < QVector < unit > > _model);

private slots:
    void on_pushButton_newModel_clicked();
    void on_pushButton_addrow_clicked();
    void on_pushButton_delrow_clicked();
    void on_pushButton_addcol_clicked();
    void on_pushButton_delcol_clicked();
    void on_pushButton_value_clicked();
    void on_lineEdit_value_editingFinished();
    void on_pushButton_expand_clicked();
    void on_pushButton_sure_clicked();
    void on_pushButton_colorScale_clicked();
    void on_pushButton_RhoRange_clicked();
    void on_pushButton_export_clicked();
    void on_pushButton_import_clicked();
    void on_pushButton_importViewPoints_clicked();
};

#endif // OCCAM2DMTMODEL_H
