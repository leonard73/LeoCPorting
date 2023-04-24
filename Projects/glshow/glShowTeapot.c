#include <GL/glut.h>
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-5, 5, -5, 5, 5, 15);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    return;
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0);
    glutWireTeapot(3);
    glFlush();
    return;
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL HelloWorld");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
