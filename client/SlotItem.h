#pragma once

#include <QWidget>
#include <QPixmap>

class QLabel;

class SlotItem : public QWidget {
    Q_OBJECT
public:
    enum class State {
        Available,   // 可借
        Empty,       // 可还
        Maintenance, // 故障
        Selected
    };

    explicit SlotItem(int index, QWidget *parent = nullptr);
    void setState(State state);
    void setIcon(const QPixmap &pixmap, const QString &descText);
    State state() const { return m_state; }

signals:
    void clicked(int index, SlotItem::State state);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void refreshStyle();

    int m_index;
    State m_state { State::Available };
    QLabel *m_iconLabel;
    QLabel *m_label;
};

