#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "leouvc.h"
#define CHECK_0_255(V) ( (V>255) ? (255) : (V<0 ? 0 : V ) )
void init_uvc_camera_device(UVC_UTILS_DEVICE * dev)
{
    dev->res = uvc_init(&dev->ctx, NULL); 
    if(dev->res<0) printf("uvc_init failed!\n");
    dev->res = uvc_find_device(dev->ctx, &dev->dev, dev->pid, dev->vid, NULL);
    if(dev->res<0) printf("uvc_find_device failed!\n");
    dev->res = uvc_open(dev->dev, &dev->devh);
    if(dev->res<0) printf("uvc_open failed!\n");
    dev->res = uvc_get_stream_ctrl_format_size(dev->devh, &dev->ctrl, dev->format, dev->frameWidth, dev->frameHeight, dev->targetFps );
    if(dev->res<0) printf("uvc_get_stream_ctrl_format_size failed!\n");
    if(dev->res>=0) printf("init_uvc_camera_device ok!\n");
}
void free_uvc_camera_device(UVC_UTILS_DEVICE * dev)
{
    uvc_stop_streaming(dev->devh);
    uvc_close(dev->devh);
    uvc_unref_device(dev->dev);
    uvc_exit(dev->ctx);
    printf("free_uvc_camera_device ok!\n");
}
void start_uvc_camera_device(UVC_UTILS_DEVICE * dev)
{
    dev->res = uvc_start_streaming(dev->devh, &dev->ctrl, dev->cb, 0, 0);
    if(dev->res>=0)
    {
        printf("start_uvc_camera_device ok!\n");
    }
}
void log_uvc_frame_format(int uvc_f)
{
    switch(uvc_f)
    {
        case UVC_FRAME_FORMAT_ANY:         printf("UVC_FORMAT=UVC_FRAME_FORMAT_ANY\n");         break;
        case UVC_FRAME_FORMAT_UNCOMPRESSED:printf("UVC_FORMAT=UVC_FRAME_FORMAT_UNCOMPRESSED\n");break;
        case UVC_FRAME_FORMAT_COMPRESSED:  printf("UVC_FORMAT=UVC_FRAME_FORMAT_COMPRESSED\n");  break;
        case UVC_FRAME_FORMAT_YUYV:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_YUYV\n");        break;
        case UVC_FRAME_FORMAT_UYVY:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_UYVY\n");        break;
        case UVC_FRAME_FORMAT_RGB:         printf("UVC_FORMAT=UVC_FRAME_FORMAT_RGB\n");         break;
        case UVC_FRAME_FORMAT_BGR:         printf("UVC_FORMAT=UVC_FRAME_FORMAT_BGR\n");         break;
        case UVC_FRAME_FORMAT_MJPEG:       printf("UVC_FORMAT=UVC_FRAME_FORMAT_MJPEG\n");       break;
        case UVC_FRAME_FORMAT_H264:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_H264\n");        break;
        case UVC_FRAME_FORMAT_GRAY8:       printf("UVC_FORMAT=UVC_FRAME_FORMAT_GRAY8\n");       break;
        case UVC_FRAME_FORMAT_GRAY16:      printf("UVC_FORMAT=UVC_FRAME_FORMAT_GRAY16\n");      break;
        case UVC_FRAME_FORMAT_BY8:         printf("UVC_FORMAT=UVC_FRAME_FORMAT_BY8\n");         break;
        case UVC_FRAME_FORMAT_BA81:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_BA81\n");        break;
        case UVC_FRAME_FORMAT_SGRBG8:      printf("UVC_FORMAT=UVC_FRAME_FORMAT_SGRBG8\n");      break;
        case UVC_FRAME_FORMAT_SGBRG8:      printf("UVC_FORMAT=UVC_FRAME_FORMAT_SGBRG8\n");      break;
        case UVC_FRAME_FORMAT_SRGGB8:      printf("UVC_FORMAT=UVC_FRAME_FORMAT_SRGGB8\n");      break;
        case UVC_FRAME_FORMAT_SBGGR8:      printf("UVC_FORMAT=UVC_FRAME_FORMAT_SBGGR8\n");      break;
        case UVC_FRAME_FORMAT_NV12:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_NV12\n");        break;
        case UVC_FRAME_FORMAT_P010:        printf("UVC_FORMAT=UVC_FRAME_FORMAT_P010\n");        break;
        case UVC_FRAME_FORMAT_COUNT:       printf("UVC_FORMAT=UVC_FRAME_FORMAT_COUNT\n");       break;

    }
}
void cvt_yuyv2rgb(uint8_t * yuyv,uint8_t * gl_rgb_buffer,uint32_t w,uint32_t h)
{

  for(uint32_t i=0;i<h;i++)
  {
    for(uint32_t j=0;j<(w/2);j++)
    {
        float Y1 = (float)(yuyv[i*2*w+j*4+0]);
        float U  = (float)(yuyv[i*2*w+j*4+1])-128;
        float Y2 = (float)(yuyv[i*2*w+j*4+2]);
        float V  = (float)(yuyv[i*2*w+j*4+3])-128;
        uint8_t r1 = (uint8_t)CHECK_0_255((float)Y1 + 1.4075*(float)V);
        uint8_t g1 = (uint8_t)CHECK_0_255((float)Y1 - 0.3455*(float)U - 0.7169*(float)V);
        uint8_t b1 = (uint8_t)CHECK_0_255((float)Y1 + 1.779* (float)U);
        uint8_t r2 = (uint8_t)CHECK_0_255((float)Y2 + 1.4075*(float)V);
        uint8_t g2 = (uint8_t)CHECK_0_255((float)Y2 - 0.3455*(float)U - 0.7169*(float)V);
        uint8_t b2 = (uint8_t)CHECK_0_255((float)Y2 + 1.779* (float)U);
  
        gl_rgb_buffer[i*3*w+j*6+0]=r1;
        gl_rgb_buffer[i*3*w+j*6+1]=g1;
        gl_rgb_buffer[i*3*w+j*6+2]=b1;
        gl_rgb_buffer[i*3*w+j*6+3]=r2;
        gl_rgb_buffer[i*3*w+j*6+4]=g2;
        gl_rgb_buffer[i*3*w+j*6+5]=b2;
    }
  }
}