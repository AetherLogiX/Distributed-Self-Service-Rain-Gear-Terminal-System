-- 插入初始测试数据
-- 插入测试用户
INSERT INTO users (user_id, password, real_name, role, credit, avatar_id) VALUES
('202483290399', '123456', '陈秋雯', 0, 100.00, 1),
('202383290032', '123456', '吴悠', 0, 50.00, 1),
('202483290447', '123456', '刘谨言', 0, 30.00, 1),
('202511420317', '123456', '陈柯安', 0, 100.00, 1),
('T001', '123456', '赵老师', 1, 200.00, 2),
('ADMIN', '123456', '系统管理员', 9, 9999.00, 3)
ON DUPLICATE KEY UPDATE user_id=user_id;

-- 插入站点数据（根据GlobalEnum.hpp中的Station枚举）
-- unavailable_slots字段：空字符串表示无故障槽位，示例"1,5"表示第1和第5号槽位故障
INSERT INTO station (name, pos_x, pos_y, status, unavailable_slots) VALUES
('文德楼', 0.30, 0.40, 1, ''),
('明德楼', 0.35, 0.45, 1, ''),
('图书馆', 0.50, 0.30, 1, ''),
('长望楼', 0.40, 0.35, 1, ''),
('藕舫楼', 0.45, 0.40, 1, ''),
('北辰楼', 0.55, 0.50, 1, ''),
('西苑宿舍楼1', 0.20, 0.70, 1, ''),
('西苑宿舍楼2', 0.25, 0.75, 1, ''),
('西苑宿舍楼3', 0.30, 0.80, 1, ''),
('中苑宿舍楼4', 0.60, 0.65, 1, ''),
('中苑宿舍楼5', 0.65, 0.70, 1, ''),
('中苑宿舍楼6', 0.70, 0.75, 1, ''),
('体育馆', 0.75, 0.60, 1, ''),
('行政楼', 0.50, 0.55, 1, '')
ON DUPLICATE KEY UPDATE name=name;

-- 插入雨具数据（每个站点分配一些雨具）
-- 站点1（文德楼）: 5把普通伞 + 3把抗风伞 + 2件雨衣
INSERT INTO raingear (gear_id, type_id, station_id, slot_id, status) VALUES
('GEAR_001', 1, 1, 1, 1), ('GEAR_002', 1, 1, 2, 1), ('GEAR_003', 1, 1, 3, 1),
('GEAR_004', 1, 1, 4, 1), ('GEAR_005', 1, 1, 5, 1),
('GEAR_006', 2, 1, 6, 1), ('GEAR_007', 2, 1, 7, 1), ('GEAR_008', 2, 1, 8, 1),
('GEAR_009', 4, 1, 9, 1), ('GEAR_010', 4, 1, 10, 1)
ON DUPLICATE KEY UPDATE gear_id=gear_id;

-- 站点2（明德楼）: 4把普通伞 + 2把遮阳伞 + 2件雨衣
INSERT INTO raingear (gear_id, type_id, station_id, slot_id, status) VALUES
('GEAR_011', 1, 2, 1, 1), ('GEAR_012', 1, 2, 2, 1), ('GEAR_013', 1, 2, 3, 1), ('GEAR_014', 1, 2, 4, 1),
('GEAR_015', 3, 2, 5, 1), ('GEAR_016', 3, 2, 6, 1),
('GEAR_017', 4, 2, 7, 1), ('GEAR_018', 4, 2, 8, 1)
ON DUPLICATE KEY UPDATE gear_id=gear_id;

-- 站点3（图书馆）: 8把普通伞 + 4把抗风伞（图书馆需求量大）
INSERT INTO raingear (gear_id, type_id, station_id, slot_id, status) VALUES
('GEAR_019', 1, 3, 1, 1), ('GEAR_020', 1, 3, 2, 1), ('GEAR_021', 1, 3, 3, 1), ('GEAR_022', 1, 3, 4, 1),
('GEAR_023', 1, 3, 5, 1), ('GEAR_024', 1, 3, 6, 1), ('GEAR_025', 1, 3, 7, 1), ('GEAR_026', 1, 3, 8, 1),
('GEAR_027', 2, 3, 9, 1), ('GEAR_028', 2, 3, 10, 1), ('GEAR_029', 2, 3, 11, 1), ('GEAR_030', 2, 3, 12, 1)
ON DUPLICATE KEY UPDATE gear_id=gear_id;


SELECT 'Data initialization completed successfully!' AS message;
SELECT COUNT(*) AS user_count FROM users;
SELECT COUNT(*) AS station_count FROM station;
SELECT COUNT(*) AS gear_count FROM raingear;

