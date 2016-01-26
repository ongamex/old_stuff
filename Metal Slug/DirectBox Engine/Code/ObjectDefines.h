#ifndef ObjectDefines_H
#define ObjectDefines_H

/*0 - 100 reserved*/

//1 - 19 - bonus misc
#define IsBonus(id) ( ( (id > 0) && (id < 20) ) ? TRUE : FALSE )

#define OBJECT_MISC_AID 1
#define OBJECT_MISC_COIN 2
#define OBJECT_MISC_PMPICO 3
#define OBJECT_MISC_CRATE 4

//bullets 20 - 49 
#define IsBullet(id) ( ( (id > 19) && (id < 50) ) ? TRUE : FALSE )

#define OBJECT_BULLET_MACHINEGUN 21 
#define OBJECT_BULLET_PMP		22
#define OBJECT_BULLET_GRENADE 23
#define OBJECT_BULLET_SNIPER 24
//explosions 51 - 79
#define IsExplosion(id) ( ( (id > 50) && (id < 80) ) ? TRUE : FALSE )

#define OBJECT_EXPLOSION_SMALL1 51
#define OBJECT_EXPLOSION_SMALL2 52

//backgrounds 80 - 99
#define IsBackground(id) ( ( (id > 80) && (id < 100) ) ? TRUE : FALSE )
#define OBJECT_BACKGROUND_TEST 81

#define IsMisc(id) ( (id < 100) && (true) ? TRUE : FALSE ) 

//players 101 - 199
#define IsPlayer(id) ( ( (id > 100) && (id < 200) ) ? TRUE : FALSE )

#define OBJECT_PLAYER1 101
#define OBJECT_PLAYER2 102 

//Enemy base_type 201 - 299
#define IsEnemy(id) ( ( (id > 200) && (id < 300) ) ? TRUE : FALSE )

#define OBJECT_ENEMY_ZOMBIE_1 201
#define OBJECT_ENEMY_SNIPER 203
#define OBJECT_ENEMY_NOMAD 204
#define OBJECT_ENEMY_KAMIKAZE_1 205
//structures 301 - 399
#define IsStructure(id) ( ( (id > 300) && (id < 400) ) ? TRUE : FALSE )
#define OBJECT_STRUCT_ROCK 301
//todo :
//null 401 - 499 
#define IsNULL(id) ( ( (id > 400) && (id < 500) ) ? TRUE : FALSE )
#define OBJECT_NULL	401
//object Update retval

#define OBJECT_IDLE 0 
#define OBJECT_DESTROY 1

#endif