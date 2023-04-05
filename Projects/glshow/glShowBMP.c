#include <GL/glut.h>
#include "bmp.h"
GLubyte *pixel_array;
uint32_t TEST_PIXEL_WIDTH_DEFAULT     = 640;
uint32_t TEST_PIXEL_HEIGHT_DEFAULT    = 480;
uint32_t TEST_PIXEL_CHANNEL_DEFAULT   = 3;
uint32_t TEST_PIXEL_NUM_DEFAULT       = 640*480;
void init(char * path_bmp)
{
    //read bmp bgr888
    BMP_DATA  readBmp;
    createBmpDataFromFile(&readBmp,path_bmp);
    uint32_t imgWidth  = (&readBmp)->bmpInfoHeader.biWidth;
    uint32_t imgHeight = (&readBmp)->bmpInfoHeader.biHeight;
    uint8_t *imgPtr    = (&readBmp)->dataPtr;
    uint32_t pixelS    = (&readBmp)->bmpInfoHeader.biBitCount>>3;
    TEST_PIXEL_WIDTH_DEFAULT = imgWidth;
    TEST_PIXEL_HEIGHT_DEFAULT= imgHeight;
    TEST_PIXEL_NUM_DEFAULT   = TEST_PIXEL_WIDTH_DEFAULT * TEST_PIXEL_HEIGHT_DEFAULT;
    pixel_array =(GLubyte*)malloc(TEST_PIXEL_NUM_DEFAULT*TEST_PIXEL_CHANNEL_DEFAULT);
    for(int i=0;i<TEST_PIXEL_NUM_DEFAULT;i++)
    {
        pixel_array[i*TEST_PIXEL_CHANNEL_DEFAULT+0] = imgPtr[i*3+2];
        pixel_array[i*TEST_PIXEL_CHANNEL_DEFAULT+1] = imgPtr[i*3+1];
        pixel_array[i*TEST_PIXEL_CHANNEL_DEFAULT+2] = imgPtr[i*3+0];
    }
    releaseBmpData(&readBmp);
    return;
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2i(-1,1);
    glPixelZoom(1.0f,-1.0f);
    glDrawPixels(TEST_PIXEL_WIDTH_DEFAULT,TEST_PIXEL_HEIGHT_DEFAULT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array);
    glFlush();
    return;
}
int main(int argc, char *argv[])
{
    init(argv[1]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(TEST_PIXEL_WIDTH_DEFAULT, TEST_PIXEL_HEIGHT_DEFAULT);
    glutCreateWindow("OpenGL glShowBMP");
    glutDisplayFunc(display);
    glutMainLoop();
    free(pixel_array);
    return 0;
}
