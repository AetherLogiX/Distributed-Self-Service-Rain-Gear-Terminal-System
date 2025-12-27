/*
    个人中心页面实现
*/
#include "ProfilePage.h"
#include "../assets/Styles.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

ProfilePage::ProfilePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void ProfilePage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // 玻璃卡片容器
    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(450, 420);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(16);
    cardLayout->setContentsMargins(40, 40, 40, 40);

    // 头像区域
    auto *avatarLabel = new QLabel(QStringLiteral("👤"), card);
    avatarLabel->setStyleSheet("font-size: 64px;");
    avatarLabel->setAlignment(Qt::AlignCenter);

    auto *titleLabel = new QLabel(tr("个人中心"), card);
    titleLabel->setStyleSheet(Styles::Labels::pageTitle());
    titleLabel->setAlignment(Qt::AlignCenter);
    
    m_nameLabel = new QLabel(card);
    m_nameLabel->setStyleSheet(Styles::Labels::info());
    m_nameLabel->setAlignment(Qt::AlignCenter);
    
    m_idLabel = new QLabel(card);
    m_idLabel->setStyleSheet(Styles::Labels::hint());
    m_idLabel->setAlignment(Qt::AlignCenter);
    
    m_balanceLabel = new QLabel(card);
    m_balanceLabel->setStyleSheet(Styles::Labels::balance());
    m_balanceLabel->setAlignment(Qt::AlignCenter);

    // 按钮区域
    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(16);
    
    auto *btnRefresh = new QPushButton(tr("🔄 刷新余额"), card);
    btnRefresh->setStyleSheet(Styles::Buttons::secondary());
    btnRefresh->setCursor(Qt::PointingHandCursor);
    connect(btnRefresh, &QPushButton::clicked, this, [this]() {
        emit refreshClicked();
        QMessageBox::information(this, tr("提示"), tr("余额已刷新"));
    });
    
    auto *btnBack = new QPushButton(tr("返回主页"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &ProfilePage::backRequested);
    
    btnLayout->addWidget(btnRefresh);
    btnLayout->addWidget(btnBack);

    cardLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(m_nameLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(m_idLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(m_balanceLabel, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addLayout(btnLayout);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

void ProfilePage::setUser(std::shared_ptr<User> user)
{
    m_currentUser = user;
    refreshBalance();
}

void ProfilePage::refreshBalance()
{
    if (!m_currentUser) {
        m_nameLabel->setText(tr("姓名：未登录"));
        m_idLabel->setText(tr("账号：-"));
        m_balanceLabel->setText(tr("￥0.00"));
        m_balanceLabel->setStyleSheet("font-size:28px; font-weight:700; color:#8f8fa3;");
        return;
    }

    bool isStaff = m_currentUser->get_role() == 1;
    m_nameLabel->setText(tr("👋 %1").arg(m_currentUser->get_name()));
    m_idLabel->setText(isStaff ? tr("工号：%1").arg(m_currentUser->get_id())
                               : tr("学号：%1").arg(m_currentUser->get_id()));
    m_balanceLabel->setText(tr("💰 ￥%1").arg(QString::number(m_currentUser->get_credit(), 'f', 2)));
    m_balanceLabel->setStyleSheet(Styles::Labels::balance());
}

