#pragma once
#ifndef  CONFIGS_H
#define  CONFIGS_H
#include "Assets.h"
static const uint8_t FPS = 60; //gameplay framerate
static const uint8_t FPS_MENUS = 15;  //framerate when not in-game
static const uint8_t GROUND_TILE_WIDTH = 32;
static const uint8_t NUMBER_OF_GROUND_TILES = 5; //32*5 = 160px, one full tile more than screen WIDTH.
static const uint8_t GROUND_LEVEL = HEIGHT-getImageHeight(ground_flat);
static const uint8_t PLAYER_MIN_POS = 0;
static const uint8_t PLAYER_MAX_POS = 100;
static const uint8_t DINO_GROUND_LEVEL = GROUND_LEVEL + 6; 
static const uint8_t CACTUS_GROUND_LEVEL = GROUND_LEVEL + 4;

static const uint8_t NUMBER_OF_OBSTACLES = 3; //max number of simultaneous obstacles
static const uint8_t OBSTACLE_SPAWN_DELAY_MIN = FPS * 1.2f; //the jump is ~60 frames long, let's avoid spawning faster than that
static const uint8_t OBSTACLE_SPAWN_DELAY_MAX = FPS * 3;	//3 seconds maximum wait
static const uint8_t FRAME_START_DOUBLE_CACTUS = FPS * 5;	//start spawning double cactus after 5 seconds
static const uint8_t FRAME_START_TRIPPLE_CACTUS = FPS * 10; //start spawning tripple cactus after 10 seconds
static const uint8_t FRAME_START_PTERODACTYL = FPS * 15;	//start spawning ptereodactyls after 15 seconds

static const uint8_t PTERODACTYL_UPPER_LIMIT = 27; //how high can the pterodactyl fly?
static const uint8_t PTERODACTYL_LOWER_LIMIT = DINO_GROUND_LEVEL-getImageHeight(dinosaur_ducking_1); //48; //how low can the pterodactyl fly?

static const uint8_t ACCEPT_BUTTON = A_BUTTON;
static const uint8_t JUMP_BUTTON = A_BUTTON;
static const uint8_t JUMP_BUTTON2 = UP_BUTTON;
static const uint8_t DUCK_BUTTON = B_BUTTON;
static const uint8_t DUCK_BUTTON2 = DOWN_BUTTON;
#endif //CONFIGS_H