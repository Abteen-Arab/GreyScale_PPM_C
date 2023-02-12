#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Defines the magic number for the PPM file format
#define PPM_MAGIC_NUMBER "P3"

//Defines the structure for the RGB values of each pixel
typedef struct {
    int r;
    int g;
    int b;
} Pixel;

//Function to apply the black and white filter to an image
void black_and_white_filter(const char *input_file, const char *output_file) {
    char magic_number[3];
    int width, height, max_color;
    int i, j;

    //Open the input file for reading
    FILE *fp_in = fopen(input_file, "r");
    if (!fp_in) {
        //Error message if the input file cannot be opened
        printf("Failed to open input file\n");
        return;
    }

    //Read the magic number, width, height, and maximum color value from the input file
    fscanf(fp_in, "%s %d %d %d", magic_number, &width, &height, &max_color);

    //Allocate memory for the pixels
    Pixel *pixels = (Pixel *) malloc(width * height * sizeof(Pixel));
    if (!pixels) {
        //Error message if memory allocation fails
        printf("Failed to allocate memory for pixels\n");
        fclose(fp_in);
        return;
    }

    //Read the RGB values for each pixel from the input file
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fscanf(fp_in, "%d %d %d", &pixels[i * width + j].r, &pixels[i * width + j].g, &pixels[i * width + j].b);
        }
    }

    //Close the input file
    fclose(fp_in);

    //Convert each pixel to grayscale
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int gray = round(0.21 * pixels[i * width + j].r + 0.72 * pixels[i * width + j].g + 0.07 * pixels[i * width + j].b);
            pixels[i * width + j].r = gray;
            pixels[i * width + j].g = gray;
            pixels[i * width + j].b = gray;
        }
    }

    //Open the output file for writing
    FILE *fp_out = fopen(output_file, "w");
    if (!fp_out) {
        //Error message if the output file cannot be opened
        printf("Failed to open output file\n");
        free(pixels);
        return;
    }
    
    if (max_color < 0 || max_color > 255) {
        printf("Invalid maximum color value\n");
        fclose(fp_in);
        return;
    }
    
    //Write the magic number, width, height, and maximum color value to the output file
    fprintf(fp_out, "%s\n%d %d\n%d\n", PPM_MAGIC_NUMBER, width, height, max_color);

    //Copies the changd image to the output image file.
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            fprintf(fp_out, "%d %d %d\n", pixels[i * width + j].r, pixels[i * width + j].g, pixels[i * width + j].b);
        }
    }

    fclose(fp_out);
    free(pixels);
}

int main(int argc, const char * argv[]) {
    if (argc != 3) {
        //Error message if the number of command line arguments is not correct
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    
    black_and_white_filter(argv[1], argv[2]);
    return 0;
}
