/*
    欢迎页面 - 系统首页
*/
#pragma once

#include <QWidget>

class WelcomePage : public QWidget {
    Q_OBJECT
public:
    explicit WelcomePage(QWidget *parent = nullptr);

signals:
    void startClicked();  // 点击开始按钮

private:
    void setupUi();
};

