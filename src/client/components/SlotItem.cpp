/*
    槽位组件实现
*/
#include "SlotItem.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>

SlotItem::SlotItem(int index, QWidget *parent)
    : QWidget(parent)
    , m_index(index)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(4);

    // 图标标签
    m_iconLabel = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedSize(72, 72);
    m_iconLabel->setScaledContents(false);

    // 数字和状态指示器的水平布局
    auto *labelLayout = new QHBoxLayout();
    labelLayout->setContentsMargins(0, 0, 0, 0);
    labelLayout->setSpacing(6);
    labelLayout->setAlignment(Qt::AlignCenter);

    m_label = new QLabel(QStringLiteral("#%1").arg(index + 1), this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setStyleSheet("font-size: 12px; font-weight: 600; color: #333;");

    // 状态指示器（小正方形）
    m_statusIndicator = new QLabel(this);
    m_statusIndicator->setFixedSize(12, 12);
    m_statusIndicator->setStyleSheet("border: 1px solid #999; border-radius: 2px;");

    labelLayout->addWidget(m_label);
    labelLayout->addWidget(m_statusIndicator);

    mainLayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
    mainLayout->addLayout(labelLayout);

    setMinimumSize(120, 120);
    setState(State::Available);
    
    setStyleSheet("SlotItem { background-color: white; border-radius: 8px; border: 1px solid #ddd; }");
}

void SlotItem::setState(State state)
{
    m_state = state;
    refreshStyle();
}

void SlotItem::setIcon(const QPixmap &pixmap, const QString &descText)
{
    if (!pixmap.isNull()) {
        QPixmap scaled = pixmap.scaled(
            m_iconLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        );
        m_iconLabel->setPixmap(scaled);
        m_iconLabel->show();
    } else {
        m_iconLabel->clear();
        m_iconLabel->hide();
    }
    if (!descText.isEmpty()) {
        m_label->setText(descText);
    }
}

void SlotItem::setGearTypeName(const QString &typeName)
{
    QString currentText = m_label->text();
    if (currentText.startsWith("#")) {
        m_label->setText(QStringLiteral("%1\n%2").arg(currentText).arg(typeName));
    } else {
        m_label->setText(QStringLiteral("#%1\n%2").arg(m_index + 1).arg(typeName));
    }
    m_label->setStyleSheet("font-size: 11px; font-weight: 500; color: #333;");
}

void SlotItem::refreshStyle()
{
    QString indicatorColor;
    QString borderColor;
    
    switch (m_state) {
    case State::Available:   
        indicatorColor = "#2ecc71"; // 绿色 - 可借
        borderColor = "#2ecc71";
        break;
    case State::Empty:       
        indicatorColor = "#bdc3c7"; // 灰色 - 空槽/可还
        borderColor = "#bdc3c7";
        break;
    case State::Maintenance: 
        indicatorColor = "#e74c3c"; // 红色 - 故障
        borderColor = "#e74c3c";
        break;
    case State::Selected:    
        indicatorColor = "#f1c40f"; // 黄色 - 选中
        borderColor = "#f1c40f";
        break;
    }

    m_statusIndicator->setStyleSheet(QStringLiteral(
        "QLabel { "
        "background-color: %1; "
        "border: 1px solid rgba(0,0,0,0.2); "
        "border-radius: 2px; "
        "min-width: 12px; min-height: 12px; "
        "max-width: 12px; max-height: 12px; "
        "}").arg(indicatorColor));
    
    setStyleSheet("");
    QApplication::processEvents();
    
    setStyleSheet(QStringLiteral(
        "SlotItem { "
        "background-color: white; "
        "border-radius: 8px; "
        "border: 2px solid %1; "
        "}").arg(borderColor));
    
    style()->unpolish(this);
    style()->polish(this);
    update();
    repaint();
}

void SlotItem::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit clicked(m_index, m_state);
}

