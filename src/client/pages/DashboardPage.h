/*
    主页/仪表板页面
    用户登录后的主界面，包含站点选择和借还伞入口
*/
#pragma once

#include <QWidget>
#include <memory>
#include "../../Model/User.h"
#include "../../Model/GlobalEnum.hpp"

class QComboBox;
class QLabel;
class StationService;

class DashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit DashboardPage(StationService *stationService, QWidget *parent = nullptr);
    
    void setUser(std::shared_ptr<User> user);
    void refreshStations();  // 刷新站点列表
    int currentStationId() const { return m_currentStationId; }

signals:
    void borrowClicked(int stationId);   // 借伞
    void returnClicked(int stationId);   // 还伞
    void profileClicked();               // 个人中心
    void mapClicked();                   // 站点地图
    void instructionClicked();           // 使用说明
    void logoutClicked();                // 退出登录

private:
    void setupUi();
    void onStationChanged(int index);

    StationService *m_stationService;
    std::shared_ptr<User> m_currentUser;
    
    QComboBox *m_stationComboBox;
    QLabel *m_userInfoLabel;
    int m_currentStationId { 0 };
};

