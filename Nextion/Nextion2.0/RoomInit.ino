#include "RoomInit.h"

void room_init(Room *obj, String name) //инициализация одной комнаты
{
  obj->name = name;
  obj->light = 0;
  obj->brightness = 50;
  obj->warmth = 50; 
}

void rooms_init() //инициализация всех комнат
{
  room_init(&room[0], "hallway"); //коридор
  room_init(&room[1], "kitchen"); //кухня
  room_init(&room[2], "bathroom");//ванная 
  room_init(&room[3], "toilet");//туалет
  room_init(&room[4], "smRoom");// мал комната
  room_init(&room[5], "bigRoom"); //большая комната
}
