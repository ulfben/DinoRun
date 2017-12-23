# DinoRun

This is an implementation of Chrome's "dino runner"
		https://chromedino.com/
and my first attempt at programming for the Arduboy.
    https://arduboy.com/

The project borrows heavily from Filmote's tutorial in Arduboy Magazine (vol 10):
	https://issuu.com/arduboymag/docs/arduboy_20magazine_20volume_2010
Most notably the bitmaps and the pixel-based collision test. Thanks Filmote!


Known bugs:
-  Can walk through low-flying Ptereodactyls
-  Obstacles::spawnObstacle doesn't respect spawn delays
-  The various gamestate constructors are not executed after new.

TODO: 	
- 	persistent storage of highscore
-	debug State ctor/dtor not executing
