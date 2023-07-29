#include "hsi_basic.h"
#include <math.h>
#define MIN_(A,B) ( (A)<(B) ? A : B )
#define MAX_(A,B) ( (A)>(B) ? A : B )
#define CLAMP_(V,A,B) (MIN_(MAX_(V,A),B))
void rgb888_to_hsiF32(const uint8_t* rgb,float* Hue, float* Saturation, float*  Intensity,uint32_t width,uint32_t height)
{
    const float my_pi = 3.14159265358979323846f;
    for(uint32_t i=0;i<width*height;i++)
    {
        float R = (float) rgb[i*3+0],G = (float) rgb[i*3+1],B = (float) rgb[i*3+2];
        float theita = acosf((0.5f*((R-G)+(R-B)))/sqrtf((R-G)*(R-G)+(R-B)*(G-B))+__DBL_EPSILON__);
        uint32_t den   = R+G+B;
        uint32_t min_v  = MIN_(MIN_(R,G),B);
        Intensity[i] = ((float)den)/3.0F;
        Hue[i]       = B > G ? (2*my_pi-theita) : theita  ;
        Saturation[i]= (den == 0 ) ? (0) :  ( 1 - 3 * (float)min_v/(float)den);
        Hue[i]       = (min_v*3 == den) ? 0.0f : Hue[i];
    }
}
void hsiF32_to_rgb888(uint8_t* rgb,const float* Hue,const float* Saturation,const float*  Intensity,uint32_t width,uint32_t height)
{
    const float my_pi = 3.14159265358979323846f;
    for(uint32_t i=0;i<width*height;i++)
    {
        float H = Hue[i],S = Saturation[i], I = Intensity[i];
        uint8_t R =0,G =0,B=0;
        if((H >= 0) && (H < (2*my_pi/3)))
        {
            B = (uint8_t) (I * (1-S));
            R = I * (1 + S * cosf(H) / cosf(my_pi/3 - H));
            G = 3 * I - (B + R);
        }
        else if((H >= (2*my_pi/3)) && (H < (4*my_pi/3)))
        {
            R = (uint8_t) (I * (1-S));
            G = I * (1 + S * cosf(H-2*my_pi/3) / cosf(my_pi - H));
            B = 3 * I - (R + G);
        }
        else
        {
            G = (uint8_t) (I * (1-S));
            B = I * (1 + S * cosf(H-4*my_pi/3) / cosf(5*my_pi/3 - H));
            R = 3 * I - (B + G);
        }
        rgb[3*i+0] = CLAMP_(R,0,255);
        rgb[3*i+1] = CLAMP_(G,0,255);
        rgb[3*i+2] = CLAMP_(B,0,255);
    }    
}
void enhanceHSI_F32( float* Hue, float* Saturation, float*  Intensity,uint32_t pixels,float Rh,float Rs,float Ri)
{
    const float my_pi = 3.14159265358979323846f;
    for(uint32_t i=0;i<pixels;i++)
    {
        Hue[i] *= Rh;
        Saturation[i] *= Rs;
        Intensity[i] *= Ri;
        Hue[i]=CLAMP_(Hue[i],0.0f,2*my_pi);
        Saturation[i]=CLAMP_(Saturation[i],0.0f,1.0f);
        Intensity[i]=CLAMP_(Intensity[i],0.0f,255.0f);
    }
}