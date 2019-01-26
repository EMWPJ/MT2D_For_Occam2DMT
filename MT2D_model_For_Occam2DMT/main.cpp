#include "projectmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjectManager w;
    w.show();

    return a.exec();
}
