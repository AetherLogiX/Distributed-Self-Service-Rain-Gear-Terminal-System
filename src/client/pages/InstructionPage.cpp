/*
    使用说明页面实现
*/
#include "InstructionPage.h"
#include "../assets/Styles.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>

InstructionPage::InstructionPage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void InstructionPage::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);

    // 玻璃卡片容器
    auto *card = new QWidget(this);
    card->setStyleSheet(Styles::pageContainer());
    auto *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 20, 24, 20);
    cardLayout->setSpacing(16);

    // 顶部标题栏
    auto *topBar = new QHBoxLayout();
    auto *title = new QLabel(tr("📖 使用说明与服务协议"), card);
    title->setStyleSheet(Styles::Labels::pageTitle());
    
    auto *btnBack = new QPushButton(tr("返回主页"), card);
    btnBack->setStyleSheet(Styles::Buttons::back());
    btnBack->setCursor(Qt::PointingHandCursor);
    connect(btnBack, &QPushButton::clicked, this, &InstructionPage::backRequested);
    
    topBar->addWidget(title);
    topBar->addStretch();
    topBar->addWidget(btnBack);

    // 内容浏览器
    auto *viewer = new QTextBrowser(card);
    viewer->setOpenExternalLinks(false);
    viewer->setHtml(
        "<div style='font-family: Microsoft YaHei UI, sans-serif; color: #1a1a2e; line-height: 1.8;'>"
        "<h2 style='text-align:center; color:#667eea;'>☂️ NUIST 智能雨具系统服务协议</h2>"
        "<hr style='border: 1px solid #e4e9f2;'>"
        "<h3 style='color:#764ba2;'>一、服务对象</h3>"
        "<p>本系统仅面向 <b>NUIST 在校教职工与学生</b> 开放，登录需验证校园一卡通账户。</p>"
        "<h3 style='color:#764ba2;'>二、借还规则</h3>"
        "<p><b>🔹 借出：</b>账户余额需大于相应押金方可借用。借出时系统将冻结相应金额作为押金。</p>"
        "<p><b>🔹 归还：</b>请将雨具插入任意站点的空闲卡槽，看到屏幕提示【归还成功】后方可离开。</p>"
        "<h3 style='color:#764ba2;'>三、资费标准</h3>"
        "<table style='width:100%; border-collapse:collapse; margin:10px 0;'>"
        "<tr style='background:#f8f9ff;'><td style='padding:8px; border:1px solid #e4e9f2;'><b>雨具类型</b></td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'><b>押金</b></td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'><b>租金</b></td></tr>"
        "<tr><td style='padding:8px; border:1px solid #e4e9f2;'>普通塑料伞</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>10 元</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>1 元/小时</td></tr>"
        "<tr><td style='padding:8px; border:1px solid #e4e9f2;'>高质量抗风伞</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>20 元</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>2 元/小时</td></tr>"
        "<tr><td style='padding:8px; border:1px solid #e4e9f2;'>专用遮阳伞</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>15 元</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>1.5 元/小时</td></tr>"
        "<tr><td style='padding:8px; border:1px solid #e4e9f2;'>雨衣</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>25 元</td>"
        "<td style='padding:8px; border:1px solid #e4e9f2;'>2 元/小时</td></tr>"
        "</table>"
        "<h3 style='color:#764ba2;'>四、遗失与损坏</h3>"
        "<p>若雨具遗失或严重损坏导致无法归还，系统将<b style='color:#ff3d71;'>扣除全额押金</b>用于赔偿。</p>"
        "<h3 style='color:#764ba2;'>五、联系我们</h3>"
        "<p>如遇设备故障或扣费异常，请致电校园服务中心：<b>5873-6110</b></p>"
        "</div>"
    );

    auto *btnConfirm = new QPushButton(tr("✓ 我已阅读并同意"), card);
    btnConfirm->setStyleSheet(Styles::Buttons::primary());
    btnConfirm->setCursor(Qt::PointingHandCursor);
    connect(btnConfirm, &QPushButton::clicked, this, &InstructionPage::backRequested);

    cardLayout->addLayout(topBar);
    cardLayout->addWidget(viewer, 1);
    cardLayout->addWidget(btnConfirm, 0, Qt::AlignCenter);

    layout->addWidget(card);
}

