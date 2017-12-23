#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
enum Frames{
	Standing,
	Running1,
	Running2,
	Ducking1,
	Ducking2,
	Dead1,
	Dead2
};
class Player : public Entity{	
	bool _isJumping = false;
	byte _jumpIndex = 0;
	Frames _frame = Frames::Standing;
	inline void doJump(){	
		//hardcoded jump arc? Thanks again to Simon Holmes for the inspiration! :)
		static const uint8_t jumpFrames = 61;
		static const int8_t jumpDeltaY[jumpFrames] = {-4,-3,-5,-4,-3,-2,-2,-2,-1,-2,-1,-1,-1,-1,-1,-1,-1,0,-1,0,-1,0,-1,0,-1,0,0,0,-1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,2,1,2,2,2,3,4,4,4};
		_y += jumpDeltaY[_jumpIndex];
		_jumpIndex++;
		if(_jumpIndex >= jumpFrames){
			_isJumping = false;
			_jumpIndex = 0;
			_y = DINO_GROUND_LEVEL;
		}
	};
	inline void animate(){
		switch(_frame){
		case Frames::Standing:
			_frame = Frames::Running1;
			break;
		case Frames::Running1:
			_frame = Frames::Running2;
			break;
		case Frames::Running2:
			_frame = Frames::Running1;
			break;
		case Frames::Ducking1:
			_frame = Frames::Ducking2;
			break;
		case Frames::Ducking2:
			_frame = Frames::Ducking1;
			break;
		case Frames::Dead1:
			_frame = Frames::Dead2;
			break;
		case Frames::Dead2:
			//final frame, do nothing.
			break;
		default:
			_frame = Frames::Standing;
			break;
		}
	};
public:			
	Player() : Entity(2, DINO_GROUND_LEVEL, dinosaur_still, dinosaur_still_mask){
		reset();
	};
	void reset(){
		_x = 2;
		_y = DINO_GROUND_LEVEL;
		_isJumping = false;
		_frame = Frames::Standing;
	};
	void input(){
		if(_isJumping || _frame == Frames::Dead1 || _frame == Frames::Dead2){
			return; //no air-control, nor movement after death.
		}		
		if(arduboy.justPressed(JUMP_BUTTON) || arduboy.justPressed(JUMP_BUTTON2)){
			_isJumping = true; 
			_jumpIndex = 0;
		}else if(arduboy.justPressed(DUCK_BUTTON) || arduboy.justPressed(DUCK_BUTTON2)){
			if(_frame != Frames::Ducking2){ _frame = Frames::Ducking1; };
		}else if(arduboy.notPressed(DUCK_BUTTON) && arduboy.notPressed(DUCK_BUTTON2) && (_frame == Frames::Ducking1 || _frame == Frames::Ducking2)){
			_frame = Frames::Running1;
		}
		if(arduboy.pressed(LEFT_BUTTON) && _x > PLAYER_MIN_POS){
			_x--;
		}
		if(arduboy.pressed(RIGHT_BUTTON) && _x < PLAYER_MAX_POS){
			_x++;
		}
	};
	void update(){
		if(_isJumping){
			doJump();
		} else if(arduboy.everyXFrames(3)){
			animate();			
		}
		_bitmap = dino_images[_frame];
		_mask = dino_masks[_frame];
	};
	void render() const {
		//the "registration point" is at the bottom of the sprite, so calculate where the top is
		Sprites::drawExternalMask(_x, yMinusHeight(), _bitmap, _mask, FRAME, FRAME);
	};
	void onCollision(){		
		if(_frame <= Frames::Running2){ //standing up
			_frame = Frames::Dead1;
		} else{ //or ducking			
			_frame = Frames::Dead2;
		}
	};
};
#endif //PLAYER_H