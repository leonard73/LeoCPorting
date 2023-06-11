#ifndef __LEOUVC_H__
#define __LEOUVC_H__
#include "libuvc/libuvc.h"
typedef void (*uvc_utils_device_cb)(uvc_frame_t *frame, void *ptr);
typedef struct UVC_UTILS_DEVICE_t
{
    int pid;
    int vid;
    int format;
    int targetFps;
    int frameWidth;
    int frameHeight;
    uvc_utils_device_cb    cb;
    uvc_context_t          *ctx;
    uvc_error_t            res;
    uvc_device_t           *dev;
    uvc_device_handle_t    *devh;
    uvc_stream_ctrl_t      ctrl;
}UVC_UTILS_DEVICE;
void init_uvc_camera_device(UVC_UTILS_DEVICE * dev);
void free_uvc_camera_device(UVC_UTILS_DEVICE * dev);
void start_uvc_camera_device(UVC_UTILS_DEVICE * dev);
void log_uvc_frame_format(int uvc_f);
void cvt_yuyv2rgb(uint8_t * yuyv,uint8_t * gl_rgb_buffer,uint32_t w,uint32_t h);
#endif //__LEOUVC_H__