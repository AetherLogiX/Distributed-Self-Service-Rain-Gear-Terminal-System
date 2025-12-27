/*
    主窗口实现
*/
#include "MainWindow.h"
#include "assets/Styles.h"

// 页面
#include "pages/WelcomePage.h"
#include "pages/AuthPages.h"
#include "pages/DashboardPage.h"
#include "pages/BorrowPage.h"
#include "pages/MapPage.h"
#include "pages/ProfilePage.h"
#include "pages/InstructionPage.h"

// Service
#include "../control/AuthService.h"
#include "../control/BorrowService.h"
#include "../control/StationService.h"

// DAO 用于刷新用户数据
#include "../dao/UserDao.h"
#include "../utils/ConnectionPool.h"

#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化 Service
    m_authService = std::make_unique<AuthService>();
    m_borrowService = std::make_unique<BorrowService>();
    m_stationService = std::make_unique<StationService>();
    
    // 应用全局样式
    qApp->setStyleSheet(Styles::globalStyle());
    
    setupUi();
    setupConnections();
    
    switchPage(Page::Welcome);
    setWindowTitle(tr("CampusRain - 校园智能共享雨具系统"));
    resize(900, 680);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    auto *central = new QWidget(this);
    central->setObjectName("centralWidget");
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_stack = new QStackedWidget(this);

    // 创建所有页面
    m_welcomePage = new WelcomePage(this);
    m_cardReadPage = new CardReadPage(this);
    m_userInputPage = new UserInputPage(m_authService.get(), this);
    m_firstLoginPage = new FirstLoginPage(m_authService.get(), this);
    m_loginPage = new LoginPage(m_authService.get(), this);
    m_resetPwdPage = new ResetPwdPage(m_authService.get(), this);
    m_dashboardPage = new DashboardPage(m_stationService.get(), this);
    m_borrowPage = new BorrowPage(m_borrowService.get(), m_stationService.get(), this);
    m_mapPage = new MapPage(m_stationService.get(), this);
    m_profilePage = new ProfilePage(this);
    m_instructionPage = new InstructionPage(this);

    // 添加到页面栈（顺序必须与 Page 枚举一致）
    m_stack->addWidget(m_welcomePage);      // 0
    m_stack->addWidget(m_cardReadPage);     // 1
    m_stack->addWidget(m_userInputPage);    // 2
    m_stack->addWidget(m_firstLoginPage);   // 3
    m_stack->addWidget(m_loginPage);        // 4
    m_stack->addWidget(m_resetPwdPage);     // 5
    m_stack->addWidget(m_dashboardPage);    // 6
    m_stack->addWidget(m_borrowPage);       // 7
    m_stack->addWidget(m_mapPage);          // 8
    m_stack->addWidget(m_profilePage);      // 9
    m_stack->addWidget(m_instructionPage);  // 10

    layout->addWidget(m_stack);
    setCentralWidget(central);
}

void MainWindow::setupConnections()
{
    // WelcomePage 信号
    connect(m_welcomePage, &WelcomePage::startClicked, this, [this]() {
        switchPage(Page::CardRead);
    });

    // CardReadPage 信号
    connect(m_cardReadPage, &CardReadPage::confirmed, this, [this]() {
        switchPage(Page::UserInput);
    });
    connect(m_cardReadPage, &CardReadPage::backClicked, this, [this]() {
        switchPage(Page::Welcome);
    });

    // UserInputPage 信号
    connect(m_userInputPage, &UserInputPage::firstLogin, this, [this](const QString &userId, const QString &userName) {
        m_tempUserId = userId;
        m_tempUserName = userName;
        m_firstLoginPage->setUserInfo(userId, userName);
        switchPage(Page::FirstLogin);
    });
    connect(m_userInputPage, &UserInputPage::normalLogin, this, [this](const QString &userId, const QString &userName) {
        m_tempUserId = userId;
        m_tempUserName = userName;
        m_loginPage->setUserInfo(userId, userName);
        switchPage(Page::Login);
    });
    connect(m_userInputPage, &UserInputPage::backClicked, this, [this]() {
        switchPage(Page::CardRead);
    });

    // FirstLoginPage 信号
    connect(m_firstLoginPage, &FirstLoginPage::registerSuccess, this, [this]() {
        m_loginPage->setUserInfo(m_tempUserId, m_tempUserName);
        switchPage(Page::Login);
    });
    connect(m_firstLoginPage, &FirstLoginPage::backClicked, this, [this]() {
        switchPage(Page::UserInput);
    });

    // LoginPage 信号
    connect(m_loginPage, &LoginPage::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_loginPage, &LoginPage::resetPassword, this, [this]() {
        m_resetPwdPage->setUserId(m_tempUserId, m_tempUserName);
        switchPage(Page::ResetPwd);
    });
    connect(m_loginPage, &LoginPage::backClicked, this, [this]() {
        switchPage(Page::UserInput);
    });

    // ResetPwdPage 信号
    connect(m_resetPwdPage, &ResetPwdPage::resetSuccess, this, [this]() {
        m_loginPage->clearInputs();
        switchPage(Page::Login);
    });
    connect(m_resetPwdPage, &ResetPwdPage::backClicked, this, [this]() {
        switchPage(Page::Login);
    });

    // DashboardPage 信号
    connect(m_dashboardPage, &DashboardPage::borrowClicked, this, [this](int stationId) {
        m_borrowPage->setContext(m_currentUser, stationId, true);
        switchPage(Page::Borrow);
    });
    connect(m_dashboardPage, &DashboardPage::returnClicked, this, [this](int stationId) {
        m_borrowPage->setContext(m_currentUser, stationId, false);
        switchPage(Page::Borrow);
    });
    connect(m_dashboardPage, &DashboardPage::profileClicked, this, [this]() {
        m_profilePage->setUser(m_currentUser);
        switchPage(Page::Profile);
    });
    connect(m_dashboardPage, &DashboardPage::mapClicked, this, [this]() {
        switchPage(Page::Map);
    });
    connect(m_dashboardPage, &DashboardPage::instructionClicked, this, [this]() {
        switchPage(Page::Instruction);
    });
    connect(m_dashboardPage, &DashboardPage::logoutClicked, this, &MainWindow::onLogout);

    // BorrowPage 信号
    connect(m_borrowPage, &BorrowPage::backRequested, this, [this]() {
        m_borrowPage->stopAutoRefresh();
        switchPage(Page::Dashboard);
    });
    connect(m_borrowPage, &BorrowPage::operationCompleted, this, [this]() {
        refreshUserData();
    });

    // MapPage 信号
    connect(m_mapPage, &MapPage::backRequested, this, [this]() {
        switchPage(Page::Dashboard);
    });

    // ProfilePage 信号
    connect(m_profilePage, &ProfilePage::backRequested, this, [this]() {
        switchPage(Page::Dashboard);
    });
    connect(m_profilePage, &ProfilePage::refreshClicked, this, [this]() {
        refreshUserData();
        m_profilePage->setUser(m_currentUser);
    });

    // InstructionPage 信号
    connect(m_instructionPage, &InstructionPage::backRequested, this, [this]() {
        switchPage(Page::Dashboard);
    });
}

void MainWindow::switchPage(Page page)
{
    m_stack->setCurrentIndex(static_cast<int>(page));
    
    // 页面切换时的额外处理
    switch (page) {
    case Page::UserInput:
        m_userInputPage->clearInputs();
        break;
    case Page::Borrow:
        m_borrowPage->startAutoRefresh();
        break;
    case Page::Map:
        m_mapPage->refreshMap();
        break;
    default:
        break;
    }
}

void MainWindow::onLoginSuccess(std::shared_ptr<User> user)
{
    m_currentUser = user;
    m_dashboardPage->setUser(user);
    m_dashboardPage->refreshStations();
    
    QMessageBox::information(this, tr("登录成功"), tr("欢迎回来，%1！").arg(user->get_name()));
    switchPage(Page::Dashboard);
}

void MainWindow::onLogout()
{
    m_currentUser.reset();
    m_tempUserId.clear();
    m_tempUserName.clear();
    
    // 清理页面状态
    m_userInputPage->clearInputs();
    m_loginPage->clearInputs();
    m_firstLoginPage->clearInputs();
    m_resetPwdPage->clearInputs();
    m_borrowPage->stopAutoRefresh();
    
    switchPage(Page::Welcome);
}

void MainWindow::refreshUserData()
{
    if (!m_currentUser) return;
    
    QSqlDatabase db = ConnectionPool::getThreadLocalConnection();
    UserDao userDao;
    auto userOpt = userDao.selectById(db, m_currentUser->get_id());
    
    if (userOpt.has_value()) {
        m_currentUser = std::make_shared<User>(
            userOpt->get_id(),
            userOpt->get_name(),
            userOpt->get_password(),
            userOpt->get_role(),
            userOpt->get_credit(),
            userOpt->get_is_active()
        );
    }
}

