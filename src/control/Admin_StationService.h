/*
    管理员站点管理服务
*/
#pragma once

#include <QString>
#include <QVector>
#include "../dao/StationDao.h"

//复用DAO层的DTO
using StationStats = StationStatsDTO;

class Admin_StationService {
public:
    QVector<StationStats> getStationStats(); //获取所有站点的统计信息
    double getOnlineRate(); //获取设备在线率
private:
    StationDao stationDao;
};
