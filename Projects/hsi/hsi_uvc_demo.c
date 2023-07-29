#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <GL/glut.h>
#include "leouvc.h"
#include "hsi_basic.h"
/*CAM2 CFG*/
#define CAM2_PIXEL_WIDTH    (640)
#define CAM2_PIXEL_HEIGHT   (480)
#define CAM2_PIXEL_CHANNEL  (3)
#define CAM2_PIXEL_NUM      (CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT)
#define CAM2_PID            (0x0120)
#define CAM2_VID            (0x1234)
#define CAM2_FPS            (30)
GLubyte pixel_array2[CAM2_PIXEL_NUM*CAM2_PIXEL_CHANNEL];
int glWindow1,glWindow2;
int render_start=1,render_start2=1;
void renderBitmapString(int x, int y, void *font, const char *string)
{
    const char *c;
    glRasterPos2i(x, y);
    glColor3f(0.0,1.0,0.0);
    // glTranslatef(50,250,0);
    for (c = string; *c != '\0'; c++)
    {
        // glutStrokeCharacter(font, *c);
        glutStrokeCharacter(GLUT_STROKE_ROMAN,*c);
    }
}
void idle_global()
{ 
  {
    glutSetWindow(glWindow2);
    render_start2=0;
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(-1,1);
    glPixelZoom(1.0f,-1.0f);
    glDrawPixels(CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array2);
    // const int font = (int)GLUT_STROKE_MONO_ROMAN;
    // renderBitmapString(1,1, (void *)font, "abcdefghjk");
    
    render_start2=1;
  }
//   glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
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
    // cvt_yuyv2rgb(data_p,pixel_array2,CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT);
    {
        float Rh = 1.3f;
        float Rs = 1.0f;
        float Ri = 1.0f;
        //yuyv2rgb
        uint8_t * rgb888_array = (uint8_t*)malloc(CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT*3*sizeof(uint8_t));
        float   * Hue          = (float*)  malloc(CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT*sizeof(float));
        float   * Saturation   = (float*)  malloc(CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT*sizeof(float));
        float   * Intensity    = (float*)  malloc(CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT*sizeof(float));
        cvt_yuyv2rgb(data_p,rgb888_array,CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT);
        rgb888_to_hsiF32(rgb888_array,Hue, Saturation,  Intensity,CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT);
        enhanceHSI_F32( Hue, Saturation,  Intensity,CAM2_PIXEL_WIDTH*CAM2_PIXEL_HEIGHT, Rh, Rs, Ri);
        hsiF32_to_rgb888(pixel_array2,Hue,Saturation, Intensity,CAM2_PIXEL_WIDTH,CAM2_PIXEL_HEIGHT);  
        free(rgb888_array);
        free(Hue);
        free(Saturation);
        free(Intensity);
        
    }
}
int main(int argc , char * argv[])
{
    UVC_UTILS_DEVICE cam2;
    cam2.format      = UVC_FRAME_FORMAT_YUYV;
    cam2.frameWidth  = CAM2_PIXEL_WIDTH;
    cam2.frameHeight = CAM2_PIXEL_HEIGHT;
    cam2.targetFps   = CAM2_FPS;
    cam2.pid         = CAM2_PID;
    cam2.vid         = CAM2_VID;
    cam2.cb          = cam2_callback;
    init_uvc_camera_device(&cam2);
    start_uvc_camera_device(&cam2);

    
    //opengl cfg
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //window2
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(CAM2_PIXEL_WIDTH, CAM2_PIXEL_HEIGHT);
    glWindow2 = glutCreateWindow("OpenGL glShowRGB_CAM2");
    glutSetWindow(glWindow2);
    glutIdleFunc(idle_global);  
    glutMainLoop();
    //

    free_uvc_camera_device(&cam2);
}