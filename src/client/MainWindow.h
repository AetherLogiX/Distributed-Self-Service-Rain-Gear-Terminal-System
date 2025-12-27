/*
    主窗口 - 页面调度器
    负责管理所有页面的切换和Service的持有
*/
#pragma once

#include <QMainWindow>
#include <memory>
#include "../Model/User.h"

class QStackedWidget;
class AuthService;
class BorrowService;
class StationService;

// 前向声明页面类
class WelcomePage;
class CardReadPage;
class UserInputPage;
class FirstLoginPage;
class LoginPage;
class ResetPwdPage;
class DashboardPage;
class BorrowPage;
class MapPage;
class ProfilePage;
class InstructionPage;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // 页面索引枚举
    enum class Page {
        Welcome = 0,
        CardRead,
        UserInput,
        FirstLogin,
        Login,
        ResetPwd,
        Dashboard,
        Borrow,
        Map,
        Profile,
        Instruction
    };

    void setupUi();
    void setupConnections();
    void switchPage(Page page);
    
    // 登录成功回调
    void onLoginSuccess(std::shared_ptr<User> user);
    // 退出登录
    void onLogout();
    // 刷新用户数据
    void refreshUserData();

    // 页面切换栈
    QStackedWidget *m_stack { nullptr };

    // Service 层（持有所有权）
    std::unique_ptr<AuthService> m_authService;
    std::unique_ptr<BorrowService> m_borrowService;
    std::unique_ptr<StationService> m_stationService;

    // 当前登录用户
    std::shared_ptr<User> m_currentUser;
    
    // 临时存储（用于登录流程）
    QString m_tempUserId;
    QString m_tempUserName;

    // 页面指针（不持有所有权，由 QStackedWidget 管理）
    WelcomePage *m_welcomePage { nullptr };
    CardReadPage *m_cardReadPage { nullptr };
    UserInputPage *m_userInputPage { nullptr };
    FirstLoginPage *m_firstLoginPage { nullptr };
    LoginPage *m_loginPage { nullptr };
    ResetPwdPage *m_resetPwdPage { nullptr };
    DashboardPage *m_dashboardPage { nullptr };
    BorrowPage *m_borrowPage { nullptr };
    MapPage *m_mapPage { nullptr };
    ProfilePage *m_profilePage { nullptr };
    InstructionPage *m_instructionPage { nullptr };
};

