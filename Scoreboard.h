#pragma once
#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "Globals.h"
class Scoreboard{
	static const int16_t BOARD_HEIGHT = 10;
	static const int16_t NOT_RUNNING = -1;
	int16_t _highScore = 0;
	int16_t _score = NOT_RUNNING;
public:	
	void update(){
		if(arduboy.everyXFrames(3)){
			_score++;
		}
	}
	void reset(){
		_score = 0;
	}
	void onGameOver(){
		if(_score > _highScore){
			_highScore = _score;
		}
		_score = NOT_RUNNING;
		//TODO: save _highscore to persistent storage
	}
	void render() const{
		arduboy.fillRect(0, 0, WIDTH, BOARD_HEIGHT, BLACK);
		if(_score != NOT_RUNNING){
			arduboy.setCursor(1, 0);
			arduboy.print(F("Score: "));
			arduboy.setCursor(39, 0); 
			printWithLeadingZeroes(_score);
		}
		arduboy.setCursor(72, 0);
		arduboy.print(F("High: "));
		arduboy.setCursor(104, 0); 
		printWithLeadingZeroes(_highScore);
		arduboy.drawLine(0, BOARD_HEIGHT-1, WIDTH, BOARD_HEIGHT-1, WHITE);
	}
	inline static void printWithLeadingZeroes(const uint16_t val){	
		if(val < 1000) arduboy.print("0");
		if(val < 100) arduboy.print("0");
		if(val < 10)  arduboy.print("0");
		arduboy.print(val);
	}
};
#endif SCOREBOARD_H