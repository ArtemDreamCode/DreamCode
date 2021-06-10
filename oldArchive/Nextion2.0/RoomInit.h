#ifndef __ROOM__INIT
#define __ROOM__INIT


#define ROOMS_COUNT 10

#define ROOM_HALLWAY  0
#define ROOM_KITCHEN  1
#define ROOM_BATHROOM 2
#define ROOM_TOILET   3
#define ROOM_SMROOM   4
#define ROOM_BIGROOM  5

typedef struct {
  String name;
  unsigned short int light; //вкл/выкл свет
  unsigned short int brightness; //яркость
  unsigned short int warmth; //теплота
//  struct info{};
} Room;

void room_init(Room *obj, String name); //инициализация одной комнаты
void rooms_init(); //инициализация всех комнат

#endif
