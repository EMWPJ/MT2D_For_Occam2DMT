#include "projectmanager.h"
#include "ui_projectmanager.h"

ProjectManager::ProjectManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectManager)
{
    ui->setupUi(this);
    showMaximized();
    this->setWindowTitle("Occam2D");
    projectsModel = new QStandardItemModel();
    ui->tableView->setModel(projectsModel);
    if(!readConfiguration())
    {
        qDebug()<<"Init Error";
    }
}

bool ProjectManager::readConfiguration()
{
    QString path = QDir::currentPath();
    QFile file(path + "/" + "occam.configuration");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }
    projects.clear();
    QString line;
    QStringList linelist;
    line = file.readLine();
    while(!file.atEnd())
    {
        line = file.readLine();
        linelist = line.split(QRegExp("[, \t\n]"), QString::SkipEmptyParts);
        if(linelist.count()!=6)
        {
            continue;
        }
        OccamProject* temp = new OccamProject();
        temp->setProjectName(linelist[0]);
        temp->setFilePath(linelist[1]);
        temp->setStartUpFileName(linelist[2]);
        temp->setDataFileName(linelist[3]);
        temp->setMeshFileName(linelist[4]);
        temp->setModelFileName(linelist[5]);
        projects<<temp;
        projects.last()->readProject();
    }
    refreshView();
    return true;
}

bool ProjectManager::writeConfiguration()
{
    refreshData();
    QString path = QDir::currentPath();
    QFile file(path + "/" + "occam.configuration");
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.close();
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return false;
    }
    QString text;
    int count = projects.count();
    if(count==0)
    {
        text = tr("None");
    }
    else
    {
        text = tr("Projects:\n");
        for(int i=0;i<count;i++)
        {
            text += projects[i]->ProjectName() + "\t"
                    + projects[i]->FilePath() + "\t"
                    + projects[i]->StartUpFileName() + "\t"
                    + projects[i]->DataFileName() + "\t"
                    + projects[i]->MeshFileName() + "\t"
                    + projects[i]->ModelFileName() + "\n";
        }
    }
    file.write(text.toLatin1().data());
    file.close();
    return true;
}

ProjectManager::~ProjectManager()
{
    delete ui;
}

void ProjectManager::refreshView()
{
    int count = projects.count();
    projectsModel->clear();
    projectsModel->setColumnCount(6);
    projectsModel->setColumnCount(count);
    projectsModel->setHorizontalHeaderLabels(QStringList()<<tr("Project Name")
                                             <<tr("Project Path")<<tr("Startup Files")
                                             <<tr("Data File")<<tr("Mesh Files")<<tr("Model Files"));
    for(int i=0;i<count;i++)
    {
        QStandardItem *temp1 = new QStandardItem(projects[i]->ProjectName());
        QStandardItem *temp2 = new QStandardItem(projects[i]->FilePath());
        QStandardItem *temp3 = new QStandardItem(projects[i]->StartUpFileName());
        QStandardItem *temp4 = new QStandardItem(projects[i]->DataFileName());
        QStandardItem *temp5 = new QStandardItem(projects[i]->MeshFileName());
        QStandardItem *temp6 = new QStandardItem(projects[i]->ModelFileName());
        projectsModel->setItem(i,0,temp1);
        projectsModel->setItem(i,1,temp2);
        projectsModel->setItem(i,2,temp3);
        projectsModel->setItem(i,3,temp4);
        projectsModel->setItem(i,4,temp5);
        projectsModel->setItem(i,5,temp6);
    }
    return;
}

void ProjectManager::refreshData()
{
    projects.clear();
    int count = projectsModel->rowCount();
    for(int i=0;i<count;i++)
    {
        QModelIndex index = projectsModel->index(i,0);
        OccamProject* temp = new OccamProject();
        temp->setProjectName(index.data().toString());
        index = projectsModel->index(i,1);
        temp->setFilePath(index.data().toString());
        index = projectsModel->index(i,2);
        temp->setStartUpFileName(index.data().toString());
        index = projectsModel->index(i,3);
        temp->setDataFileName(index.data().toString());
        index = projectsModel->index(i,4);
        temp->setMeshFileName(index.data().toString());
        index = projectsModel->index(i,5);
        temp->setModelFileName(index.data().toString());
        projects<<temp;
        projects.last()->readProject();
    }
    return;
}

void ProjectManager::on_pushButton_new_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New Project"),
                                               tr("Project name:"), QLineEdit::Normal,
                                               tr("Project"), &ok);
    if (ok && !text.isEmpty())
    {
        OccamProject* newProject = new OccamProject();
        projects.push_front(newProject);
        projects[0]->readProject(QDir::currentPath() + tr("/newProject/new.iter"));
        newProject->setProjectName(text);
        projects[0]->show();
        refreshView();
    }
    return;
}

void ProjectManager::on_pushButton_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Startup File"),
                                                    QDir::currentPath(),
                                                    tr("Startup (*.iter);;All (*)"));
    int count = projects.count();
    for(int i=0;i<count;i++)
    {
        if(filename == projects[i]->FilePath() + "/" + projects[i]->StartUpFileName())
        {
            projects[i]->show();
            return;
        }
    }
    OccamProject* newProject = new OccamProject();
    newProject->readProject(filename);
    newProject->setProjectName(newProject->StartUpFileName());
    projects.push_front(newProject);
    projects[0]->show();
    refreshView();
    return;
}

void ProjectManager::on_tableView_doubleClicked(const QModelIndex &index)
{
    projects[index.row()]->show();
}

void ProjectManager::closeEvent(QCloseEvent *e)
{
    writeConfiguration();
    e->accept();
}
