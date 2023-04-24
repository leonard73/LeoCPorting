#include <GL/glut.h>
#define TEST_PIXEL_WIDTH   (640)
#define TEST_PIXEL_HEIGHT  (480)
#define TEST_PIXEL_CHANNEL (3)
#define TEST_PIXEL_NUM    (TEST_PIXEL_WIDTH*TEST_PIXEL_HEIGHT)
GLubyte pixel_array[TEST_PIXEL_NUM*TEST_PIXEL_CHANNEL];
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    for(int i=0;i<TEST_PIXEL_NUM;i++)
    {
        pixel_array[i*TEST_PIXEL_CHANNEL+0] = 0;
        pixel_array[i*TEST_PIXEL_CHANNEL+1] = 255;
        pixel_array[i*TEST_PIXEL_CHANNEL+2] = 0;
    }
    return;
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(TEST_PIXEL_WIDTH,TEST_PIXEL_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,pixel_array);
    glFlush();
    return;
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(TEST_PIXEL_WIDTH, TEST_PIXEL_HEIGHT);
    glutCreateWindow("OpenGL glShowRGB");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
