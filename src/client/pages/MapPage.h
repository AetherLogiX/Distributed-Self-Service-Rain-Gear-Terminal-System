/*
    地图页面
    显示校园站点分布图
*/
#pragma once

#include <QWidget>

class StationService;

class MapPage : public QWidget {
    Q_OBJECT
public:
    explicit MapPage(StationService *stationService, QWidget *parent = nullptr);
    
    void refreshMap();  // 刷新地图数据

signals:
    void backRequested();

private:
    void setupUi();
    void loadMapStations();

    StationService *m_stationService;
    QWidget *m_mapContainer;
};

