#ifndef OCCAMPROJECT_H
#define OCCAMPROJECT_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QTextBrowser>
#include <QTextCursor>
#include <QInputDialog>
#include <QCloseEvent>
#include <QDebug>

#include "occam2dmtmodel.h"
#include "site.h"
#include "unit.h"
#include "functions.h"

typedef struct
{
    qint32 layers;
    qint32 count;
    QVector < int > number;
}Layer;

typedef struct
{
    qreal datum;
    qreal error;
}DataError;

namespace Ui {
class OccamProject;
}

class OccamProject : public QWidget
{
    Q_OBJECT

public:
    explicit OccamProject(QWidget *parent = 0);
    ~OccamProject();

private slots:
    void on_toolButton_filepath_clicked();

    void on_toolButton_ModelFile_clicked();

    void on_toolButton_DateFile_clicked();

    void on_pushButton_SITES_clicked();

    void on_toolButton_DateTime_clicked();

    void on_pushButton_EditModel_clicked();

    void on_pushButton_Forward_clicked();

    void on_pushButton_Inversion_clicked();

    void on_pushButton_SaveProject_clicked();

    void on_pushButton_ExportData_clicked();

private:
    Ui::OccamProject *ui;
    QString projectName;
    QString filePath;
    QString startUpFileName;
    QString dataFileName;
    QString meshFileName;
    QString modelFileName;

    QString forwardOutputFilename;

    Site* site;
    Occam2DMTModel* modeling;
    QTextBrowser* browser;

    QProcess* process;


    //Startup File
    QString Format,Description,DateTime,ModifyRoughness,ModelLimits,ModelValueSteps;
    qint32 IterationsToRun,RoughnessType,DiagonalPenalties,StepsizeCutCount,DebugLevel,Iteration,ParamCount;
    qreal TargetMisfit,LagrangeValue,RoughnessValue,MisfitValue,MisfitReached;
    QVector < qreal > parameters;

    //Model File
    QString MFORMAT,MODELNAME,MDESCRIPTION,MESHTYPE,STATICSFILE,PREJUDICEFILE;
    qreal BINDINGOFFSET;
    qint32 NUMLAYERS,NUMBEREXCEPTIONS;
    QVector < Layer > layers;

    //Mesh File
    QString DescriptionTextOfMeshFile;
    qint32 number1,number2,number3,number4,number5,number6;
    QVector < qreal > FixedResistivity;
    QVector < qreal > ColumnWidths;
    QVector < qreal > LayerHeights;
    qreal ZeroBefore;
    QVector < QVector < QString > > parameterSpecs;

    //Data File
    QString FORMAT,DESCRIPTION;
    qint32 SITES,FREQUENCIESCOUNT,DATABLOCKS;
    QStringList SITENAMES;
    QVector < qreal > OFFSETS,FREQUENCIES;
    QVector < DataBlock > datablocks;


public:
    void setProjectName(QString _name);
    void setFilePath(QString _name);
    void setStartUpFileName(QString _name);
    void setDataFileName(QString _name);
    void setMeshFileName(QString _name);
    void setModelFileName(QString _name);

    void readProject(QString _filename = tr(""));

    QString ProjectName();
    QString FilePath();
    QString StartUpFileName();
    QString DataFileName();
    QString MeshFileName();
    QString ModelFileName();


private:
    bool readStartUpFile(QString _filename);
    bool readDataFile(QString _filename);
    bool readMeshFile(QString _filename);
    bool readModelFile(QString _filename);

    bool writeStartUpFile(QString _filename);
    bool writeDataFile(QString _filename);
    bool writeMeshFile(QString _filename);
    bool writeModelFile(QString _filename);
    bool exportData(QString _filename);

    bool setModel();
    bool saveModel();

signals:
    void initModelFromOccam(QVector < QVector < unit > > _model);
    void initFrequency(QVector < qreal > _frequencies);
    void initSites(QStringList _sitenames,QVector < qreal > _offsets);
    void initDataBlocks(QVector < DataBlock > _datablocks);
    void initDataInformation(QStringList _sitenames,
                             QVector < qreal > _offsets,
                             QVector < qreal > _frequencies,
                             QVector < DataBlock > _datablocks);

public slots:
    void setFrequency(QVector < qreal > _frequencies);
    void setSites(QStringList _sitenames,QVector < qreal > _offsets);
    void setDataBlocks(QVector < DataBlock > _datablocks);
    void setDataInformation(QStringList _sitenames,
                            QVector < qreal > _offsets,
                            QVector < qreal > _frequencies,
                            QVector < DataBlock > _datablocks);
    void setModelFromModeling(QVector < QVector < unit > > _model);
    void forwardInversionLog();
    void processFinished(int _status);

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // OCCAMPROJECT_H
