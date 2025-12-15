#include "SlotItem.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPixmap>

SlotItem::SlotItem(int index, QWidget *parent)
    : QWidget(parent)
    , m_index(index)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setAlignment(Qt::AlignCenter);
    m_iconLabel->setFixedSize(72, 72);
    m_iconLabel->setScaledContents(true);

    m_label = new QLabel(QStringLiteral("#%1").arg(index + 1), this);
    m_label->setAlignment(Qt::AlignCenter);

    layout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
    layout->addWidget(m_label, 0, Qt::AlignCenter);

    setMinimumSize(120, 120);
    setState(State::Available);
}

void SlotItem::setState(State state)
{
    m_state = state;
    refreshStyle();
}

void SlotItem::setIcon(const QPixmap &pixmap, const QString &descText)
{
    if (!pixmap.isNull()) {
        m_iconLabel->setPixmap(pixmap.scaled(m_iconLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    if (!descText.isEmpty()) {
        m_label->setText(descText);
    }
}

void SlotItem::refreshStyle()
{
    QString bgColor;
    switch (m_state) {
    case State::Available:   bgColor = "#2ecc71"; break; // green
    case State::Empty:       bgColor = "#bdc3c7"; break; // gray
    case State::Maintenance: bgColor = "#e74c3c"; break; // red
    case State::Selected:    bgColor = "#f1c40f"; break; // yellow
    }

    setStyleSheet(QStringLiteral(
        "SlotItem { border-radius: 10px; background: %1; border: 1px solid rgba(0,0,0,0.08);} "
        "QLabel { color: #111; font-weight: 600; }").arg(bgColor));
}

void SlotItem::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit clicked(m_index, m_state);
}

