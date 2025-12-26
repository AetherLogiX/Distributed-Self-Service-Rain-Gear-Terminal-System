/*
定义了用户类User，用于管理用户信息和借还雨具的操作。
*/
#pragma once

#include <QString>
#include <memory>
#include "RainGear.hpp" 

class User {
public:
    User(QString id, QString name, QString password, int role, double credit, bool is_active);
    ~User() = default;
    //getters
    const QString& get_id() const;
    const QString& get_name() const;
    const QString& get_password() const;
    int get_role() const;
    double get_credit() const;
    bool get_is_active() const;
private:
    QString id;
    QString name;
    QString password;
    int role;
    double credit;  //一卡通余额
    bool is_active; //是否已激活，0:未激活需首次设置密码, 1:已激活
};