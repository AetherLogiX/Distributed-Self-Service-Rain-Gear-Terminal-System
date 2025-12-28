/*
    管理员用户管理服务
*/
#pragma once

#include <QString>
#include <QVector>

#include "../dao/UserDao.h"
#include "../model/User.h"

class Admin_UserService {
public:
    // 获取所有用户,支持搜索
    QVector<User> getAllUsers(const QString& searchText = "");
    // 重置用户密码
    bool resetUserPassword(const QString& userId, const QString& newPassword);

private:
    UserDao userDao;
};
