#include "hsi_basic.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MIN_(A,B) ( (A)<(B) ? A : B )
#define MAX_(A,B) ( (A)>(B) ? A : B )
#define CLAMP_(V,A,B) (MIN_(MAX_(V,A),B))
#define M_PI_ (3.14159265358979323846f)
#define M_E_  (2.7182818284590452354f)
#define _GET_GAUSSIAN_K2D_(x,y,sigma2) ((1.0f/(2*M_PI_*sigma2)) * powf(M_E_,(x*x+y*y)/((-2.0f)*sigma2)) )
void generate_gaussian_kernel_2d(float *gaussianKernel_2D,uint32_t kernel_size,float sigma)
{
   int k_h =  kernel_size / 2;
   int id=0;
   for(int h  =  -k_h;  h <=  k_h ;  ++  h)
   {
       for(int w = -k_h; w <= k_h ; ++ w)
       {
           gaussianKernel_2D[id++] = _GET_GAUSSIAN_K2D_(w,h,sigma*sigma);
       }
   }
}
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
void enhanceHSI_HistGamma( float* Hue, float* Saturation, float*  Intensity,uint32_t pixels,uint8_t bottom_limit)
{
    uint32_t bottom_limit_counter=0;
    for(uint32_t i=0;i<pixels;i++)
    {
        bottom_limit_counter = (Intensity[i] < (float)bottom_limit) ? (bottom_limit_counter+1) : bottom_limit_counter;
    }
    float gamma =1.0f -   ((float)bottom_limit_counter/(float)pixels) ; 
    gamma = CLAMP_(gamma,0.7,1.0);
    for(uint32_t i=0;i<pixels;i++)
    {
        Intensity[i] = powf( Intensity[i]/255.0f,gamma ) * 255.0f;
        Intensity[i]=CLAMP_(Intensity[i],0.0f,255.0f);
    }    
}
void do_convolution_2d_f32(float * input2d,float* output2d,float * kernel,uint32_t w,uint32_t h,uint32_t window_sz)
{
    int radius = window_sz/2;
    memcpy(output2d,input2d,radius*w*sizeof(float));
    for(int i=radius;i<(h-radius);i++)
    {
        for(int j=0;j<radius;j++)
        {
            output2d[i*w+j] = input2d[i*w+j];
        }
        for(int j=radius;j<(w-radius);j++)
        {
            float sum=0.0f;
            int counter=0;
            for(int a = (i-radius);a<=(i+radius);a++)
            {
                for(int b=(j-radius);b<=(j+radius);b++)
                {
                    sum += input2d[a*w+b] * kernel[counter++];
                }
            }
            output2d[i*w+j] = sum;
        }
        for(int j=w-radius;j<w;j++)
        {
            output2d[i*w+j] = input2d[i*w+j];
        }
    }
    memcpy(output2d+(h-radius)*w,input2d+(h-radius)*w,radius*w*sizeof(float));
}
void enhanceHSI_Retinex( float* Hue, float* Saturation, float*  Intensity,uint32_t width,uint32_t height,uint32_t smooth_window_size)
{
    float * L_array = (float*) malloc(width * height * sizeof(float));
    float * gaussian_kernel = (float*) malloc(smooth_window_size*smooth_window_size*sizeof(float));
    generate_gaussian_kernel_2d(gaussian_kernel,smooth_window_size,1.2f);
    do_convolution_2d_f32(Intensity,L_array,gaussian_kernel,width,height,smooth_window_size);
    float max_v=0.0f;
    float gamma = 1.0;
    for(uint32_t i=0;i<(width*height);i++)
    {
        L_array[i] = powf( L_array[i]/255.0f,gamma ) * 255.0f;
        float diff =fabsf( log2f(Intensity[i])-log2f(L_array[i]));
        diff = diff > 0.0f ? diff:0.0f;
        Intensity[i] = exp2f(diff);
        // Intensity[i] = L_array[i] > 0.0f? Intensity[i]/(powf( L_array[i]/255.0f,gamma ) * 255.0f) : 0.0f;
        max_v = Intensity[i] > max_v  ? Intensity[i] : max_v;
        // printf(" %f; ",Intensity[i] );
        // Intensity[i]=CLAMP_(Intensity[i],0.0f,255.0f);
    }
    printf("max_v=%f\n",max_v);
    float max_v_inv = 255.0f /max_v;
    for(uint32_t i=0;i<(width*height);i++)
    {
        Intensity[i] *= max_v_inv;
        Intensity[i]=CLAMP_(Intensity[i],0.0f,255.0f);
    }
    free(gaussian_kernel);
    free(L_array);   
}