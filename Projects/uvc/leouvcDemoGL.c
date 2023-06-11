#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <GL/glut.h>
#include "leouvc.h"
/*CAM1 CFG*/
#define CAM1_PIXEL_WIDTH    (1280)
#define CAM1_PIXEL_HEIGHT   (480)
#define CAM1_PIXEL_CHANNEL  (3)
#define CAM1_PIXEL_NUM      (CAM1_PIXEL_WIDTH*CAM1_PIXEL_HEIGHT)
#define CAM1_PID            (0x15aa)
#define CAM1_VID            (0x1555)
#define CAM1_FPS            (100)
/*CAM2 CFG*/
#define CAM2_PIXEL_WIDTH    (640)
#define CAM2_PIXEL_HEIGHT   (480)
#define CAM2_PIXEL_CHANNEL  (3)
#define CAM2_PIXEL_NUM      (CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT)
#define CAM2_PID            (0x0120)
#define CAM2_VID            (0x1234)
#define CAM2_FPS            (30)
GLubyte pixel_array1[CAM1_PIXEL_NUM*CAM1_PIXEL_CHANNEL];
GLubyte pixel_array2[CAM2_PIXEL_NUM*CAM2_PIXEL_CHANNEL];
int glWindow1,glWindow2;
int render_start=1,render_start2=1;
void idle_global()
{ 
  {
    glutSetWindow(glWindow1);
    render_start=0;
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(-1,1);
    glPixelZoom(1.0f,-1.0f);
    glDrawPixels(CAM1_PIXEL_WIDTH,CAM1_PIXEL_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array1);
    render_start=1;
    glutSwapBuffers();
  }
  {
    glutSetWindow(glWindow2);
    render_start2=0;
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(-1,1);
    glPixelZoom(1.0f,-1.0f);
    glDrawPixels(CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array2);
    render_start2=1;
  }
//   glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}
void cam1_callback(uvc_frame_t *frame, void *ptr)
{
    struct timeval ts_cur;
    gettimeofday(&ts_cur,NULL);
    printf("[cam1_callback]: %lf ms ",(double)(ts_cur.tv_sec*100000+ts_cur.tv_usec)/1000);
    log_uvc_frame_format(frame->frame_format);
    uint8_t * data_p = (uint8_t *)(frame->data);
    while(render_start==0)
    {
      usleep(1);
    }
    cvt_yuyv2rgb(data_p,pixel_array1,CAM1_PIXEL_WIDTH,CAM1_PIXEL_HEIGHT);
    
}
void cam2_callback(uvc_frame_t *frame, void *ptr)
{
    struct timeval ts_cur;
    gettimeofday(&ts_cur,NULL);
    printf("[cam2_callback]: %lf ms ",(double)(ts_cur.tv_sec*100000+ts_cur.tv_usec)/1000);
    log_uvc_frame_format(frame->frame_format);
    uint8_t * data_p = (uint8_t *)(frame->data);
    while(render_start2==0)
    {
      usleep(1);
    }
    cvt_yuyv2rgb(data_p,pixel_array2,CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT);
}
int main(int argc , char * argv[])
{
    UVC_UTILS_DEVICE cam1;
    cam1.format      = UVC_FRAME_FORMAT_YUYV;
    cam1.frameWidth  = CAM1_PIXEL_WIDTH;
    cam1.frameHeight = CAM1_PIXEL_HEIGHT;
    cam1.targetFps   = CAM1_FPS;
    cam1.pid         = CAM1_PID;
    cam1.vid         = CAM1_VID;
    cam1.cb          = cam1_callback;
    UVC_UTILS_DEVICE cam2;
    cam2.format      = UVC_FRAME_FORMAT_YUYV;
    cam2.frameWidth  = CAM2_PIXEL_WIDTH;
    cam2.frameHeight = CAM2_PIXEL_HEIGHT;
    cam2.targetFps   = CAM2_FPS;
    cam2.pid         = CAM2_PID;
    cam2.vid         = CAM2_VID;
    cam2.cb          = cam2_callback;
    init_uvc_camera_device(&cam1);
    init_uvc_camera_device(&cam2);
    start_uvc_camera_device(&cam1);
    start_uvc_camera_device(&cam2);

    
    //opengl cfg
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //window1
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(CAM1_PIXEL_WIDTH, CAM1_PIXEL_HEIGHT);
    glWindow1=glutCreateWindow("OpenGL glShowRGB_CAM1");
    glutSetWindow(glWindow1);
    glutIdleFunc(idle_global); 
    //window2
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(CAM2_PIXEL_WIDTH, CAM2_PIXEL_HEIGHT);
    glWindow2 = glutCreateWindow("OpenGL glShowRGB_CAM2");
    glutSetWindow(glWindow2);
    glutIdleFunc(idle_global);  
    glutMainLoop();
    //

    free_uvc_camera_device(&cam1);
    free_uvc_camera_device(&cam2);
}