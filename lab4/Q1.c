#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if a string represents a valid float
int is_valid_float(const char *str) {
    char *end;  // Pointer to track where the float parsing stops
    strtof(str, &end);  // Attempt to convert the string to a float, and `end` will point to the first invalid character
    return *end == '\0';  // If the end pointer points to the null terminator, it means the string was entirely a valid float
}

int main() {
    FILE *sourceFile, *outputFile;  // File pointers for the source (input) and output files
    char buffer[100];  // Buffer to store a single line of the input file
    int invalidCount = 0;  // Counter to track the number of invalid floats encountered

    // Attempt to open the source file in read mode
    sourceFile = fopen("data.txt", "r");
    if (!sourceFile) {  // If the file could not be opened (e.g., it does not exist)
        perror("Error opening source file");  // Print a system error message
        return 1;  // Exit with an error code
    }
    printf("Source file opened successfully.\n");

    // Attempt to open the output file in write mode
    outputFile = fopen("output.txt", "w");
    if (!outputFile) {  // If the output file could not be opened (e.g., permission issues)
        perror("Error opening output file");  // Print a system error message
        fclose(sourceFile);  // Close the already opened source file before exiting
        return 1;  // Exit with an error code
    }
    printf("Output file opened successfully.\n");

    // Check if the source file is empty by seeking to the end and checking the file size
    fseek(sourceFile, 0, SEEK_END);  // Move the file pointer to the end of the file
    if (ftell(sourceFile) == 0) {  // If the file size is 0, the file is empty
        printf("Source file is empty.\n");
        fclose(sourceFile);  // Close the source file
        fclose(outputFile);  // Close the output file
        return 1;  // Exit with an error code
    }
    fseek(sourceFile, 0, SEEK_SET);  // Reset the file pointer to the start of the file to begin reading

    // Process each line from the source file
    while (fgets(buffer, sizeof(buffer), sourceFile)) {  // Read one line at a time into the buffer
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove the newline character at the end of the line (if present)

        // Tokenize the line by spaces (i.e., split the line into words)
        char *token = strtok(buffer, " ");  // Get the first token
        while (token) {  // Continue processing as long as there are tokens
            if (is_valid_float(token)) {  // If the token is a valid float
                fprintf(outputFile, "%s\n", token);  // Write the valid float to the output file
            } else {
                invalidCount++;  // If not a valid float, increment the invalid count
            }
            token = strtok(NULL, " ");  // Get the next token
        }

        // Check if there was an error while reading the file
        if (ferror(sourceFile)) {  // If a read error occurred
            perror("Error reading the source file.");  // Print a system error message
            break;  // Exit the loop to stop reading further
        }
    }

    // Verify if the end of file (EOF) was reached properly after the reading process
    if (feof(sourceFile)) {  // If EOF was reached correctly
        printf("End of file reached.\n");
    } else {
        printf("Unexpected error: End of file not reached.\n");  // If the EOF wasn't reached, there was an issue
    }

    // Close the source file and check if it was closed successfully
    if (fclose(sourceFile) == 0) {  // If the file was closed successfully
        printf("Source file closed.\n");
    } else {
        perror("Error closing source file");  // If there was an issue closing the source file, print an error
    }

    // Close the output file and check if it was closed successfully
    if (fclose(outputFile) == 0) {  // If the file was closed successfully
        printf("Output file closed.\n");
    } else {
        perror("Error closing output file");  // If there was an issue closing the output file, print an error
    }

    // Print the count of invalid floats found in the input file
    printf("Invalid float count: %d\n", invalidCount);

    return 0;  // Exit the program successfully
}
