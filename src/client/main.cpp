/*
    客户端入口
*/
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置Qt插件路径
    QString appDir = QCoreApplication::applicationDirPath();
    QString pluginPath1 = QDir(appDir).absoluteFilePath("plugins");
    QString pluginPath2 = QDir(appDir).absoluteFilePath("sqldrivers");
    QCoreApplication::addLibraryPath(pluginPath1);
    QCoreApplication::addLibraryPath(pluginPath2);
    
    qDebug() << "[Main] Application dir:" << appDir;
    qDebug() << "[Main] Available SQL drivers:" << QSqlDatabase::drivers();
    
    MainWindow w;
    w.show();
    return app.exec();
}

