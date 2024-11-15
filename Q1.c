#include <stdio.h>
#include <stdbool.h>

// FUNCTION PROTOTYPES

// Checks if a given position in an array is valid
bool isValid(const int arr[], int length, int pos);

// Removes an element from an array at a specified position
void remove_element(int arr[], int length, int pos);

// Inserts an element into an array at a specified position
void insert_element(int arr[], int length, int pos, int value);

// Checks if there is a duplicate element in the array
bool found_duplicate(int arr[], int length);

// Reshapes a 1D array into a 2D array format
void reshape(const int arr[], int length, int nRows, int nCols, int arr2d[nRows][nCols]);

// Transposes a given matrix
void trans_matrix(int nRows, int nCols, const int mat[nRows][nCols], int mat_transp[nCols][nRows]);

int main() { 
    // Initialize an array with six elements
    int arr[6] = {10, 20, 30, 40, 50, 0};
    int length = 6;

    // Print the original array
    printf("Original array:\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Insert an element (80) at position 2
    insert_element(arr, length, 2, 80);

    // Remove an element at position 3
    remove_element(arr, length, 3);

    // Reshape a 1D array to a 2D array
    int arr1d[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int length1d = 12;
    int nRows = 3;
    int nCols = 4;
    int arr2d[3][4];
    reshape(arr1d, length1d, nRows, nCols, arr2d);

    // Transpose a 3x4 matrix
    int mat[3][4] = { {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12} };
    int mat_transp[4][3];
    trans_matrix(3, 4, mat, mat_transp);

    // Check for duplicates in an array
    int arr_with_duplicates[] = {1, 2, 3, 4, 5, 3};
    if (found_duplicate(arr_with_duplicates, 6)) {
        printf("Duplicate found in the array.\n");
    } else {
        printf("No duplicates found in the array.\n");
    }

    return 0;
}

// FUNCTION DEFINITIONS

// Checks if the given position is within a valid range for the array
bool isValid(const int arr[], int length, int pos) {
    return (pos > 0 && pos < length); // Ensures pos is within range and not the first element
}

// Removes an element by shifting elements to the left from a given position
void remove_element(int arr[], int length, int pos) {
    // Ensure pos is a valid index and not the first element
    if (!isValid(arr, length, pos)) {
        printf("Invalid index. Cannot remove the element at index %d.\n", pos);
        return;
    }

    // Shift elements to remove the element at specified pos
    for (int i = pos; i > 0; i--) {
        arr[i] = arr[i - 1];
    }

    // Print updated array for verification
    printf("Array after shifting elements before index %d:\n", pos);
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Inserts an element at a specified position by shifting elements
void insert_element(int arr[], int length, int pos, int value) {
    // Check if pos is within the valid range
    if (!isValid(arr, length, pos)) {
        printf("Invalid index. Cannot insert element at index %d.\n", pos);
        return;
    }

    // Shift elements to make space for the new element
    for (int i = 0; i < pos; i++) {
        arr[i] = arr[i + 1]; // Shift elements to the right
    }

    // Insert the new element at the specified position
    arr[pos] = value;

    // Print updated array for verification
    printf("Array after inserting %d at index %d:\n", value, pos);
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Checks for duplicates in the array
bool found_duplicate(int arr[], int length) {
    for (int i = 0; i < length; ++i) {
        // Check if position is valid
        if (isValid(arr, length, i)) {
            // Compare the current element with all subsequent elements
            for (int j = i + 1; j < length; ++j) {
                if (arr[i] == arr[j]) {
                    return true; // Duplicate found
                }
            }
        }
    }
    return false; // No duplicates found
}

// Converts a 1D array into a 2D array
void reshape(const int arr[], int length, int nRows, int nCols, int arr2d[nRows][nCols]) {
    // Check if 1D array size matches the 2D array requirements
    if (length != nRows * nCols) {
        printf("Error: 1D array size (%d) does not match the required size (%d) for a %d x %d matrix.\n", length, nRows * nCols, nRows,
         nCols);
        return;
    }

    int index = 0;
    // Convert the 1D array to 2D (column-major order)
    for (int col = 0; col < nCols; col++) {
        for (int row = 0; row < nRows; row++) {
            arr2d[row][col] = arr[index++];
        }
    }

    // Print the 2D array
    printf("Converted 2D array (column-major order):\n");
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            printf("%d ", arr2d[i][j]);
        }
        printf("\n");
    }
}

//Transposes a matrix and prints the result
void trans_matrix(int nRows, int nCols, const int mat[nRows][nCols], int mat_transp[nCols][nRows]) {
    // Perform matrix transpose
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            mat_transp[j][i] = mat[i][j];
        }
    }

    // Print the transposed matrix
    printf("Transposed matrix:\n");
    for (int i = 0; i < nCols; i++) {
        for (int j = 0; j < nRows; j++) {
            printf("%d ", mat_transp[i][j]);
        }
        printf("\n");
    }
}
