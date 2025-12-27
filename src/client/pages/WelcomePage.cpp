/*
    欢迎页面实现
*/
#include "WelcomePage.h"
#include "../assets/Styles.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void WelcomePage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(16);

    // 玻璃卡片容器
    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    card->setFixedSize(480, 420);
    
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignCenter);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(40, 50, 40, 50);

    // 图标
    auto *iconLabel = new QLabel(QStringLiteral("☂️"), card);
    iconLabel->setStyleSheet(Styles::Labels::welcomeIcon());
    iconLabel->setAlignment(Qt::AlignCenter);

    // 主标题
    auto *title = new QLabel(tr("NUIST 智能雨具系统"), card);
    title->setStyleSheet(Styles::Labels::title());
    title->setAlignment(Qt::AlignCenter);

    // 副标题
    auto *subtitle = new QLabel(tr("校园智能共享雨具借还平台"), card);
    subtitle->setStyleSheet(Styles::Labels::subtitle());
    subtitle->setAlignment(Qt::AlignCenter);

    // 开始按钮
    auto *btnStart = new QPushButton(tr("开始使用"), card);
    btnStart->setStyleSheet(Styles::Buttons::primaryLarge());
    btnStart->setCursor(Qt::PointingHandCursor);
    connect(btnStart, &QPushButton::clicked, this, &WelcomePage::startClicked);

    cardLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
    cardLayout->addWidget(title, 0, Qt::AlignCenter);
    cardLayout->addWidget(subtitle, 0, Qt::AlignCenter);
    cardLayout->addSpacing(20);
    cardLayout->addWidget(btnStart, 0, Qt::AlignCenter);

    layout->addWidget(card, 0, Qt::AlignCenter);
}

