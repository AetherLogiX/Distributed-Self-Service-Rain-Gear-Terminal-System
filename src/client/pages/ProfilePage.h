/*
    个人中心页面
    显示用户信息和余额
*/
#pragma once

#include <QWidget>
#include <memory>
#include "../../Model/User.h"

class QLabel;

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    explicit ProfilePage(QWidget *parent = nullptr);
    
    void setUser(std::shared_ptr<User> user);
    void refreshBalance();  // 刷新余额显示

signals:
    void backRequested();
    void refreshClicked();  // 请求刷新用户数据

private:
    void setupUi();

    std::shared_ptr<User> m_currentUser;
    QLabel *m_nameLabel;
    QLabel *m_idLabel;
    QLabel *m_balanceLabel;
};

