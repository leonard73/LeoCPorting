#ifndef __HSI_BASIC_H__
#define __HSI_BASIC_H__
#include <stdint.h>
void rgb888_to_hsiF32(const uint8_t* rgb,float* Hue, float* Saturation, float*  Intensity,uint32_t width,uint32_t height);
void hsiF32_to_rgb888(uint8_t* rgb,const float* Hue,const float* Saturation,const float*  Intensity,uint32_t width,uint32_t height);
void enhanceHSI_F32( float* Hue, float* Saturation, float*  Intensity,uint32_t pixels,float Rh,float Rs,float Ri);
void enhanceHSI_HistGamma( float* Hue, float* Saturation, float*  Intensity,uint32_t pixels,uint8_t bottom_limit);
#endif //__HSI_BASIC_H__