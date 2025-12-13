
#include <QList>
#include <QString>
#include <memory>

#include "GlobalEnum.hpp"
#include "RainGear.h"
#include "RainGearFactory.h"
#include "RainGear_subclasses.hpp"
#include "StationUtils.h"
#include "Stationlocal.h"

Stationlocal::Stationlocal(Station station, double posX, double posY):station(station),posX(posX),posY(posY){}


//判断雨具是否损坏，是否可用
bool Stationlocal::is_gear_available(int index) const{
    if(index < 0 || index >= N) return false;
    if(!inventory[index]||inventory[index]->broken_flag) return false;  
    return true;
}


//库存总数量与可用数量
int get_inventory_count() const{
    int count = 0;
    for(auto &gear : inventory){
        if(gear) count++;
    }
    return count; 
}

int get_available_count() const{
    int count = 0;
    for(auto &gear : inventory){
        if(gear && !gear->broken_flag) count++;
    }
    return count;
}

//服务器管理员权限
void add_gear(int index, std::unique_ptr<RainGear> gear){
    if(index < 0 || index >= N || !gear) return;
    inventory[index] = std::move(gear);
} //添加雨具到库存

std::unique_ptr<RainGear> take_gear(int index);{
    if(index < 0 || index >= N || !inventory[index]) return nullptr;
    return std::move(inventory[index]);
} //从库存中取出雨具

void mark_unavailable(int index){
    if(index < 0 || index >= N || !inventory[index]) return;
    inventory[index]->broken_flag = true;
    unavailable_gears.insert(index);
}//标记雨具不可用

void mark_available(int index){
    if(index < 0 || index >= N || !inventory[index]) return;
    inventory[index]->broken_flag = false;
    unavailable_gears.remove(index);
} //标记雨具可用