#include "occamproject.h"
#include "ui_occamproject.h"

OccamProject::OccamProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OccamProject)
{
    ui->setupUi(this);

    site = new Site();
    connect(this,SIGNAL(initSites(QStringList,QVector<qreal>)),
            site,SLOT(initSites(QStringList,QVector<qreal>)));
    connect(this,SIGNAL(initFrequency(QVector<qreal>)),
            site,SLOT(initFrequency(QVector<qreal>)));
    connect(this,SIGNAL(initDataBlocks(QVector<DataBlock>)),
            site,SLOT(initDataBlocks(QVector<DataBlock>)));
    connect(this,SIGNAL(initDataInformation(QStringList,QVector<qreal>,QVector<qreal>,QVector<DataBlock>)),
            site,SLOT(initDataInformation(QStringList,QVector<qreal>,QVector<qreal>,QVector<DataBlock>)));
    connect(site,SIGNAL(setSites(QStringList,QVector<qreal>)),
            this,SLOT(setSites(QStringList,QVector<qreal>)));
    connect(site,SIGNAL(setFrequency(QVector<qreal>)),
            this,SLOT(setFrequency(QVector<qreal>)));
    connect(site,SIGNAL(setDataBlocks(QVector<DataBlock>)),
            this,SLOT(setDataBlocks(QVector<DataBlock>)));
    connect(site,SIGNAL(setDataInformation(QStringList,QVector<qreal>,QVector<qreal>,QVector<DataBlock>)),
            this,SLOT(setDataInformation(QStringList,QVector<qreal>,QVector<qreal>,QVector<DataBlock>)));
    modeling = new Occam2DMTModel();
    connect(this,SIGNAL(initModelFromOccam(QVector<QVector<unit> >)),
            modeling,SLOT(initModelFromOccam(QVector<QVector<unit> >)));
    connect(modeling,SIGNAL(setModelFromModeling(QVector<QVector<unit> >)),
            this,SLOT(setModelFromModeling(QVector<QVector<unit> >)));
    process = new QProcess(this);
    connect(process,SIGNAL(readyReadStandardOutput()),
            this,SLOT(forwardInversionLog()));
    connect(process,SIGNAL(finished(int)),
            this,SLOT(processFinished(int)));
    browser = new QTextBrowser();
    browser->setWindowTitle("Forward");
    browser->clear();
}

OccamProject::~OccamProject()
{
    delete ui;
}

bool OccamProject::setModel()
{
    QVector < QVector < unit > > model;
    qint32 ycount = number2 - 1;
    qint32 zcount = number3 - 1;
    qreal dycount = 0;
    for(int i=0;i<ycount;i++)
    {
        dycount += ColumnWidths[i];
    }
    qreal y = -dycount/2;
    qreal z = 0;
    model.clear();
    QVector < unit > tempLayer;
    for(int i=0;i<zcount;i++)
    {
        y = -dycount/2;
        tempLayer.clear();
        for(int j=0;j<ycount;j++)
        {
            unit temp;
            temp.setX(y);
            temp.setWidth(ColumnWidths[j]);
            temp.setY(z);
            temp.setHeight(-LayerHeights[i]);
            temp.value = 50;
            tempLayer<<temp;
            y += ColumnWidths[j];
        }
        model<<tempLayer;
        z -= LayerHeights[i];
    }
    qint32 currentLayer = 0;
    QVector < qint32 > currentLayers;
    qint32 currentParameter = 0;
    for(int i=0;i<NUMLAYERS;i++)
    {
        currentLayers.clear();
        int tempLayerNumber = currentLayer;
        for(int j=0;j<layers[i].layers;j++)
        {
            currentLayers<<currentLayer;
            currentLayer++;
        }
        qint32 currentCol = 0;
        for(int k=0;k<layers[i].count;k++)
        {
            if(parameterSpecs[tempLayerNumber*4][currentCol]==tr("0"))
            {
                for(int l=0;l<layers[i].number[k];l++)
                {
                    for(int j=0;j<layers[i].layers;j++)
                    {
                        model[currentLayers[j]][currentCol].value = AirRho;
                    }
                    currentCol++;
                }
            }
            else
            {
                for(int l=0;l<layers[i].number[k];l++)
                {
                    for(int j=0;j<layers[i].layers;j++)
                    {
                        model[currentLayers[j]][currentCol].value = parameters[currentParameter];
                    }
                    currentCol++;
                }
                currentParameter++;
            }
        }
    }
    emit initModelFromOccam(model);
    return true;
}

void OccamProject::setModelFromModeling(QVector < QVector < unit > > _model)
{
    BINDINGOFFSET = _model[0][5].x();
    ui->lineEdit_BINDINGOFFSET->setText(QString::number(BINDINGOFFSET));
    number3 = _model.count() + 1;
    ui->lineEdit3->setText(QString::number(number3));
    number2 = _model[0].count() + 1;
    ui->lineEdit2->setText(QString::number(number2));
    LayerHeights.clear();
    ColumnWidths.clear();
    for(int i=0;i<number3-1;i++)
    {
        LayerHeights<<fabs(_model[i][0].height());
    }
    for(int i=0;i<number2-1;i++)
    {
        ColumnWidths<<fabs(_model[0][i].width());
    }
    parameterSpecs.clear();
    QVector < QString > temp;
    for(int i=0;i<number3-1;i++)
    {
        for(int k=0;k<4;k++)
        {
            temp.clear();
            for(int j=0;j<number2-1;j++)
            {
                if(sameNumber(_model[i][j].value,AirRho))
                {
                    temp<<"0";
                }
                else if(_model[i][j].editable)
                {
                    temp<<"?";
                }
                else
                {
                    temp<<"?";
                }
            }
            parameterSpecs<<temp;
        }
    }
    NUMLAYERS = 0;
    layers.clear();
    ParamCount = 0;
    parameters.clear();

    bool Single;
    int i = 0;
    Layer templayer;
    //1到倒数第12层
    while(i<number3 - 1 -11)
    {
        QVector < unit > layer1 = _model[i];
        QVector < unit > layer2 = _model[i+1];
        bool same = true;
        for(int j=0;j<number2-1;j++)
        {
            if(!sameNumber(layer1[j].value,layer2[j].value))
            {
                same = false;
                break;
            }
        }
        if(same&&i + 1<number3 - 1 -11)
        {
            templayer.layers = 2;
            i += 2;
        }
        else
        {
            templayer.layers = 1;
            i++;
        }
        int j = 0;
        for(int k=1;k<5;k++)
        {
            if(!sameNumber(layer1[k].value,layer1[0].value))
            {
                break;
            }
            j++;
        }
        templayer.number<<j+1;
        j++;
        if(!sameNumber(layer1[0].value,AirRho))
        {
            parameters<<layer1[0].value;
        }
        Single = false;
        while(j<number2-6)
        {
            if(!sameNumber(layer1[j].value,AirRho))
            {
                parameters<<layer1[j].value;
            }
            if(sameNumber(layer1[j].value,layer1[j+1].value)
                    && j+1<number2-6
                    &&!Single)
            {
                templayer.number<<2;
                j+=2;
            }
            else
            {
                templayer.number<<1;
                j++;
                if(Single)
                {
                    Single = false;
                }
                else
                {
                    Single = true;
                }
            }
        }
        templayer.number<<5;
        if(!sameNumber(layer1[number2-2].value,AirRho))
        {
            parameters<<layer1[number2-2].value;
        }
        templayer.count = templayer.number.count();
        layers<<templayer;
        templayer.number.clear();
    }

    //倒数第11,10层，中间为4
    for(int k=0;k<2;k++)
    {
        templayer.layers = 1;
        templayer.number<<5;
        if(!sameNumber(_model[i].first().value,AirRho))
        {
            parameters<<_model[i].first().value;
        }
        int j = 5;
        while(j<number2-1-5)
        {
            if(!sameNumber(_model[i][j].value,AirRho))
            {
                parameters<<_model[i][j].value;
            }
            if(number2-1-5-j<4)
            {
                templayer.number<<number2-1-5-j;
                j += number2-1-5-j;
            }
            else
            {
                templayer.number<<4;
                j += 4;
            }
        }
        templayer.number<<5;
        if(!sameNumber(_model[i].last().value,AirRho))
        {
            parameters<<_model[i].last().value;
        }
        templayer.count = templayer.number.count();
        layers<<templayer;
        templayer.number.clear();
    }

    //倒数第9,8层，中间为8
    for(int k=0;k<2;k++)
    {
        templayer.layers = 1;
        templayer.number<<5;
        if(!sameNumber(_model[i].first().value,AirRho))
        {
            parameters<<_model[i].first().value;
        }
        int j = 5;
        while(j<number2-1-5)
        {
            if(!sameNumber(_model[i][j].value,AirRho))
            {
                parameters<<_model[i][j].value;
            }
            if(number2-1-5-j<8)
            {
                templayer.number<<number2-1-5-j;
                j += number2-1-5-j;
            }
            else
            {
                templayer.number<<8;
                j += 8;
            }
        }
        templayer.number<<5;
        if(!sameNumber(_model[i].last().value,AirRho))
        {
            parameters<<_model[i].last().value;
        }
        templayer.count = templayer.number.count();
        layers<<templayer;
        templayer.number.clear();
    }

    //倒数第7,6层，中间为16
    for(int k=0;k<2;k++)
    {
        templayer.layers = 1;
        templayer.number<<5;
        if(!sameNumber(_model[i].first().value,AirRho))
        {
            parameters<<_model[i].first().value;
        }
        int j = 5;
        while(j<number2-1-5)
        {
            if(!sameNumber(_model[i][j].value,AirRho))
            {
                parameters<<_model[i][j].value;
            }
            if(number2-1-5-j<16)
            {
                templayer.number<<number2-1-5-j;
                j += number2-1-5-j;
            }
            else
            {
                templayer.number<<16;
                j += 16;
            }
        }
        templayer.number<<5;
        if(!sameNumber(_model[i].last().value,AirRho))
        {
            parameters<<_model[i].last().value;
        }
        templayer.count = templayer.number.count();
        layers<<templayer;
        templayer.number.clear();
    }

    //倒数第5层，中间为32
    templayer.layers = 1;
    templayer.number<<5;
    if(!sameNumber(_model[i].first().value,AirRho))
    {
        parameters<<_model[i].first().value;
    }
    int j = 5;
    while(j<number2-1-5)
    {
        if(!sameNumber(_model[i][j].value,AirRho))
        {
            parameters<<_model[i][j].value;
        }
        if(number2-1-5-j<32)
        {
            templayer.number<<number2-1-5-j;
            j += number2-1-5-j;
        }
        else
        {
            templayer.number<<32;
            j += 32;
        }
    }
    templayer.number<<5;
    if(!sameNumber(_model[i].last().value,AirRho))
    {
        parameters<<_model[i].last().value;
    }
    templayer.count = templayer.number.count();
    layers<<templayer;
    templayer.number.clear();

    //倒数第4层，中间为64
    templayer.layers = 1;
    templayer.number<<5;
    if(!sameNumber(_model[i].first().value,AirRho))
    {
        parameters<<_model[i].first().value;
    }
    j = 5;
    while(j<number2-1-5)
    {
        if(!sameNumber(_model[i][j].value,AirRho))
        {
            parameters<<_model[i][j].value;
        }
        if(number2-1-5-j<64)
        {
            templayer.number<<number2-1-5-j;
            j += number2-1-5-j;
        }
        else
        {
            templayer.number<<64;
            j += 64;
        }
    }
    templayer.number<<5;
    if(!sameNumber(_model[i].last().value,AirRho))
    {
        parameters<<_model[i].last().value;
    }
    templayer.count = templayer.number.count();
    layers<<templayer;
    templayer.number.clear();

    //倒数第3层，中间为128
    templayer.layers = 1;
    templayer.number<<5;
    if(!sameNumber(_model[i].first().value,AirRho))
    {
        parameters<<_model[i].first().value;
    }
    j = 5;
    while(j<number2-1-5)
    {
        if(!sameNumber(_model[i][j].value,AirRho))
        {
            parameters<<_model[i][j].value;
        }
        if(number2-1-5-j<128)
        {
            templayer.number<<number2-1-5-j;
            j += number2-1-5-j;
        }
        else
        {
            templayer.number<<128;
            j += 128;
        }
    }
    templayer.number<<5;
    if(!sameNumber(_model[i].last().value,AirRho))
    {
        parameters<<_model[i].last().value;
    }
    templayer.count = templayer.number.count();
    layers<<templayer;
    templayer.number.clear();

    //倒数第2层，中间为256
    templayer.layers = 1;
    templayer.number<<5;
    if(!sameNumber(_model[i].first().value,AirRho))
    {
        parameters<<_model[i].first().value;
    }
    j = 5;
    while(j<number2-1-5)
    {
        if(!sameNumber(_model[i][j].value,AirRho))
        {
            parameters<<_model[i][j].value;
        }
        if(number2-1-5-j<256)
        {
            templayer.number<<number2-1-5-j;
            j += number2-1-5-j;
        }
        else
        {
            templayer.number<<256;
            j += 256;
        }
    }
    templayer.number<<5;
    if(!sameNumber(_model[i].last().value,AirRho))
    {
        parameters<<_model[i].last().value;
    }
    templayer.count = templayer.number.count();
    layers<<templayer;
    templayer.number.clear();

    //倒数第1层，中间为512
    templayer.layers = 1;
    templayer.number<<5;
    if(!sameNumber(_model[i].first().value,AirRho))
    {
        parameters<<_model[i].first().value;
    }
    j = 5;
    while(j<number2-1-5)
    {
        if(!sameNumber(_model[i][j].value,AirRho))
        {
            parameters<<_model[i][j].value;
        }
        if(number2-1-5-j<512)
        {
            templayer.number<<number2-1-5-j;
            j += number2-1-5-j;
        }
        else
        {
            templayer.number<<512;
            j += 512;
        }
    }
    templayer.number<<5;
    if(!sameNumber(_model[i].last().value,AirRho))
    {
        parameters<<_model[i].last().value;
    }
    templayer.count = templayer.number.count();
    layers<<templayer;
    templayer.number.clear();

    NUMLAYERS = layers.count();
    ParamCount = parameters.count();
    ui->lineEdit_NUMLAYERS->setText(QString::number(NUMLAYERS));
    ui->lineEdit_ParamCount->setText(QString::number(ParamCount));
    return;
}

void OccamProject::on_toolButton_filepath_clicked()
{
    readStartUpFile(QFileDialog::getSaveFileName(this, tr("Startup File"),
                                                    filePath,
                                                    tr("Iter (*.iter);;All (*)")));
    return;
}

void OccamProject::on_toolButton_ModelFile_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Model File"),
                                                    filePath,
                                                    tr("Model (*.model);;All (*)"));
    if(readModelFile(filename))
    {
        qint32 index = filename.lastIndexOf("/");
        modelFileName = filename.right(filename.count()-index);
        ui->lineEdit_MoedlFile->setText(modelFileName);
    }
    return;
}

void OccamProject::on_toolButton_DateFile_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Model File"),
                                                    filePath,
                                                    tr("DAT (*.dat *.txt);;All (*)"));
    if(readDataFile(filename))
    {
        qint32 index = filename.lastIndexOf("/");
        dataFileName = filename.right(filename.count()-index);
        ui->lineEdit_DateFile->setText(dataFileName);
    }
    return;
}

void OccamProject::on_pushButton_SITES_clicked()
{
    emit initDataInformation(SITENAMES,OFFSETS,FREQUENCIES,datablocks);
    site->show();
}

bool OccamProject::readStartUpFile(QString _filename)
{
    if(_filename.isEmpty())
    {
        _filename = filePath + "/" + startUpFileName;
    }
    QFile file(_filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
//        file.open(QIODevice::WriteOnly|QIODevice::Text);
//        file.close();
//        file.open(QIODevice::ReadOnly|QIODevice::Text);
    }
    filePath = _filename;
    qint32 index = filePath.lastIndexOf("/");
    filePath = filePath.left(index);
    startUpFileName = _filename.remove(filePath);
    startUpFileName.remove("/");
    ui->lineEdit_filepath->setText(filePath + "/" + startUpFileName);
    QString line;
    QString tempTitle;
    while(!file.atEnd())
    {
        line = file.readLine();
        line.remove(tr("\n"));
        qint32 index = line.indexOf(":");
        tempTitle = line.left(index);
        if(tempTitle == "Format")
        {
            Format = line.right(line.count()-20);
            ui->lineEdit_Format->setText(Format);
        }
        else if(tempTitle == "Description")
        {
            Description = line.right(line.count()-20);
            ui->lineEdit_Description->setText(Description);
        }
        else if(tempTitle == "Model File")
        {
            modelFileName = line.right(line.count()-20);
        }
        else if(tempTitle == "Data File")
        {
            dataFileName = line.right(line.count()-20);
        }
        else if(tempTitle == "Date/Time")
        {
            DateTime = line.right(line.count()-20);
            ui->lineEdit_DateTime->setText(DateTime);
        }
        else if(tempTitle == "Iterations to run")
        {
            IterationsToRun = line.right(line.count()-20).toInt();
            ui->lineEdit_IterationsToRun->setText(QString::number(IterationsToRun));
        }
        else if(tempTitle == "Target Misfit")
        {
            TargetMisfit = line.right(line.count()-20).toDouble();
            ui->lineEdit_TargetMisfit->setText(QString::number(TargetMisfit));
        }
        else if(tempTitle == "Roughness Type")
        {
            RoughnessType = line.right(line.count()-20).toInt();
            ui->lineEdit_RoughnessType->setText(QString::number(RoughnessType));
        }
        else if(tempTitle == "Diagonal Penalties")
        {
            DiagonalPenalties = line.right(line.count()-20).toInt();
            ui->lineEdit_DiagonalPenalties->setText(QString::number(DiagonalPenalties));
        }
        else if(tempTitle == "!Modify Roughness")
        {
            ModifyRoughness = line;
            ui->lineEdit_ModifyRoughness->setText(ModifyRoughness);
        }
        else if(tempTitle == "Modify Roughness")
        {
            ModifyRoughness = line;
            ui->lineEdit_ModifyRoughness->setText(ModifyRoughness);
        }
        else if(tempTitle == "Stepsize Cut Count")
        {
            StepsizeCutCount = line.right(line.count()-20).toInt();
            ui->lineEdit_StepsizeCutCount->setText(QString::number(StepsizeCutCount));
        }
        else if(tempTitle == "!Model Limits")
        {
            ModelLimits = line;
            ui->lineEdit_ModelLimits->setText(ModelLimits);
        }
        else if(tempTitle == "Model Limits")
        {
            ModelLimits = line;
            ui->lineEdit_ModelLimits->setText(ModelLimits);
        }
        else if(tempTitle == "!Model Value Steps")
        {
            ModelValueSteps = line;
            ui->lineEdit_ModelValueSteps->setText(ModelValueSteps);
        }
        else if(tempTitle == "Model Value Steps")
        {
            ModelValueSteps = line;
            ui->lineEdit_ModelValueSteps->setText(ModelValueSteps);
        }
        else if(tempTitle == "Debug Level")
        {
            DebugLevel = line.right(line.count()-20).toInt();
            ui->lineEdit_DebugLevel->setText(QString::number(DebugLevel));
        }
        else if(tempTitle == "Iteration")
        {
            Iteration = line.right(line.count()-20).toInt();
            ui->lineEdit_Iteration->setText(QString::number(Iteration));
        }
        else if(tempTitle == "Lagrange Value")
        {
            LagrangeValue = line.right(line.count()-20).toDouble();
            ui->lineEdit_LagrangeValue->setText(QString::number(LagrangeValue));
        }
        else if(tempTitle == "Roughness Value")
        {
            RoughnessValue = line.right(line.count()-20).toDouble();
            ui->lineEdit_RoughnessValue->setText(QString::number(RoughnessValue));
        }
        else if(tempTitle == "Misfit Value")
        {
            MisfitValue = line.right(line.count()-20).toDouble();
            ui->lineEdit_MisfitValue->setText(QString::number(MisfitValue));
        }
        else if(tempTitle == "Misfit Reached")
        {
            MisfitReached = line.right(line.count()-20).toDouble();
            ui->lineEdit_MisfitReached->setText(QString::number(MisfitReached));
        }
        else if(tempTitle == "Param Count")
        {
            ParamCount = line.right(line.count()-20).toInt();
            ui->lineEdit_ParamCount->setText(QString::number(ParamCount));
            QString string;
            while(!file.atEnd())
            {
                string.push_back(file.readLine());
            }
            QStringList stringlist = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
            if(stringlist.count()<ParamCount)
            {
                qDebug()<<"Error!The paramcount is "<<ParamCount<<",but there are "<<stringlist.count()<<" data.";
                file.close();
                return false;
            }
            parameters.clear();
            for(int i=0;i<ParamCount;i++)
            {
                parameters<<QString(stringlist[i]).toDouble();
            }
        }
        else
        {
            qDebug()<<"Startup file read error!The temp title is"<<tempTitle<<".";
        }
    }
    file.close();

    if(readDataFile(filePath + "/" + dataFileName))
    {
        ui->lineEdit_DateFile->setText(dataFileName);
    }
    if(readModelFile(filePath + "/" + modelFileName))
    {
        ui->lineEdit_MoedlFile->setText(modelFileName);
    }
    if(readMeshFile(filePath + "/" + meshFileName))
    {
        ui->lineEdit_MESHFILE->setText(meshFileName);
    }
    return true;
}

bool OccamProject::readDataFile(QString _filename)
{
    QFile file(_filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    QString line;
    line = file.readLine();
    line.remove(tr("\n"));
    FORMAT = line.right(line.count()-18);
    ui->lineEdit_FORMAT->setText(FORMAT);
    line = file.readLine();
    line.remove(tr("\n"));
    DESCRIPTION = line.right(line.count()-18);
    ui->lineEdit_DESCRIPTION->setText(DESCRIPTION);
    line = file.readLine();
    line.remove(tr("\n"));
    line.remove("SITES:");
    SITES = line.toInt();
    ui->lineEdit_SITES->setText(QString::number(SITES));
    ui->lineEdit_OFFSETS->setText(QString::number(SITES));
    SITENAMES.clear();
    for(int i=0;i<SITES;i++)
    {
        line = file.readLine();
        line.remove(tr("\n"));
        SITENAMES<<line;
    }
    line = file.readLine();
    OFFSETS.clear();
    for(int i=0;i<SITES;i++)
    {
        line = file.readLine();
        line.remove(tr("\n"));
        OFFSETS<<line.toDouble();
    }
    line = file.readLine();
    line.remove(tr("\n"));
    line.remove("FREQUENCIES:");
    FREQUENCIESCOUNT = line.toInt();
    ui->lineEdit_FREQUENCIES->setText(QString::number(FREQUENCIESCOUNT));
    FREQUENCIES.clear();
    for(int i=0;i<FREQUENCIESCOUNT;i++)
    {
        line = file.readLine();
        line.remove(tr("\n"));
        FREQUENCIES<<line.toDouble();
    }
    line = file.readLine();
    line.remove(tr("\n"));
    line.remove("DATA BLOCKS:");
    DATABLOCKS = line.toInt();
    line = file.readLine();
    QStringList stringlist;
    datablocks.clear();
    DataBlock tempDataBlock;
    for(int i=0;i<DATABLOCKS;i++)
    {
        line = file.readLine();
        stringlist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        tempDataBlock.SITE=QString(stringlist[0]).toInt();
        tempDataBlock.FREQ=QString(stringlist[1]).toInt();
        tempDataBlock.TYPE=QString(stringlist[2]).toInt();
        tempDataBlock.DATUM=QString(stringlist[3]).toDouble();
        tempDataBlock.ERR=QString(stringlist[4]).toDouble();
        datablocks<<tempDataBlock;
    }
    file.close();
    return true;
}

bool OccamProject::readMeshFile(QString _filename)
{
    QFile file(_filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    QString line;
    line = file.readLine();
    DescriptionTextOfMeshFile = line.remove(tr("\n"));
    ui->lineEdit_MESHDESCRIPTION->setText(DescriptionTextOfMeshFile);
    line = file.readLine();
    QStringList stringlist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
    number1 = QString(stringlist[0]).toInt();
    number2 = QString(stringlist[1]).toInt();
    number3 = QString(stringlist[2]).toInt();
    number4 = QString(stringlist[3]).toInt();
    number5 = QString(stringlist[4]).toInt();
    number6 = QString(stringlist[5]).toInt();
    ui->lineEdit1->setText(QString::number(number1));
    ui->lineEdit2->setText(QString::number(number2));
    ui->lineEdit3->setText(QString::number(number3));
    ui->lineEdit4->setText(QString::number(number4));
    ui->lineEdit5->setText(QString::number(number5));
    ui->lineEdit6->setText(QString::number(number6));
    FixedResistivity.clear();
    ColumnWidths.clear();
    LayerHeights.clear();
    ZeroBefore = 0;
    parameterSpecs.clear();
    QString string;
    string.clear();
    while(!file.atEnd())
    {
        string.push_back(file.readLine());
    }
    file.close();
    stringlist = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
    if(number4>0)
    {
        for(int i=0;i<number4;i++)
        {
            FixedResistivity<<QString(stringlist[0]).toDouble();
            stringlist.removeFirst();
        }
    }
    if(number2>0)
    {
        for(int i=0;i<number2-1;i++)
        {
            ColumnWidths<<QString(stringlist[0]).toDouble();
            stringlist.removeFirst();
        }
    }
    if(number3>0)
    {
        for(int i=0;i<number3-1;i++)
        {
            LayerHeights<<QString(stringlist[0]).toDouble();
            stringlist.removeFirst();
        }
    }
    ZeroBefore = QString(stringlist[0]).toInt();
    stringlist.removeFirst();
    parameterSpecs.clear();
    QVector < QString > tempParameterSpecs;
    for(int i=0;i<(number3-1)*4;i++)
    {
        tempParameterSpecs.clear();
        QString tempstring = stringlist[i];
        for(int j=0;j<number2-1;j++)
        {
            tempParameterSpecs<<QString(tempstring[j]);
        }
        parameterSpecs<<tempParameterSpecs;
    }
    return true;
}

bool OccamProject::readModelFile(QString _filename)
{
    QFile file(_filename);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    QString line;
    QString tempTitle;
    while(!file.atEnd())
    {
        line = file.readLine();
        line.remove(tr("\n"));
        qint32 index = line.indexOf(":");
        tempTitle = line.left(index);
        if(tempTitle == "FORMAT")
        {
            MFORMAT = line.right(line.count()-18);
            ui->lineEdit_MFORMAT->setText(MFORMAT);
        }
        else if(tempTitle == "MODEL NAME")
        {
            MODELNAME = line.right(line.count()-18);
            ui->lineEdit_MODELNAME->setText(MODELNAME);
        }
        else if(tempTitle == "DESCRIPTION")
        {
            MDESCRIPTION = line.right(line.count()-18);
            ui->lineEdit_MDESCRIPTION->setText(MDESCRIPTION);
        }
        else if(tempTitle == "MESH FILE")
        {
            meshFileName = line.right(line.count()-18);
        }
        else if(tempTitle == "MESH TYPE")
        {
            MESHTYPE = line.right(line.count()-18);
            ui->lineEdit_MESHTYPE->setText(MESHTYPE);
        }
        else if(tempTitle == "STATICS FILE")
        {
            STATICSFILE = line.right(line.count()-18);
            ui->lineEdit_STATICSFILE->setText(STATICSFILE);
        }
        else if(tempTitle == "PREJUDICE FILE")
        {
            PREJUDICEFILE = line.right(line.count()-18);
            ui->lineEdit_PREJUDICEFILE->setText(PREJUDICEFILE);
        }
        else if(tempTitle == "BINDING OFFSET")
        {
            BINDINGOFFSET = line.right(line.count()-18).toDouble();
            ui->lineEdit_BINDINGOFFSET->setText(QString::number(BINDINGOFFSET));
        }
        else if(tempTitle == "NUM LAYERS")
        {
            NUMLAYERS = line.right(line.count()-18).toInt();
            layers.clear();
            ui->lineEdit_NUMLAYERS->setText(QString::number(NUMLAYERS));
            QStringList stringlist;
            QString string;
            Layer templayer;
            for(int i=0;i<NUMLAYERS;i++)
            {
                string = file.readLine();
                stringlist = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
                templayer.layers = QString(stringlist[0]).toInt();
                templayer.count = QString(stringlist[1]).toInt();
                string = file.readLine();
                stringlist = string.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
                templayer.number.clear();
                for(int j=0;j<templayer.count;j++)
                {
                    templayer.number<<QString(stringlist[j]).toInt();
                }
                layers<<templayer;
            }
        }
        else if(tempTitle == "NUMBER EXCEPTIONS 0")
        {
            NUMBEREXCEPTIONS = line.remove("NUMBER EXCEPTIONS").toInt();
        }
        else
        {
            qDebug()<<"Model file read error!The temp title is"<<tempTitle<<".";
        }
    }
    file.close();
    return true;
}

bool OccamProject::writeStartUpFile(QString _filename)
{
    QFile file(_filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QString text;
    Format = ui->lineEdit_Format->text();
    text = tr("Format:             ") + Format + tr("\n");
    Description = ui->lineEdit_Description->text();
    text += tr("Description:        ") + Description + tr("\n");
    modelFileName = ui->lineEdit_MoedlFile->text();
    text += tr("Model File:         ") + modelFileName + tr("\n");
    dataFileName = ui->lineEdit_DateFile->text();
    text += tr("Data File:          ") + dataFileName + tr("\n");
    DateTime = ui->lineEdit_DateTime->text();
    text += tr("Date/Time:          ") + DateTime + tr("\n");
    IterationsToRun = ui->lineEdit_IterationsToRun->text().toInt();
    text += tr("Iterations to run:  ") + QString::number(IterationsToRun) + tr("\n");
    TargetMisfit = ui->lineEdit_TargetMisfit->text().toDouble();
    text += tr("Target Misfit:      ") + QString::number(TargetMisfit) + tr("\n");
    RoughnessType = ui->lineEdit_RoughnessType->text().toInt();
    text += tr("Roughness Type:     ") + QString::number(RoughnessType) + tr("\n");
    DiagonalPenalties = ui->lineEdit_DiagonalPenalties->text().toInt();
    text += tr("Diagonal Penalties: ") + QString::number(DiagonalPenalties) + tr("\n");
    ModifyRoughness = ui->lineEdit_ModifyRoughness->text();
    text += ModifyRoughness + tr("\n");
    StepsizeCutCount = ui->lineEdit_StepsizeCutCount->text().toInt();
    text += tr("Stepsize Cut Count: ") + QString::number(StepsizeCutCount) + tr("\n");
    ModelLimits = ui->lineEdit_ModelLimits->text();
    text += ModelLimits + tr("\n");
    ModelValueSteps = ui->lineEdit_ModelValueSteps->text();
    text += ModelValueSteps + tr("\n");
    DebugLevel = ui->lineEdit_DebugLevel->text().toInt();
    text += tr("Debug Level:        ") + QString::number(DebugLevel) + tr("\n");
    Iteration = ui->lineEdit_Iteration->text().toInt();
    text += tr("Iteration:          ") + QString::number(Iteration) + tr("\n");
    LagrangeValue = ui->lineEdit_LagrangeValue->text().toDouble();
    text += tr("Lagrange Value:     ") + QString::number(LagrangeValue) + tr("\n");
    RoughnessValue = ui->lineEdit_RoughnessValue->text().toDouble();
    text += tr("Roughness Value:    ") + QString::number(RoughnessValue) + tr("\n");
    MisfitValue = ui->lineEdit_MisfitValue->text().toDouble();
    text += tr("Misfit Value:       ") + QString::number(MisfitValue) + tr("\n");
    MisfitReached = ui->lineEdit_MisfitReached->text().toDouble();
    text += tr("Misfit Reached:     ") + QString::number(MisfitReached) + tr("\n");
    text += tr("Param Count:        ") + QString::number(ParamCount) + tr("\n");
    for(int i=0;i<ParamCount;i++)
    {
        text += QString::number(parameters[i]) + "  ";
        if(i%8==7)
        {
            text += tr("\n");
        }
    }
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

bool OccamProject::writeDataFile(QString _filename)
{
    QFile file(_filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QString text;
    FORMAT = ui->lineEdit_FORMAT->text();
    text = tr("FORMAT:           ") + FORMAT + tr("\n");
    DESCRIPTION = ui->lineEdit_DESCRIPTION->text();
    text += tr("DESCRIPTION:      ") + DESCRIPTION + tr("\n");
    text += tr("SITES:            ") + QString::number(SITES) + tr("\n");
    for(int i=0;i<SITES;i++)
    {
        text += SITENAMES[i] + tr("\n");
    }
    text += tr("OFFSETS (M):\n");
    for(int i=0;i<SITES;i++)
    {
        text += QString::number(OFFSETS[i]) + tr("\n");
    }
    text += tr("FREQUENCIES:      ") + QString::number(FREQUENCIESCOUNT) + tr("\n");
    for(int i=0;i<FREQUENCIESCOUNT;i++)
    {
        text += QString::number(FREQUENCIES[i]) + tr("\n");
    }
    text += tr("DATA BLOCKS:      ") + QString::number(DATABLOCKS) + tr("\n");
    text += tr("SITE FREQ TYPE DATUM  ERR\n");
    for(int i=0;i<DATABLOCKS;i++)
    {
        text += Q2F(datablocks[i].SITE,5)
                + Q2F(datablocks[i].FREQ,5)
                + Q2F(datablocks[i].TYPE,5)
                + Q2F(datablocks[i].DATUM,"f",10,4)
                + Q2F(datablocks[i].ERR,"f",10,4)
                + "\n";
    }
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

bool OccamProject::writeMeshFile(QString _filename)
{
    QFile file(_filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QString text;
    DescriptionTextOfMeshFile = ui->lineEdit_MESHDESCRIPTION->text();
    text = DescriptionTextOfMeshFile + tr("\n");
    text += QString::number(number1) + tr("  ")
            + QString::number(number2) + tr("  ")
            + QString::number(number3) + tr("  ")
            + QString::number(number4) + tr("  ")
            + QString::number(number5) + tr("  ")
            + QString::number(number6) + tr("\n");
    if(number4!=0)
    {
        for(int i=0;i<number4-1;i++)
        {
            text += Q2F(FixedResistivity[i],"f",10,1);
            if(i%5==4)
            {
                text += tr("\n");
            }
        }
    }
    QString right = text;
    if(right.right(1)!=tr("\n"))
    {
        text += tr("\n");
    }
    for(int i=0;i<number2-1;i++)
    {
        text += Q2F(ColumnWidths[i],"f",10,1);
        if(i%5==4)
        {
            text += tr("\n");
        }
    }
    right = text;
    if(right.right(1)!=tr("\n"))
    {
        text += tr("\n");
    }
    for(int i=0;i<number3-1;i++)
    {
        text += Q2F(LayerHeights[i],"f",10,1);
        if(i%5==4)
        {
            text += tr("\n");
        }
    }
    right = text;
    if(right.right(1)!=tr("\n"))
    {
        text += tr("\n");
    }
    text += tr("0\n");
    int count = parameterSpecs.count();
    int count1 = parameterSpecs[0].count();
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count1;j++)
        {
            text += parameterSpecs[i][j];
        }
        text += tr("\n");
    }
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

bool OccamProject::writeModelFile(QString _filename)
{
    QFile file(_filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QString text;
    MFORMAT = ui->lineEdit_MFORMAT->text();
    text = tr("FORMAT:           ") + MFORMAT + tr("\n");
    MODELNAME = ui->lineEdit_MODELNAME->text();
    text += tr("MODEL NAME:       ") + MODELNAME + tr("\n");
    MDESCRIPTION = ui->lineEdit_MDESCRIPTION->text();
    text += tr("DESCRIPTION:      ") + MDESCRIPTION + tr("\n");
    meshFileName = ui->lineEdit_MESHFILE->text();
    text += tr("MESH FILE:        ") + meshFileName + tr("\n");
    MESHTYPE = ui->lineEdit_MESHTYPE->text();
    text += tr("MESH TYPE:        ") + MESHTYPE + tr("\n");
    STATICSFILE = ui->lineEdit_STATICSFILE->text();
    text += tr("STATICS FILE:     ") + STATICSFILE + tr("\n");
    PREJUDICEFILE = ui->lineEdit_PREJUDICEFILE->text();
    text += tr("PREJUDICE FILE:   ") + PREJUDICEFILE + tr("\n");
    BINDINGOFFSET = ui->lineEdit_BINDINGOFFSET->text().toDouble();
    text += tr("BINDING OFFSET:   ") + QString::number(BINDINGOFFSET) + tr("\n");
    text += tr("NUM LAYERS:       ") + QString::number(NUMLAYERS) + tr("\n");
    for(int i=0;i<NUMLAYERS;i++)
    {
        text += QString::number(layers[i].layers) + tr(" ") + QString::number(layers[i].count) + tr("\n");
        for(int j=0;j<layers[i].count;j++)
        {
            text += QString::number(layers[i].number[j]) + tr(" ");
        }
        text += "\n";
    }
    text += tr("NUMBER EXCEPTIONS ") + QString::number(NUMBEREXCEPTIONS) + tr("\n");
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

void OccamProject::on_toolButton_DateTime_clicked()
{
    QDateTime datetime = QDateTime::currentDateTime();
    DateTime = datetime.toString("MMM ddd dd hh:mm:ss yyyy");
    ui->lineEdit_DateTime->setText(DateTime);
}

void OccamProject::setFrequency(QVector < qreal > _frequencies)
{
    FREQUENCIES = _frequencies;
    return;
}

void OccamProject::setSites(QStringList _sitenames,QVector < qreal > _offsets)
{
    SITENAMES = _sitenames;
    OFFSETS = _offsets;
    return;
}

void OccamProject::setDataBlocks(QVector < DataBlock > _datablocks)
{
    datablocks = _datablocks;
    return;
}

void OccamProject::setDataInformation(QStringList _sitenames,
                                      QVector < qreal > _offsets,
                                      QVector < qreal > _frequencies,
                                      QVector < DataBlock > _datablocks)
{
    FREQUENCIES = _frequencies;
    FREQUENCIESCOUNT = FREQUENCIES.count();
    ui->lineEdit_FREQUENCIES->setText(QString::number(FREQUENCIESCOUNT));
    SITENAMES = _sitenames;
    SITES = SITENAMES.count();
    ui->lineEdit_SITES->setText(QString::number(SITES));
    OFFSETS = _offsets;
    datablocks = _datablocks;
    DATABLOCKS = datablocks.count();
    ui->lineEdit_DATABLOCKS->setText(QString::number(DATABLOCKS));
    return;
}

void OccamProject::on_pushButton_EditModel_clicked()
{
    setModel();
    modeling->show();
}

void OccamProject::setProjectName(QString _name)
{
    projectName = _name;
    this->setWindowTitle(projectName);
}

void OccamProject::setFilePath(QString _name)
{
    filePath = _name;
}

void OccamProject::setStartUpFileName(QString _name)
{
    startUpFileName = _name;
}

void OccamProject::setDataFileName(QString _name)
{
    dataFileName = _name;
}

void OccamProject::setMeshFileName(QString _name)
{
    meshFileName = _name;
}

void OccamProject::setModelFileName(QString _name)
{
    modelFileName = _name;
}

void OccamProject::readProject(QString _filename)
{
    readStartUpFile(_filename);
}

QString OccamProject::ProjectName()
{
    return projectName;
}

QString OccamProject::FilePath()
{
    return filePath;
}

QString OccamProject::StartUpFileName()
{
    return startUpFileName;
}

QString OccamProject::DataFileName()
{
    return dataFileName;
}

QString OccamProject::MeshFileName()
{
    return meshFileName;
}

QString OccamProject::ModelFileName()
{
    return modelFileName;
}

void OccamProject::on_pushButton_Forward_clicked()
{
    if(process->state()!=0)
    {
        return;
    }
    forwardOutputFilename = QFileDialog::getSaveFileName(this, tr("FWD File"),
                                                          filePath,
                                                          tr("FWD (*.fwd);;All (*)"));
    forwardOutputFilename = forwardOutputFilename.remove(filePath);
    forwardOutputFilename.remove("/");
    forwardOutputFilename.remove(".fwd");
    if (!forwardOutputFilename.isEmpty() && saveModel())
    {
        saveModel();
        QFile file(filePath + tr("/occam.log"));
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.close();
        file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        file.write(QString("Forward\n").toLatin1().data());
        file.write(QDateTime::currentDateTime().toString(tr("yyyy MM dd hh:mm:ss")).toLatin1().data());
        file.close();
        browser->setWindowTitle("Forward");
        browser->show();
        process->setWorkingDirectory(filePath);
        process->start(tr("./Occam2d -F ") + startUpFileName + tr(" ") + forwardOutputFilename);
    }
    return;
}

void OccamProject::forwardInversionLog()
{
    QFile file(filePath + tr("/occam.log"));
    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    QString cmdText = process->readAllStandardOutput();
    browser->insertPlainText(cmdText);
    browser->moveCursor(QTextCursor::End);
    file.write(cmdText.toLatin1().data());
    file.close();
    return;
}

void OccamProject::on_pushButton_Inversion_clicked()
{
    if(process->state()!=0)
    {
        return;
    }
    bool ok;
    QString inversionName = QInputDialog::getText(this, tr("Inversion"),
                                                  tr("Inversion name:"), QLineEdit::Normal,
                                                  tr("Inversion1"), &ok);
    if(ok&&saveModel())
    {
        QFile file(filePath + tr("/occam.log"));
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        file.close();
        file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
        file.write(QString("Inversion\n").toLatin1().data());
        file.write(QDateTime::currentDateTime().toString(tr("yyyy MM dd hh:mm:ss")).toLatin1().data());
        file.close();
        browser->setWindowTitle("Inversion");
        browser->show();
        process->setWorkingDirectory(filePath);
        process->start(tr("./Occam2d ") + startUpFileName + tr(" ") + inversionName);
    }
    return;
}

void OccamProject::processFinished(int _status)
{
    if(_status!=0)
    {
        return;
    }
    if(browser->windowTitle()==tr("Forward"))
    {
        readDataFile(filePath + tr("/") + forwardOutputFilename + tr(".fwd"));
        exportData(filePath + tr("/") + forwardOutputFilename + tr(".csv"));
        return;
    }
    else if(browser->windowTitle()==tr("Inversion"))
    {
        return;
    }
}

void OccamProject::on_pushButton_SaveProject_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Startup File"),
                                                    filePath,
                                                    tr("Iter (*.iter);;All (*)"));
    if(filename.isNull())
    {
        return;
    }
    filePath = filename;
    qint32 index = filePath.lastIndexOf("/");
    filePath = filePath.left(index);
    startUpFileName = filename.remove(filePath);
    startUpFileName.remove("/");
    ui->lineEdit_filepath->setText(filePath + "/" + startUpFileName);
    saveModel();
    return;
}

bool OccamProject::saveModel()
{
    on_toolButton_DateTime_clicked();
    if(writeStartUpFile(filePath + tr("/") + startUpFileName))
    {
        if(writeDataFile(filePath + tr("/") + dataFileName))
        {
            if(writeModelFile(filePath + tr("/") + modelFileName))
            {
                if(writeMeshFile(filePath + tr("/") + meshFileName))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void OccamProject::on_pushButton_ExportData_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Result File"),
                                                    filePath,
                                                    tr("Result (*.csv);;All (*)"));
    if(filename.isNull())
    {
        return;
    }
    exportData(filename);
    return;
}

bool OccamProject::exportData(QString _filename)
{

    QVector < QVector < QVector < DataError > > > datas;
    datas.clear();
    datas.resize(SITES);
    for(int i=0;i<SITES;i++)
    {
        datas[i].resize(FREQUENCIESCOUNT);
        for(int j=0;j<FREQUENCIESCOUNT;j++)
        {
            datas[i][j].resize(18);
            for(int k=0;k<18;k++)
            {
                datas[i][j][k].datum = -9999.9999;
                datas[i][j][k].error = -9999.9999;
            }
        }
    }
    foreach(DataBlock _block,datablocks)
    {
        datas[_block.SITE-1][_block.FREQ-1][_block.TYPE-1].datum = _block.DATUM;
        datas[_block.SITE-1][_block.FREQ-1][_block.TYPE-1].error = _block.ERR;
    }

    QFile file(_filename);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QStringList title;
    title.clear();
    title<<tr("TE apparent resistivity (log10)")<<tr("TE phase")<<tr("real(tipper) Hz/Hy")<<tr("imag(tipper) Hz/Hy")
        <<tr("TM apparent resistivity (log10)")<<tr("TM phase")<<tr("reserved for TM Ez/Ey tipper")<<tr("reserved for TM Ez/Ey tipper")
       <<tr("TE apparent resistivity (linear)")<<tr("TM apparent resistivity (linear)e")<<tr("real(Zxx)")<<tr("imag(Zxx)")
      <<tr("real(Zxy)")<<tr("imag(Zxy)")<<tr("real(Zyx)")<<tr("imag(Zyx)")<<tr("real(Zyy)")<<tr("imag(Zyy)");
    QString text = tr("Y,Frequency,Log10(tFrequency),");
    for(int i=0;i<18;i++)
    {
        text += title[i] + tr(",Error,");
    }
    text.chop(1);
    text += tr("\n");

    for(int i=0;i<SITES;i++)
    {
        for(int j=0;j<FREQUENCIESCOUNT;j++)
        {
            bool null = true;
            for(int k=0;k<18;k++)
            {
                if(datas[i][j][k].datum>-999||datas[i][j][k].error>-999)
                {
                    null = false;
                    break;
                }
            }
            if(!null)
            {
                text += Q2F(OFFSETS[i],"e",10,2) + tr(",");
                text += Q2F(FREQUENCIES[j],"e",10,2) + tr(",");
                text += Q2F(log10(FREQUENCIES[j]),"e",10,2) + tr(",");
                for(int k=0;k<18;k++)
                {
                    if(datas[i][j][k].datum<-999||datas[i][j][k].error<-999)
                    {
                        text += tr("          ,         ,");
                    }
                    else
                    {
                        text += Q2F(datas[i][j][k].datum,"e",10,2) + tr(",") + Q2F(datas[i][j][k].error,"e",10,2) + tr(",");
                    }
                }
                text.chop(1);
                text += tr("\n");
            }
        }
    }
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

void OccamProject::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}
