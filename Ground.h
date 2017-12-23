#ifndef GROUND_H
#define GROUND_H
#include "Entity.h"
enum GroundType{
	Flat,
	Bump,
	Hole,
};
class Ground : public Entity{
	GroundType _tiles[NUMBER_OF_GROUND_TILES];
	static GroundType getRandomTile(){
		const byte type = random(0, 6);
		if(type < 4){
			return GroundType::Flat;
		} else if(type < 5){
			return GroundType::Bump;
		}
		return GroundType::Hole;				
	}
public:
	Ground() : Entity(0, GROUND_LEVEL, nullptr, nullptr){
		for(uint8_t i = 0; i < NUMBER_OF_GROUND_TILES; ++i){
			_tiles[i] = getRandomTile();
		}
	}
	void update(){		
		if(++_x < GROUND_TILE_WIDTH){ return; } //check if we've scrolled an entire tile off screen
		_x = 0;		
		const uint8_t lastIndex = NUMBER_OF_GROUND_TILES - 1;
		for(uint8_t i = 0; i < lastIndex; ++i){
			_tiles[i] = _tiles[i+1]; //move all tiles "up" one slot
		};		
		_tiles[lastIndex] = getRandomTile();
	}
	void render() const{		
		for(uint8_t i = 0; i < NUMBER_OF_GROUND_TILES; ++i){
			Sprites::drawSelfMasked((i * GROUND_TILE_WIDTH) - _x, GROUND_LEVEL, ground_images[_tiles[i]], FRAME);
		}
	};
	inline uint8_t width() const override{
		return GROUND_TILE_WIDTH*NUMBER_OF_GROUND_TILES;
	};
	inline uint8_t height() const override{
		const byte* bitmap = ground_images[_tiles[0]]; //measure off an arbitrary tile in the list.
		return pgm_read_byte(bitmap + 1);
	};
};
#endif //GROUND_H