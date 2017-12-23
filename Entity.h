#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include "Globals.h"
class Entity{
protected:
	static const uint8_t FRAME = 0; //for use with non-animated entities (default)
	const byte* _bitmap = nullptr;
	const byte* _mask = nullptr;	
public:
	int16_t _x;
	int16_t _y;
	Entity(const int16_t x, const int16_t y, const byte* image, const byte* mask = nullptr)
		: _bitmap{image}, _mask{mask}, _x{x}, _y{y} {
	}
	inline virtual uint8_t width() const{
		return pgm_read_byte(_bitmap);
	};
	inline virtual uint8_t height() const{
		return pgm_read_byte(_bitmap + 1);
	};
	inline uint8_t yMinusHeight() const{
		return _y - height();
	};
	inline const byte* getImage() const{
		return _bitmap;
	}
	inline const byte* getMask() const{
		return _mask;
	}
	virtual ~Entity(){}
};
#endif //ENTITY_H