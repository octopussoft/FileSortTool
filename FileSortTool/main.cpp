#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("SortTool");
    a.setOrganizationName("octopusSoft");
    a.setOrganizationDomain("octopussoft.de");
    a.setApplicationVersion("0.1.0");

    MainWindow w;
    w.setWindowIcon(QIcon("://icons/octopus.ico"));
    w.setWindowTitle(QApplication::applicationName() + " " + QApplication::applicationVersion());
    w.show();

    return a.exec();
}
