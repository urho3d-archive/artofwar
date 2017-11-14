BEGIN TRANSACTION;
INSERT INTO `units` (id,name,minDist,maxSep,type,model,texture,font,mass,maxSpeed,scale,rotatable,nation,icon,actionState) VALUES (0,'warrior',0.5,2.0,0,'Cube.mdl','grey.xml','Anonymous Pro.ttf',1.0,5.0,0.5,1,0,'warrior.png',3),
 (1,'archer',0.4,2.0,3,'Hedra.mdl','darkgrey.xml','Anonymous Pro.ttf',0.8,6.0,0.4,1,0,'archer.png',3),
 (2,'pikeman',0.4,2.0,1,'Pyramid.mdl','red.xml','Anonymous Pro.ttf',0.9,3.0,0.4,1,0,'pikeman.png',6),
 (3,'cavalry',0.6,2.0,2,'horse.mdl','horse.xml','Anonymous Pro.ttf',2.0,12.0,0.6,1,0,'cavalry.png',3),
 (4,'worker',0.3,2.0,4,'Sphere.mdl','red.xml','Anonymous Pro.ttf',0.5,3.0,0.3,0,0,'worker.png',9),
 (5,'general',0.5,2.0,0,'general.mdl','general.xml','Anonymous Pro.ttf',1.0,7.0,0.5,1,0,'general.png',1);
INSERT INTO `unit_type` (id,name) VALUES (0,'warrior'),
 (1,'pikeman'),
 (2,'cavalry'),
 (3,'archer'),
 (4,'worker');
INSERT INTO `resource` (id,name,icon,maxCapacity,texture,model,scale,sizeX,sizeZ,maxUsers) VALUES (0,'gold','gold.png',1000,'gold.xml
','rock.mdl',0.5,2,2,4),
 (1,'wood','wood.png',100,'tree0.xml
tree1.xml','tree.mdl',1.0,3,3,3),
 (2,'food','food.png',100,'food.xml',NULL,0.5,2,2,3),
 (3,'stone','stone.png',1000,'stone.xml','rock.mdl',0.5,2,2,4);
INSERT INTO `orders_to_unit` (id,unit,order) VALUES (0,0,0),
 (1,0,1),
 (2,0,3),
 (3,0,4),
 (4,0,5),
 (5,0,6),
 (6,1,0),
 (7,1,1),
 (8,1,3),
 (9,1,4),
 (10,1,5),
 (11,2,0),
 (12,2,1),
 (13,2,3),
 (14,2,4),
 (15,2,5),
 (16,2,6),
 (17,3,0),
 (18,3,1),
 (19,3,2),
 (20,3,3),
 (21,3,4),
 (22,4,0),
 (23,4,1),
 (24,4,4),
 (25,4,8),
 (26,5,0),
 (27,5,1),
 (28,5,2),
 (29,5,3),
 (30,5,4),
 (31,5,5),
 (32,5,6),
 (33,5,7),
 (34,5,8),
 (35,0,7),
 (36,1,7),
 (37,2,7),
 (38,3,7),
 (39,4,7);
INSERT INTO `orders` (id,icon) VALUES (0,'go.png'),
 (1,'stop.png'),
 (2,'charge.png'),
 (3,'attack.png'),
 (4,'patrol.png'),
 (5,'dead.png'),
 (6,'defend.png'),
 (7,'follow.png'),
 (8,'collect.png');
INSERT INTO `nation` (id,name) VALUES (0,'basic'),
 (1,'enemy');
INSERT INTO `map` (id,height_map,texture,scale_hor,scale_ver) VALUES (0,'map/HeightMap.png','Materials/StoneTiled.xml',2.0,0.1),
 (1,'map/test2.png','Materials/test.xml',2.0,0.1);
INSERT INTO `hud_size_vars` (id,hud_size,name,value) VALUES (0,0,'iconX',42.0),
 (1,0,'iconY',42.0),
 (2,0,'spaceX',4.0),
 (3,0,'spaceY',3.0),
 (4,0,'bigX',960.0),
 (5,0,'bigY',540.0),
 (6,1,'iconX',64.0),
 (7,1,'iconY',64.0),
 (8,1,'spaceX',6.0),
 (9,1,'spaceY',4.0),
 (10,1,'bigX',1440.0),
 (11,1,'bigY',810.0),
 (12,2,'iconX',86.0),
 (13,2,'iconY',86.0),
 (14,2,'spaceX',4.0),
 (15,2,'spaceY',6.0),
 (16,2,'bigX',1920.0),
 (17,2,'bigY',1080.0),
 (18,3,'iconX',128.0),
 (19,3,'iconY',128.0),
 (20,3,'spaceX',12.0),
 (21,3,'spaceY',8.0),
 (22,3,'bigX',2880.0),
 (23,3,'bigY',1620.0),
 (24,0,'fontSize',10.0),
 (25,1,'fontSize',12.0),
 (26,2,'fontSize',14.0),
 (27,3,'fontSize',18.0);
INSERT INTO `hud_size` (id,name) VALUES (0,'s'),
 (1,'m'),
 (2,'l'),
 (3,'xl');
INSERT INTO `graph_settings` (id,hud_size,res_x,res_y,style,fullscreen,max_fps,min_fps) VALUES (0,0,1366,768,'DefaultStyle.xml',0,140.0,1.0);
INSERT INTO `cost_unit` (id,resource,value,unit) VALUES (0,0,90,3),
 (1,1,120,0),
 (2,0,80,1),
 (3,1,130,2),
 (4,2,50,4),
 (5,1,30,1),
 (6,0,200,5);
INSERT INTO `cost_building` (id,resource,value,building) VALUES (0,3,100,0),
 (1,1,50,0),
 (2,3,100,1),
 (3,1,100,2),
 (4,3,100,2),
 (5,1,150,3),
 (6,3,50,3),
 (7,1,100,4),
 (8,3,100,4),
 (9,0,100,4),
 (10,0,300,5);
INSERT INTO `building_type` (id,name) VALUES (0,'house'),
 (1,'tower'),
 (2,'barracks'),
 (3,'archery_range'),
 (4,'mock');
INSERT INTO `building_to_unit` (id,building,unit) VALUES (0,4,0),
 (1,2,0),
 (2,2,2),
 (3,3,1),
 (4,5,0),
 (5,4,1),
 (6,4,2),
 (7,4,3),
 (8,4,4),
 (9,5,1),
 (10,5,2),
 (11,5,3),
 (12,0,4),
 (13,4,5),
 (14,5,5);
INSERT INTO `building` (id,name,sizeX,sizeZ,type,model,texture,font,scale,texture_temp,nation,icon,queue_max_capacity) VALUES (0,'house',1,1,0,'House2.mdl','house2.xml','Anonymous Pro.ttf',2.5,'house_temp.xml',0,'house.png',5),
 (1,'tower',1,1,1,'Tower.mdl','tower.xml','Anonymous Pro.ttf',0.5,'tower_temp.xml',0,'tower.png',5),
 (2,'barracks',2,2,2,'Barracks.mdl','barracks.xml','Anonymous Pro.ttf',0.5,'barracks_temp.xml',0,'barracks.png',10),
 (3,'archery_range',2,2,3,'Barracks.mdl','barracks.xml','Anonymous Pro.ttf',0.5,'barracks_temp.xml',0,'archery_range.png',10),
 (4,'mock',3,3,4,'cube.mdl','transparent.xml','Anonymous Pro.ttf',5.0,'transparent_temp.xml',0,'mock.png',20),
 (5,'mock_enemy',3,3,4,'cube.mdl','transparent.xml','Anonymous Pro.ttf',2.5,'transparent_temp.xml',1,'mock2.png',20);
COMMIT;
