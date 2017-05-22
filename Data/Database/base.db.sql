BEGIN TRANSACTION;
CREATE TABLE "units" (
	`name`	TEXT,
	`minDist`	REAL,
	`maxSep`	REAL,
	`type`	INTEGER,
	`model`	TEXT,
	`texture`	TEXT,
	`font`	TEXT,
	`mass`	REAL,
	`maxSpeed`	REAL,
	`scale`	REAL,
	FOREIGN KEY(`type`) REFERENCES 'units_type'('id')
);
INSERT INTO `units` (name,minDist,maxSep,type,model,texture,font,mass,maxSpeed,scale) VALUES ('warrior',0.5,3.0,0,'Cube.mdl','red.xml','Anonymous Pro.ttf',1.0,5.0,1.0),
 ('archer',0.4,2.0,4,'Cylinder.mdl','red.xml','Anonymous Pro.ttf',0.8,6.0,1.0);
CREATE TABLE `unit_type` (
	`name`	TEXT,
	`id`	INTEGER,
	PRIMARY KEY(`id`)
);
INSERT INTO `unit_type` (name,id) VALUES ('warrior',0),
 ('pikeman',1),
 ('cavalry',2),
 ('archer',3),
 ('worker',4);
CREATE TABLE `hud_size` (
	`id`	INTEGER,
	`name`	TEXT,
	`icon_size_x`	INTEGER,
	`icon_size_y`	INTEGER,
	PRIMARY KEY(`id`)
);
INSERT INTO `hud_size` (id,name,icon_size_x,icon_size_y) VALUES (0,'s',42,42),
 (1,'m',64,64),
 (2,'l',86,86),
 (3,'xl',128,128);
CREATE TABLE "graph_settings" (
	`id`	INTEGER,
	`hud_size`	INTEGER,
	`res_x`	INTEGER,
	`res_y`	INTEGER,
	`style`	INTEGER,
	`fullscreen`	INTEGER,
	`max_fps`	REAL,
	`min_fps`	REAL,
	PRIMARY KEY(`id`),
	FOREIGN KEY(`hud_size`) REFERENCES `hud_size`(`id`)
);
INSERT INTO `graph_settings` (id,hud_size,res_x,res_y,style,fullscreen,max_fps,min_fps) VALUES (0,1,1280,720,'DefaultStyle.xml','false',10000.0,'0,01');
COMMIT;
