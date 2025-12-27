/*
    槽位组件 - 借伞/还伞界面中的单个槽位显示
    纯展示组件，不包含业务逻辑
*/
#pragma once

#include <QWidget>
#include <QPixmap>

class QLabel;

class SlotItem : public QWidget {
    Q_OBJECT
public:
    enum class State {
        Available,   // 可借（绿色）
        Empty,       // 空槽可还（灰色）
        Maintenance, // 故障（红色）
        Selected     // 选中（黄色）
    };

    explicit SlotItem(int index, QWidget *parent = nullptr);
    
    void setState(State state);
    void setIcon(const QPixmap &pixmap, const QString &descText);
    void setGearTypeName(const QString &typeName);
    State state() const { return m_state; }
    int index() const { return m_index; }

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
    QLabel *m_statusIndicator;
};

