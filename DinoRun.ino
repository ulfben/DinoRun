#include <Arduboy2.h>
#include "Globals.h"
#include "GameEngine.h"
/*
* This is an implementation of Chrome's "dino runner"
		https://chromedino.com/
* and my first attempt at programming for the Arduboy.
* The project borrows heavily from Filmote's tutorial
* in Arduboy Magazine (vol 10):
	https://issuu.com/arduboymag/docs/arduboy_20magazine_20volume_2010
* Most notably the bitmaps and the pixel-based collision test.
* 

* Known bugs:
*	can walk under low-flying Ptereodactyls
*	doesn't respect spawn delays
* TODO: 	
*	persistent storage of highscore
*	debug State ctor/dtor not executing
*/
GameEngine _engine;
void setup(){	
	arduboy.boot();
	arduboy.blank();
	arduboy.safeMode(); //hold UP at boot to unfuck a locked bootloader 
#ifdef DEBUG
	Serial.begin(9600);
	while(!Serial);
	Serial.println("Tracing ON");
#endif
	_engine.init();
};
void loop(){
	if(!arduboy.nextFrame()){ return; } //idles the CPU 1ms at a time
	_engine.input();
	_engine.update();
	_engine.render();
};
