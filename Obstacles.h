#pragma once
#ifndef OBSTACLES_H
#define OBSTACLES_H
#include "Entity.h"
#include "CollisionManager.h"
/*
 * This file contains two classes: both the Obstacle-entity, and the Obstacles "container".
 * I am currently debugging Obstacles::spawnObstacle(); as it ignores spawn order
 */
enum ObstacleType : uint8_t{
	SingleCactus,
	DoubleCactus,
	TripleCactus,
	Pterodactyl1,
	Pterodactyl2,
	count = 4, //Pterodactyl1 and Pterodactyl2 refer to the same obstacle, so -1
};
class Obstacle : public Entity {	
protected:
	ObstacleType _type = ObstacleType::SingleCactus;	
public:	
	bool _enabled = false;	
	explicit Obstacle(const ObstacleType type = ObstacleType::SingleCactus)
		: Entity{0, 0, obstacle_images[type], nullptr}, _type{type}, _enabled{false}{					
		trace("Obstacle ctor");
	}	
	void reset(const int nx, const int ny, const ObstacleType type){
		_x = nx; 
		_y = ny; 
		_type = type; 
		_bitmap = obstacle_images[type];
		_enabled = true;		
		adjustSpawnHeight();
	}
	void update(){
		if(arduboy.everyXFrames(3)){
			if(_type == ObstacleType::Pterodactyl1){
				_type = ObstacleType::Pterodactyl2;
			} else if(_type == ObstacleType::Pterodactyl2){
				_type = ObstacleType::Pterodactyl1;
			}
			_bitmap = obstacle_images[_type];
		}				
	}
	void render() const{
		Sprites::drawOverwrite(_x, yMinusHeight(), _bitmap, FRAME);
	}
	inline bool isColliding(const int16_t x, const int16_t y, const uint8_t* img) const{
		return collide(x, y, img, this->_x, yMinusHeight(), _bitmap);
	}
	inline bool isColliding(const Entity& that) const{
		return isColliding(that._x, that._y, that.getImage());
	}
private:
	inline void adjustSpawnHeight() {
		if(_type == ObstacleType::Pterodactyl1 || _type == ObstacleType::Pterodactyl2){
			_y = random(PTERODACTYL_UPPER_LIMIT, PTERODACTYL_LOWER_LIMIT);
		}
	}
};
class Obstacles {	
	unsigned int _nextSpawn = OBSTACLE_SPAWN_DELAY_MIN;
	uint16_t _frameCount = 0;
	Obstacle _obstacles[NUMBER_OF_OBSTACLES] = {};
	inline static ObstacleType randomType(const ObstacleType upTo){				
		return static_cast<ObstacleType>(
			random(static_cast<long>(SingleCactus), static_cast<long>(upTo) + 1));
	}
	ObstacleType getNextType() const {		
		if(_frameCount < FRAME_START_DOUBLE_CACTUS){			
			trace(F("framecount < 300"));
			return randomType(ObstacleType::SingleCactus);
		} 
		if(_frameCount < FRAME_START_TRIPPLE_CACTUS){			
			trace(F("framecount < 600"));
			return randomType(ObstacleType::DoubleCactus);
		} 
		if(_frameCount < FRAME_START_PTERODACTYL){			
			trace(F("framecount < 900"));
			return randomType(ObstacleType::TripleCactus);
		} 		
		trace(F("framecount > 900"));
		return randomType(ObstacleType::count);		
	}	
	void spawnObstacle(){
		trace(F("spawn obstacle"));
		_nextSpawn = random(OBSTACLE_SPAWN_DELAY_MIN, OBSTACLE_SPAWN_DELAY_MAX);		
		for(uint8_t i = 0; i < NUMBER_OF_OBSTACLES; ++i){
			if(_obstacles[i]._enabled){ continue; } //look for unused entity
			const ObstacleType next = getNextType();
			_obstacles[i].reset(WIDTH - 1, CACTUS_GROUND_LEVEL, next);
			return;			
		}
	}
public:	
	Obstacles(){		
		reset(); //I am paranoid over the _obstacles array not initializiing
	};
	void reset() {	
		_frameCount = 0;
		for(uint8_t i = 0; i < NUMBER_OF_OBSTACLES; ++i){
			_obstacles[i].reset(0, 0, ObstacleType::SingleCactus);
			_obstacles[i]._enabled = false;
		}
	}
	void update(){
		_frameCount++;
		if(--_nextSpawn == 0){
			spawnObstacle();
		}		
		for(uint8_t i = 0; i < NUMBER_OF_OBSTACLES; ++i){
			if(!_obstacles[i]._enabled){ continue; }
			if(_obstacles[i]._x-- < -_obstacles[i].width()){
				_obstacles[i]._enabled = false; //out of view				
			}
			_obstacles[i].update();				
		}
	};
	void render() const{
		for(uint8_t i = 0; i < NUMBER_OF_OBSTACLES; ++i){
			if(!_obstacles[i]._enabled){continue;}	
			_obstacles[i].render();
		}
		arduboy.setCursor(17, 12);
		arduboy.print(_frameCount);		
	};
	bool collidesWith(Entity& that) const{
		const auto xpos = that._x;
		const auto ypos = that.yMinusHeight(); 
		const auto image = that.getImage();
		for(uint8_t i = 0; i < NUMBER_OF_OBSTACLES; i++){
			if(_obstacles[i]._enabled && _obstacles[i].isColliding(xpos, ypos, image)){				
				return true;
			}
		}
		return false;
	}
};
#endif //OBSTACLES_H