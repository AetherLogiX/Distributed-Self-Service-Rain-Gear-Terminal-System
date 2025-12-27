/*
    借还伞页面
    显示12个槽位，处理借伞和还伞操作
*/
#pragma once

#include <QWidget>
#include <QVector>
#include <memory>
#include "../../Model/User.h"
#include "../../Model/GlobalEnum.hpp"

class QLabel;
class QTimer;
class SlotItem;
class BorrowService;
class StationService;

class BorrowPage : public QWidget {
    Q_OBJECT
public:
    explicit BorrowPage(BorrowService *borrowService, StationService *stationService, QWidget *parent = nullptr);
    ~BorrowPage();
    
    // 设置上下文
    void setContext(std::shared_ptr<User> user, int stationId, bool isBorrowMode);
    void refreshSlots();  // 刷新槽位状态
    void startAutoRefresh();  // 开始自动刷新
    void stopAutoRefresh();   // 停止自动刷新

signals:
    void backRequested();
    void operationCompleted();  // 借还操作完成，通知刷新用户余额

private:
    void setupUi();
    void onSlotClicked(int slotIndex);
    void handleBorrow(int slotId);
    void handleReturn(int slotId);

    BorrowService *m_borrowService;
    StationService *m_stationService;
    
    std::shared_ptr<User> m_currentUser;
    int m_currentStationId { 0 };
    bool m_isBorrowMode { true };
    
    QVector<SlotItem*> m_slots;
    QLabel *m_titleLabel;
    QTimer *m_refreshTimer;
};

