#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>
#include <QFileDialog>
#include <QFile>
#include <QInputDialog>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QCloseEvent>
#include <QDebug>

#include "occamproject.h"

namespace Ui {
class ProjectManager;
}

class ProjectManager : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManager(QWidget *parent = 0);
    ~ProjectManager();

private slots:
    void on_pushButton_new_clicked();

    void on_pushButton_open_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::ProjectManager *ui;
    QStandardItemModel* projectsModel;
    QVector < OccamProject* > projects;

private:
    bool readConfiguration();
    bool writeConfiguration();
    void refreshView();
    void refreshData();

protected:
    void closeEvent(QCloseEvent *e);
};

#endif // PROJECTMANAGER_H
