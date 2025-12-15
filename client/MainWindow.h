#pragma once

#include <QMainWindow>
#include <QVector>
#include <memory>

#include "Control/DatabaseManager.h"
#include "Model/User.h"

class QStackedWidget;
class QWidget;
class QPushButton;
class QLabel;
class QLineEdit;
class QTextBrowser;
class SlotItem;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    enum class Page {
        Welcome,
        CardRead,      // 刷卡提示页
        UserInput,     // 手动输入学号姓名页
        FirstLogin,    // 首次登录设置密码页
        Login,         // 密码登录页
        ResetPwd,     // 修改密码页
        Dashboard,
        Borrow,
        Map,
        Profile,
        Instruction
    };

    enum class Role {
        Unknown,
        Student,
        Staff
    };

    void setupUi();
    QWidget* createWelcomePage();
    QWidget* createCardReadPage();      // 刷卡提示页
    QWidget* createUserInputPage();     // 手动输入学号姓名页
    QWidget* createFirstLoginPage();    // 首次登录设置密码页
    QWidget* createLoginPage();         // 密码登录页
    QWidget* createResetPwdPage();
    QWidget* createDashboardPage();
    QWidget* createBorrowPage();
    QWidget* createMapPage();
    QWidget* createProfilePage();
    QWidget* createInstructionPage();
    void switchPage(Page page);
    void populateSlots(bool borrowMode);
    void updateRoleLabel();
    void updateProfileFromUser();
    bool performUserInput();    // 处理学号姓名输入，判断是否首次登录
    bool performFirstLogin();   // 首次登录设置密码
    bool performLogin();         // 密码登录（明文密码）

    QStackedWidget *m_stack { nullptr };
    QVector<SlotItem*> m_slots;
    QLabel *m_slotTitle { nullptr };
    bool m_borrowMode { true };

    // Login/role state
    Role m_currentRole { Role::Unknown };
    QLabel *m_loginRoleLabel { nullptr };
    QLineEdit *m_inputUser { nullptr };      // 学号/工号输入
    QLineEdit *m_inputName { nullptr };      // 姓名输入
    QLineEdit *m_inputPass { nullptr };      // 密码输入
    QLineEdit *m_inputNewPass { nullptr };  // 新密码输入（首次登录）
    QLineEdit *m_inputConfirmPass { nullptr }; // 确认密码输入（首次登录）
    QString m_tempUserId;                    // 临时存储用户ID（用于首次登录）
    QString m_tempUserName;                  // 临时存储用户姓名（用于首次登录）
    std::unique_ptr<User> m_currentUser;

    // Profile mock labels
    QLabel *m_profileName { nullptr };
    QLabel *m_profileId { nullptr };
    QLabel *m_profileBalance { nullptr };
    QLabel *m_profileTitle { nullptr };

    QTextBrowser *m_instructionViewer { nullptr };
};

