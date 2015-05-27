#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if (w.checkSingle()) {
        QMessageBox *pmbx =
                new QMessageBox(QMessageBox::Information,
                                "Locomotion Service",
                                "Application is already running, <br> please check system tray for application icon.",
                                QMessageBox::Ok | QMessageBox::Escape);
        pmbx->exec();
        return -1;
    }

    w.setFixedSize(480, 640);
    w.show();

    return a.exec();
}
