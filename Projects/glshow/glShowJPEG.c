#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <GL/glut.h>

// Define structure to hold RGB data
typedef struct {
    unsigned char *data;
    int width;
    int height;
} RGBImage;

// Function to decode JPEG file and extract RGB data
RGBImage decode_jpeg(const char *filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Initialize error handling
    cinfo.err = jpeg_std_error(&jerr);

    // Initialize decompression object
    jpeg_create_decompress(&cinfo);

    // Open input file
    FILE *infile = fopen(filename, "rb");
    if (!infile) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    // Specify input file
    jpeg_stdio_src(&cinfo, infile);

    // Read JPEG header
    jpeg_read_header(&cinfo, TRUE);

    // Start decompression
    jpeg_start_decompress(&cinfo);

    // Allocate memory for RGB data
    RGBImage rgbImage;
    rgbImage.width = cinfo.output_width;
    rgbImage.height = cinfo.output_height;
    rgbImage.data = (unsigned char *)malloc(cinfo.output_width * cinfo.output_height * cinfo.output_components);
    if (!rgbImage.data) {
        fprintf(stderr, "Error allocating memory for image data\n");
        exit(EXIT_FAILURE);
    }

    // Read scanlines and decode image
    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char *buffer = rgbImage.data + cinfo.output_scanline * cinfo.output_width * cinfo.output_components;
        jpeg_read_scanlines(&cinfo, &buffer, 1);
    }

    // Finish decompression
    jpeg_finish_decompress(&cinfo);

    // Close input file
    fclose(infile);

    // Destroy decompression object
    jpeg_destroy_decompress(&cinfo);

    return rgbImage;
}

RGBImage image;
GLuint textureID; // Texture ID

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);

    // Render a textured quad covering the entire viewport
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(-1, -1);  // Bottom-left
    glTexCoord2f(1, 1); glVertex2f(1, -1);   // Bottom-right
    glTexCoord2f(1, 0); glVertex2f(1, 1);    // Top-right
    glTexCoord2f(0, 0); glVertex2f(-1, 1);   // Top-left
    glEnd();

    glutSwapBuffers();
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file.jpg>\n", argv[0]);
        return 1;
    }

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("RGB Image Viewer");
    glutDisplayFunc(display);

    // Decode JPEG image and extract RGB data
    image = decode_jpeg(argv[1]);

    // Set up OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Enter GLUT event loop
    glutMainLoop();

    // Free allocated memory
    free(image.data);

    return 0;
}
