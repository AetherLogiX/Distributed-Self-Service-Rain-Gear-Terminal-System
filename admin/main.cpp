#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include "AdminMainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置Qt插件路径，让Qt能找到sqldrivers
    QString appDir = QCoreApplication::applicationDirPath();
    // Qt期望插件在plugins子目录下
    QString pluginPath1 = QDir(appDir).absoluteFilePath("plugins");
    QString pluginPath2 = QDir(appDir).absoluteFilePath("sqldrivers");
    QCoreApplication::addLibraryPath(pluginPath1);
    QCoreApplication::addLibraryPath(pluginPath2);
    qDebug() << "[AdminMain] Application dir:" << appDir;
    qDebug() << "[AdminMain] Plugin path 1 (plugins):" << pluginPath1 << QDir(pluginPath1).exists();
    qDebug() << "[AdminMain] Plugin path 2 (sqldrivers):" << pluginPath2 << QDir(pluginPath2).exists();
    qDebug() << "[AdminMain] All library paths:" << QCoreApplication::libraryPaths();
    qDebug() << "[AdminMain] Available SQL drivers:" << QSqlDatabase::drivers();
    
    AdminMainWindow w;
    w.show();
    return app.exec();
}
