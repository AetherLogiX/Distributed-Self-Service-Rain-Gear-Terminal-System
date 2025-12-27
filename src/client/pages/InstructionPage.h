/*
    使用说明页面
    显示服务协议和收费标准
*/
#pragma once

#include <QWidget>

class InstructionPage : public QWidget {
    Q_OBJECT
public:
    explicit InstructionPage(QWidget *parent = nullptr);

signals:
    void backRequested();

private:
    void setupUi();
};

