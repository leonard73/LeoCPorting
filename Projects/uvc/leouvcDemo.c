#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "leouvc.h"
void cam1_callback(uvc_frame_t *frame, void *ptr)
{
    struct timeval ts_cur;
    gettimeofday(&ts_cur,NULL);
    printf("[cam1_callback]: %lf ms ",(double)(ts_cur.tv_sec*100000+ts_cur.tv_usec)/1000);
    log_uvc_frame_format(frame->frame_format);
}
void cam2_callback(uvc_frame_t *frame, void *ptr)
{
    struct timeval ts_cur;
    gettimeofday(&ts_cur,NULL);
    printf("[cam2_callback]: %lf ms ",(double)(ts_cur.tv_sec*100000+ts_cur.tv_usec)/1000);
    log_uvc_frame_format(frame->frame_format);
}
int main(int argc , char * argv[])
{
    UVC_UTILS_DEVICE cam1;
    cam1.format      = UVC_FRAME_FORMAT_YUYV;
    cam1.frameWidth  = 640;
    cam1.frameHeight = 480;
    cam1.targetFps   = 30;
    cam1.pid         = 0x0120;
    cam1.vid         = 0x1234;
    cam1.cb          = cam1_callback;
    UVC_UTILS_DEVICE cam2;
    cam2.format      = UVC_FRAME_FORMAT_YUYV;
    cam2.frameWidth  = 1280;
    cam2.frameHeight = 480;
    cam2.targetFps   = 100;
    cam2.pid         = 0x15aa;
    cam2.vid         = 0x1555;
    cam2.cb          = cam2_callback;
    init_uvc_camera_device(&cam1);
    init_uvc_camera_device(&cam2);
    start_uvc_camera_device(&cam1);
    start_uvc_camera_device(&cam2);
    sleep(10);
    free_uvc_camera_device(&cam1);
    free_uvc_camera_device(&cam2);
    
}