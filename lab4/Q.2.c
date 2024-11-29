#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 512
#define HEIGHT 512

// Function to read a PGM (Portable GrayMap) image in text format.
// 'pixels' stores the pixel values of the image specified by 'filename'.
// The image dimensions (width and height) are passed as parameters.
// Returns 0 on success, -1 on failure.
int readPGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "r");  // Open the PGM file in read mode.
    if (!file) {
        perror("Error opening file");  // If the file can't be opened, display an error message.
        return -1; // Return -1 to indicate failure.
    }

    // Allocate memory to store the file format (P2 for text-based PGM).
    char *format = malloc(3);  
    if (!format) {
        fclose(file);  
        perror("Error allocating memory for format");
        return -1;  // Return -1 to indicate failure in memory allocation.
    }

    // Check if the file starts with "P2", indicating a valid text-based PGM image.
    if (fscanf(file, "%2s", format) != 1 || strcmp(format, "P2") != 0) {
        free(format);  
        fclose(file);  
        fprintf(stderr, "Error: Not a valid PGM file (expected P2 format).\n");
        return -1;  // Return -1 if the file format is incorrect.
    }
    free(format);  // Free the memory allocated for the format string.

    // Read the width and height from the file.
    int scanned = 0;
    while ((scanned = fscanf(file, "%d %d", &width, &height)) != 2) {
        if (scanned == EOF) {
            fclose(file);  
            fprintf(stderr, "Error: Invalid image dimensions.\n");
            return -1;  // Return -1 if dimensions can't be read.
        }
        fgetc(file);  // Skip any non-numeric characters such as comments.
    }

    // Read the maximum gray value (expected to be 255).
    int maxGray;
    if (fscanf(file, "%d", &maxGray) != 1 || maxGray != 255) {
        fclose(file);  
        fprintf(stderr, "Warning: Max gray value is %d, expected 255.\n", maxGray);
    }

// 'pixels' stores the pixel values of the image specified by 'filename'.
    int pixelCount = 0;
    while (fscanf(file, "%hhu", pixels + pixelCount) == 1) {
        pixelCount++;  // Increment pixel count for each pixel read.
        if (pixelCount >= width * height) {
            break;  // Stop reading once all pixel values have been read.
        }
    }

    fclose(file);  // Close the file after reading.
    return 0;  // Return 0 to indicate success.
}

// Function to write a PGM image in text format.
// 'pixels' stores the pixel values of the image specified by 'filename'.
// The image dimensions (width and height) are passed as parameters.
int writePGMText(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "w");  // Open the file for writing in text mode.
    if (!file) {
        perror("Error opening file for writing");  // If file can't be opened, display an error.
        return -1;  // Return -1 to indicate failure.
    }

    // Write the PGM header and image dimensions to the file.
    fprintf(file, "P2\n# Created by Steganography Program\n%d %d\n255\n", width, height);

    // Write each pixel value, formatting the output with a newline every row.
    for (int i = 0; i < width * height; i++) {
        fprintf(file, "%d ", *(pixels + i));  // Write the pixel value.
        if ((i + 1) % width == 0) {
            fprintf(file, "\n");  // Start a new line after every row of pixels.
        }
    }

    fclose(file);  // Close the file after writing.
    return 0;  // Return 0 to indicate success.
}

// Function to write a PGM image in binary format (P5 format).
// It writes pixel values to the file in binary.
int writePGMBinary(const char *filename, unsigned char *pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");  // Open the file for binary writing.
    if (!file) {
        perror("Error opening file for writing");  // If file can't be opened, display an error.
        return -1;  // Return -1 to indicate failure.
    }

    // Write the PGM header and image dimensions to the file.
    fprintf(file, "P5\n# Created by Steganography Program\n%d %d\n255\n", width, height);

    // Write all pixel data in binary format.
    fwrite(pixels, sizeof(unsigned char), width * height, file);

    fclose(file);  // Close the file after writing.
    return 0;  // Return 0 to indicate success.
}

// Function to embed a secret image inside a cover image using 4-bit LSB (Least Significant Bit) steganography.
// The secret image is encoded into the cover image by modifying the least significant bits of each pixel.
void embedLSB(unsigned char *coverPixels, unsigned char *secretPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // Embed the secret image's 4 most significant bits into the least significant bits of the cover image.
        *(coverPixels + i) = (*(coverPixels + i) & 0xF0) | ((*(secretPixels + i) >> 4) & 0x0F);
    }
}

// Function to extract the secret image from a stego image using 4-bit LSB steganography.
// The secret image is retrieved by isolating the least significant bits embedded in the cover image.
void extractLSB(unsigned char *coverPixels, unsigned char *outputPixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // Extract the 4 least significant bits and shift them to the 4 most significant bits.
        *(outputPixels + i) = (*(coverPixels + i) & 0x0F) << 4;
    }
}

int main() {
    // Declare the filenames for the cover image, secret image, stego image, and the extracted secret image.
    const char *cover_image = "baboon.pgm";       // Cover image file name.
    const char *secret_image = "farm.pgm";        // Secret image file name.
    const char *stego_image = "stego_image_bin.pgm";  // Stego image file name where secret will be embedded.
    const char *extracted_secret = "extracted_secret.pgm";  // Extracted secret image file name.

    // Declare variables for storing pixel data of cover, secret, and output (extracted) images.
    unsigned char *coverPixels = NULL;  
    unsigned char *outputPixels = NULL;  
    int coverWidth = WIDTH, coverHeight = HEIGHT, secretWidth = WIDTH, secretHeight = HEIGHT;

    // Allocate memory to store the pixel values of the cover image.
    coverPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (!coverPixels) {
        fprintf(stderr, "Memory allocation error for coverPixels.\n");
        return 1;  // Return 1 if memory allocation fails.
    }

    // Read the cover image and store its pixel data.
    if (readPGMText(cover_image, coverPixels, coverWidth, coverHeight) != 0) {
        fprintf(stderr, "Error reading cover image.\n");
        free(coverPixels);  
        return 1;  // Return 1 if reading the cover image fails.
    }

    // Allocate memory to store the pixel values of the secret image.
    unsigned char *secretPixels = (unsigned char *)malloc(WIDTH * HEIGHT);  
    if (!secretPixels) {
        fprintf(stderr, "Memory allocation error for secretPixels.\n");
        free(coverPixels);  
        return 1;  // Return 1 if memory allocation fails.
    }

    // Read the secret image and store its pixel data.
    if (readPGMText(secret_image, secretPixels, secretWidth, secretHeight) != 0) {
        fprintf(stderr, "Error reading secret image.\n");
        free(coverPixels);  
        free(secretPixels);  
        return 1;  // Return 1 if reading the secret image fails.
    }

    // Check if the dimensions of the cover and secret images match.
    if (coverWidth != secretWidth || coverHeight != secretHeight) {
        fprintf(stderr, "Error: Image dimensions do not match.\n");
        free(coverPixels);  
        free(secretPixels);  
        return 1;  // Return 1 if dimensions do not match.
    }

    // Embed the secret image into the cover image.
    embedLSB(coverPixels, secretPixels, coverWidth, coverHeight);

    // Write the resulting stego image to a file.
    if (writePGMBinary(stego_image, coverPixels, coverWidth, coverHeight) != 0) {
        fprintf(stderr, "Error writing stego image.\n");
        free(coverPixels);  
        free(secretPixels);  
        return 1;  // Return 1 if writing the stego image fails.
    }

    // Allocate memory for storing the extracted secret image's pixel data.
    outputPixels = (unsigned char *)malloc(WIDTH * HEIGHT);
    if (!outputPixels) {
        fprintf(stderr, "Memory allocation error for outputPixels.\n");
        free(coverPixels);  
        free(secretPixels);  
        return 1;  // Return 1 if memory allocation fails.
    }

    // Extract the secret image from the stego image.
    extractLSB(coverPixels, outputPixels, coverWidth, coverHeight);

    // Write the extracted secret image to a file.
    if (writePGMText(extracted_secret, outputPixels, coverWidth, coverHeight) != 0) {
        fprintf(stderr, "Error writing extracted secret image.\n");
        free(coverPixels);  
        free(secretPixels);  
        free(outputPixels);  
        return 1;  // Return 1 if writing the extracted image fails.
    }

    // Free all allocated memory.
    free(coverPixels);
    free(secretPixels);
    free(outputPixels);

    // Print a success message.
    printf("Steganography process completed successfully.\n");
    return 0;
}
