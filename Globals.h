#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
#include <Arduboy2.h>
#include "Assets.h"
#include "Configs.h"
static Arduboy2 arduboy;

#define DEBUG
#ifdef DEBUG	
	#define trace(x)	Serial.println(x)
#else	
	#define trace(x)
#endif

#endif //GLOBALS_H