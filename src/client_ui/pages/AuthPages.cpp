/*
    认证相关页面实现
*/
#include "AuthPages.h"
#include "../assets/Styles.h"
#include "../../control/AuthService.h"
#include "../../dao/UserDao.h"
#include "../../utils/ConnectionPool.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

//CardReadPage实现
CardReadPage::CardReadPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void CardReadPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(500, 450);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(40, 40, 40, 40);

    auto *iconLabel = new QLabel(QStringLiteral("💳"), card);
    iconLabel->setStyleSheet(Styles::Labels::welcomeIcon());
    iconLabel->setAlignment(Qt::AlignCenter);

    auto *tip = new QLabel(tr("请将您的一卡通放置在刷卡处"), card);
    tip->setStyleSheet(Styles::Labels::pageTitle());
    tip->setAlignment(Qt::AlignCenter);

    auto *subtip = new QLabel(tr("系统将自动识别您的学号和姓名"), card);
    subtip->setStyleSheet(Styles::Labels::subtitle());
    subtip->setAlignment(Qt::AlignCenter);

    auto *hintLabel = new QLabel(tr("（演示模式：点击确定手动输入信息）"), card);
    hintLabel->setStyleSheet(Styles::Labels::hint());
    hintLabel->setAlignment(Qt::AlignCenter);

    auto *btnConfirm = new QPushButton(tr("确定"), card);
    btnConfirm->setStyleSheet(Styles::Buttons::primary());
    btnConfirm->setCursor(Qt::PointingHandCursor);
    connect(btnConfirm, &QPushButton::clicked, this, &CardReadPage::confirmed);

    auto *btnBack = new QPushButton(tr("返回首页"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &CardReadPage::backClicked);

    cardLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(tip, 0, Qt::AlignCenter);
    cardLayout->addWidget(subtip, 0, Qt::AlignCenter);
    cardLayout->addWidget(hintLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnConfirm, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(btnBack, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

//UserInputPage实现
UserInputPage::UserInputPage(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , m_authService(authService)
{
    setupUi();
}

void UserInputPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(480, 450);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(50, 40, 50, 40);

    auto *title = new QLabel(tr("请输入您的信息"), card);
    title->setStyleSheet(Styles::Labels::pageTitle());
    title->setAlignment(Qt::AlignCenter);

    auto *subtitle = new QLabel(tr("演示模式：请手动输入学号/工号和姓名"), card);
    subtitle->setStyleSheet(Styles::Labels::hint());
    subtitle->setAlignment(Qt::AlignCenter);

    m_inputUser = new QLineEdit(card);
    m_inputUser->setPlaceholderText(tr("请输入学号/工号"));
    m_inputUser->setFixedWidth(320);

    m_inputName = new QLineEdit(card);
    m_inputName->setPlaceholderText(tr("请输入姓名"));
    m_inputName->setFixedWidth(320);

    auto *btnSubmit = new QPushButton(tr("提交验证"), card);
    btnSubmit->setStyleSheet(Styles::Buttons::primary());
    btnSubmit->setCursor(Qt::PointingHandCursor);
    connect(btnSubmit, &QPushButton::clicked, this, &UserInputPage::onSubmit);

    auto *btnBack = new QPushButton(tr("返回"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &UserInputPage::backClicked);

    cardLayout->addWidget(title, 0, Qt::AlignCenter);
    cardLayout->addWidget(subtitle, 0, Qt::AlignCenter);
    cardLayout->addSpacing(16);
    cardLayout->addWidget(m_inputUser, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_inputName, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnSubmit, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(btnBack, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

void UserInputPage::clearInputs()
{
    if (m_inputUser) m_inputUser->clear();
    if (m_inputName) m_inputName->clear();
}

void UserInputPage::onSubmit()
{
    QString userId = m_inputUser->text().trimmed();
    QString userName = m_inputName->text().trimmed();

    if (userId.isEmpty() || userName.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入学号/工号和姓名"));
        return;
    }

    auto status = m_authService->checkLogin(userId, userName);

    switch (status) {
    case AuthService::LoginStatus::SuccessFirstTime:
        emit firstLogin(userId, userName);
        break;
    case AuthService::LoginStatus::SuccessNormal:
        emit normalLogin(userId, userName);
        break;
    case AuthService::LoginStatus::UserNotFound:
        QMessageBox::warning(this, tr("用户不存在"), tr("未找到该学号/工号对应的用户，请检查输入。"));
        break;
    case AuthService::LoginStatus::NameMismatch:
        QMessageBox::warning(this, tr("姓名不匹配"), tr("姓名与学号/工号不匹配，请检查输入。"));
        break;
    case AuthService::LoginStatus::DatabaseError:
        QMessageBox::critical(this, tr("数据库错误"), tr("无法连接到数据库，请检查服务是否已启动。"));
        break;
    case AuthService::LoginStatus::AdminNotAllowed:
        QMessageBox::warning(this, tr("权限错误"), tr("管理员账号请使用管理员后台登录，不能在客户端登录。"));
        break;
    }
}

//FirstLoginPage实现
FirstLoginPage::FirstLoginPage(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , m_authService(authService)
{
    setupUi();
}

void FirstLoginPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(480, 520);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(16);
    cardLayout->setContentsMargins(50, 40, 50, 40);

    auto *title = new QLabel(tr("🎉 首次登录"), card);
    title->setStyleSheet(Styles::Labels::pageTitle());
    title->setAlignment(Qt::AlignCenter);

    auto *subtitle = new QLabel(tr("欢迎加入！请设置您的登录密码"), card);
    subtitle->setStyleSheet(Styles::Labels::subtitle());
    subtitle->setAlignment(Qt::AlignCenter);

    m_userInfoLabel = new QLabel(card);
    m_userInfoLabel->setStyleSheet(Styles::Labels::info());
    m_userInfoLabel->setAlignment(Qt::AlignCenter);

    m_inputNewPass = new QLineEdit(card);
    m_inputNewPass->setPlaceholderText(tr("请输入新密码（至少6位）"));
    m_inputNewPass->setEchoMode(QLineEdit::Password);
    m_inputNewPass->setFixedWidth(320);

    m_inputConfirmPass = new QLineEdit(card);
    m_inputConfirmPass->setPlaceholderText(tr("请再次确认密码"));
    m_inputConfirmPass->setEchoMode(QLineEdit::Password);
    m_inputConfirmPass->setFixedWidth(320);

    auto *btnSubmit = new QPushButton(tr("完成注册"), card);
    btnSubmit->setStyleSheet(Styles::Buttons::accent());
    btnSubmit->setCursor(Qt::PointingHandCursor);
    connect(btnSubmit, &QPushButton::clicked, this, &FirstLoginPage::onSubmit);

    auto *btnBack = new QPushButton(tr("返回"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &FirstLoginPage::backClicked);

    cardLayout->addWidget(title, 0, Qt::AlignCenter);
    cardLayout->addWidget(subtitle, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(m_userInfoLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(12);
    cardLayout->addWidget(m_inputNewPass, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_inputConfirmPass, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnSubmit, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(btnBack, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

void FirstLoginPage::setUserInfo(const QString &userId, const QString &userName)
{
    m_userId = userId;
    m_userName = userName;
    m_userInfoLabel->setText(tr("账号：%1 | 姓名：%2").arg(userId).arg(userName));
}

void FirstLoginPage::clearInputs()
{
    if (m_inputNewPass) m_inputNewPass->clear();
    if (m_inputConfirmPass) m_inputConfirmPass->clear();
}

void FirstLoginPage::onSubmit()
{
    QString newPass = m_inputNewPass->text();
    QString confirmPass = m_inputConfirmPass->text();

    if (newPass.isEmpty() || confirmPass.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入新密码和确认密码"));
        return;
    }

    if (newPass != confirmPass) {
        QMessageBox::warning(this, tr("密码不一致"), tr("两次输入的密码不一致，请重新输入。"));
        return;
    }

    if (newPass.length() < 6) {
        QMessageBox::warning(this, tr("密码太短"), tr("密码长度至少为6位，请重新输入。"));
        return;
    }

    if (m_authService->activateUser(m_userId, m_userName, newPass)) {
        clearInputs();
        QMessageBox::information(this, tr("注册成功"), tr("密码设置成功！请使用新密码登录。"));
        emit registerSuccess();
    } else {
        QMessageBox::critical(this, tr("设置失败"), tr("密码设置失败，请重试。"));
    }
}

//LoginPage实现
LoginPage::LoginPage(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , m_authService(authService)
{
    setupUi();
}

void LoginPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(480, 550);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(12);
    cardLayout->setContentsMargins(50, 40, 50, 40);

    auto *title = new QLabel(tr("🔐 账号登录"), card);
    title->setStyleSheet(Styles::Labels::pageTitle());
    title->setAlignment(Qt::AlignCenter);

    m_userInfoLabel = new QLabel(card);
    m_userInfoLabel->setStyleSheet(Styles::Labels::info());
    m_userInfoLabel->setAlignment(Qt::AlignCenter);

    m_inputPass = new QLineEdit(card);
    m_inputPass->setEchoMode(QLineEdit::Password);
    m_inputPass->setPlaceholderText(tr("请输入密码"));
    m_inputPass->setFixedWidth(320);

    auto *btnLogin = new QPushButton(tr("登录"), card);
    btnLogin->setStyleSheet(Styles::Buttons::primary());
    btnLogin->setCursor(Qt::PointingHandCursor);
    connect(btnLogin, &QPushButton::clicked, this, &LoginPage::onLogin);

    // 忘记密码按钮
    auto *btnForgot = new QPushButton(tr("忘记密码"), card);
    btnForgot->setStyleSheet(Styles::Buttons::link());
    btnForgot->setCursor(Qt::PointingHandCursor);
    connect(btnForgot, &QPushButton::clicked, this, &LoginPage::onForgotPassword);

    // 修改密码按钮
    auto *btnChange = new QPushButton(tr("修改密码"), card);
    btnChange->setStyleSheet(Styles::Buttons::link());
    btnChange->setCursor(Qt::PointingHandCursor);
    connect(btnChange, &QPushButton::clicked, this, &LoginPage::onChangePassword);

    // 将忘记密码和修改密码放在同一行
    auto *passwordLinksLayout = new QHBoxLayout();
    passwordLinksLayout->setAlignment(Qt::AlignCenter);
    passwordLinksLayout->setSpacing(16);
    passwordLinksLayout->addWidget(btnForgot);
    passwordLinksLayout->addWidget(btnChange);
    auto *passwordLinksWidget = new QWidget(card);
    passwordLinksWidget->setLayout(passwordLinksLayout);

    auto *btnBack = new QPushButton(tr("返回"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &LoginPage::backClicked);

    cardLayout->addWidget(title, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(m_userInfoLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(16);
    cardLayout->addWidget(m_inputPass, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnLogin, 0, Qt::AlignCenter);
    cardLayout->addSpacing(12);
    cardLayout->addWidget(passwordLinksWidget, 0, Qt::AlignCenter);
    cardLayout->addSpacing(16);
    cardLayout->addWidget(btnBack, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

void LoginPage::setUserInfo(const QString &userId, const QString &userName)
{
    m_userId = userId;
    m_userName = userName;
    m_userInfoLabel->setText(tr("账号：%1 | 姓名：%2").arg(userId).arg(userName));
}

void LoginPage::clearInputs()
{
    if (m_inputPass) m_inputPass->clear();
}

void LoginPage::onLogin()
{
    QString password = m_inputPass->text();

    if (password.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入密码"));
        return;
    }

    if (!m_authService->verifyPassword(m_userId, password)) {
        QMessageBox::warning(this, tr("登录失败"), tr("密码错误，请检查输入。"));
        return;
    }

    // 从数据库获取完整用户信息
    QSqlDatabase db = ConnectionPool::getThreadLocalConnection();
    UserDao userDao;
    auto userOpt = userDao.selectById(db, m_userId);
    
    if (userOpt.has_value()) {
        auto user = std::make_shared<User>(
            userOpt->get_id(),
            userOpt->get_name(),
            userOpt->get_password(),
            userOpt->get_role(),
            userOpt->get_credit(),
            userOpt->get_is_active()
        );
        
        // 检查是否是管理员
        if (user->get_role() == 9) {
            QMessageBox::warning(this, tr("权限错误"), tr("管理员账号请使用管理员后台登录。"));
            return;
        }
        
        clearInputs();
        emit loginSuccess(user);
    } else {
        QMessageBox::critical(this, tr("错误"), tr("获取用户信息失败"));
    }
}

void LoginPage::onForgotPassword()
{
    QMessageBox::information(this, tr("忘记密码"), tr("请联系管理员进行密码重置。"));
}

void LoginPage::onChangePassword()
{
    emit changePassword();
}

//ResetPwdPage实现
ResetPwdPage::ResetPwdPage(AuthService *authService, QWidget *parent)
    : QWidget(parent)
    , m_authService(authService)
{
    setupUi();
}

void ResetPwdPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(480, 500);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(14);
    cardLayout->setContentsMargins(50, 40, 50, 40);

    auto *title = new QLabel(tr("🔑 修改密码"), card);
    title->setStyleSheet(Styles::Labels::pageTitle());
    title->setAlignment(Qt::AlignCenter);

    m_inputOld = new QLineEdit(card);
    m_inputOld->setPlaceholderText(tr("请输入旧密码"));
    m_inputOld->setEchoMode(QLineEdit::Password);
    m_inputOld->setFixedWidth(320);

    m_inputNew = new QLineEdit(card);
    m_inputNew->setPlaceholderText(tr("请输入新密码（至少6位）"));
    m_inputNew->setEchoMode(QLineEdit::Password);
    m_inputNew->setFixedWidth(320);

    m_inputConfirm = new QLineEdit(card);
    m_inputConfirm->setPlaceholderText(tr("请再次确认新密码"));
    m_inputConfirm->setEchoMode(QLineEdit::Password);
    m_inputConfirm->setFixedWidth(320);

    auto *btnSubmit = new QPushButton(tr("确认修改"), card);
    btnSubmit->setStyleSheet(Styles::Buttons::primary());
    btnSubmit->setCursor(Qt::PointingHandCursor);
    connect(btnSubmit, &QPushButton::clicked, this, &ResetPwdPage::onSubmit);

    auto *btnBack = new QPushButton(tr("返回"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &ResetPwdPage::backClicked);

    cardLayout->addWidget(title, 0, Qt::AlignCenter);
    cardLayout->addSpacing(16);
    cardLayout->addWidget(m_inputOld, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_inputNew, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_inputConfirm, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnSubmit, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(btnBack, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

void ResetPwdPage::setUserId(const QString &userId, const QString &userName)
{
    m_userId = userId;
    m_userName = userName;
}

void ResetPwdPage::clearInputs()
{
    if (m_inputOld) m_inputOld->clear();
    if (m_inputNew) m_inputNew->clear();
    if (m_inputConfirm) m_inputConfirm->clear();
}

void ResetPwdPage::onSubmit()
{
    QString oldPass = m_inputOld->text();
    QString newPass = m_inputNew->text();
    QString confirmPass = m_inputConfirm->text();

    if (oldPass.isEmpty() || newPass.isEmpty() || confirmPass.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请填写所有密码字段"));
        return;
    }

    if (newPass != confirmPass) {
        QMessageBox::warning(this, tr("密码不一致"), tr("两次输入的新密码不一致，请重新输入。"));
        return;
    }

    if (newPass.length() < 6) {
        QMessageBox::warning(this, tr("密码太短"), tr("密码长度至少为6位，请重新输入。"));
        return;
    }

    // 先验证旧密码
    if (!m_authService->verifyPassword(m_userId, oldPass)) {
        QMessageBox::warning(this, tr("修改失败"), tr("旧密码错误，请重试。"));
        return;
    }

    // 修改密码
    if (m_authService->activateUser(m_userId, m_userName, newPass)) {
        clearInputs();
        QMessageBox::information(this, tr("修改成功"), tr("密码已成功修改，请使用新密码重新登录。"));
        emit resetSuccess();
    } else {
        QMessageBox::critical(this, tr("修改失败"), tr("密码修改失败，请重试。"));
    }
}

