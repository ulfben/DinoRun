#pragma once
#ifndef  COLLISION_MANAGER_H
#define  COLLISION_MANAGER_H
//pixel-perfect collision test courtesy of Simon Holmes
	//https://issuu.com/arduboymag/docs/arduboy_20magazine_20volume_2010
//any bugs were likely introduced by me.
static const uint8_t PROGMEM LUT[]{
	0xFF >> 8, 0xFF >> 7, 0xFF >> 6, 0xFF >> 5, 0xFF >> 4, 0xFF >> 3, 0xFF >> 2, 0xFF >> 1
};
//pixel perfect collision test
static bool collide(const int16_t x1, const int16_t y1, const uint8_t* img1, 
					const int16_t x2, const int16_t y2, const uint8_t* img2){
	static const uint8_t IMG_DATA_OFFSET = 2;
	const uint8_t w1 = pgm_read_byte(&img1[0]);
	const uint8_t h1 = pgm_read_byte(&img1[1]);
	const uint8_t w2 = pgm_read_byte(&img2[0]);
	const uint8_t h2 = pgm_read_byte(&img2[1]);

	// Do the images overlap at all? 
	if((x2 > x1 + w1 || x2 + w2 < x1 || y2 > y1 + h1 || y2 + h2 < y1)){
		return false;
	}
	//calculate overlap
	const uint16_t overlap_left   = max(x1, x2);
	const uint16_t overlap_right  = min(x1 + w1, x2 + w2);
	const uint16_t overlap_top	  = max(y1, y2);
	const uint16_t overlap_bottom = min(y1 + h1, y2 + h2);
		
	// Determine the portion of each _bitmap that fall within the overlapping rectangle.  
	const int16_t img1_left = (overlap_left - x1);
	const int16_t img1_right = (overlap_right - x1);
		  int16_t img1_top_row = (overlap_top - y1) >> 3; //  / 8;
	const int16_t img1_top_bit = (overlap_top - y1) % 8;
	const int16_t img1_bottom_row = (overlap_bottom - y1 - 1) >> 3; // / 8;
	const int16_t img1_bottom_bit = (overlap_bottom - y1) % 8;

	const int16_t img2_left = (overlap_left - x2);
		  int16_t img2_top_row = (overlap_top - y2) >> 3; //  / 8;
	const int16_t img2_top_bit = (overlap_top - y2) % 8;
	const int16_t img2_bottom_row = (overlap_bottom - y2 - 1) >> 3; //  / 8;
	const int16_t img2_bottom_bit = (overlap_bottom - y2) % 8;

	// get the index for the relevant pixels in each _bitmap
	int16_t i1 = (img1_top_row * w1) + img1_left + IMG_DATA_OFFSET;
	int16_t i2 = (img2_top_row * w2) + img2_left + IMG_DATA_OFFSET;

	while(img1_top_row < img1_bottom_row){		
		uint16_t d1 = pgm_read_byte(&img1[i1]) & (img1_top_row == img1_bottom_row && img1_bottom_bit != 0 ? pgm_read_byte(&LUT[img1_bottom_bit]) : 0xFF);
		uint16_t d2 = pgm_read_byte(&img2[i2]) & (img2_top_row == img2_bottom_row && img2_bottom_bit != 0 ? pgm_read_byte(&LUT[img2_bottom_bit]) : 0xFF);
		if(img1_top_bit > 0 && img1_top_row < img1_bottom_row){
			d1 = d1 | ((pgm_read_byte(&img1[i1 + w1]) & (img1_top_row + 1 == img1_bottom_row ? pgm_read_byte(&LUT[img1_bottom_bit]) : 0xFF)) << 8);
		}
		if(img2_top_bit > 0 && img2_top_row < img2_bottom_row){
			d2 = d2 | ((pgm_read_byte(&img2[i2 + w2]) & (img2_top_row + 1 == img2_bottom_row ? pgm_read_byte(&LUT[img2_bottom_bit]) : 0xFF)) << 8);
		}
		// Finally, we bit shift the result if necessary and 'prune' to 8 bits
		d1 = (d1 >> img1_top_bit) & 0xFF;
		d2 = (d2 >> img2_top_bit) & 0xFF;

		// If there has been a collision, then we can exit out of here!  
		if((d1 & d2) > 0){
			return true;
		}

		//If we have stepped past the maximum width of the _bitmap, then wrap to the next row
		if(i1 < (img1_top_row * w1) + img1_right + IMG_DATA_OFFSET){
			++i1;
			++i2;
		} else{			
			++img1_top_row;
			++img2_top_row;
			i1 = (img1_top_row * w1) + img1_left + IMG_DATA_OFFSET;
			i2 = (img2_top_row * w2) + img2_left + IMG_DATA_OFFSET;			
		}
	}
	return false;
}
#endif //COLLISION_MANAGER_H