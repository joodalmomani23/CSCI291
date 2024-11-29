#include <iostream>  // Includes the standard input/output stream library
#include <iomanip>   // Includes the library for input/output manipulators like fixed, setw, and setprecision

using namespace std;

// Section A: Function to print a number in fixed-point notation with specific field width and precision
// This function takes a floating-point number, the desired field width, and the precision for printing
void print_real(float number, int fieldspace, int precision) {
    // 'fixed' forces the output to use fixed-point notation
    // 'setw(fieldspace)' sets the minimum width of the printed field
    // 'setprecision(precision)' sets the number of digits to print after the decimal point
    cout << fixed << setw(fieldspace) << setprecision(precision) << number << endl;
}

// Section B: Template function to update two variables using arithmetic operations and scaling
// This is a template function that can work with any data type (T) for the input variables
// It takes three parameters: m1 and m2 (which will be updated) and m3 (the scaling factor with a default value of 10)
template <typename T>
void update_scale(T &m1, T &m2, T m3 = 10) {
    // Store the original values of m1 and m2 for later calculations
    T original_m1 = m1;
    T original_m2 = m2;

    // Update m1 by adding original_m1 and original_m2, then multiplying the result by m3
    m1 = (original_m1 + original_m2) * m3;

    // Update m2 by subtracting original_m2 from original_m1, then multiplying the result by m3
    m2 = (original_m1 - original_m2) * m3;
}

int main(void) {
    float a, b;  // Declare two floating-point variables a and b

    // Section A: Prompt user for input for the two real numbers
    cout << "Please input two real numbers: ";
    cin >> a >> b;  // Read the values into a and b

    // First call to print_real to display the values of a and b with a field width of 7 and precision of 3
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    // Section B: Call the update_scale function to modify the values of a and b
    // m3 defaults to 10, so no need to pass it explicitly
    update_scale(a, b);

    // Second call to print_real to display the updated values of a and b after the update_scale function
    print_real(a, 7, 3);
    print_real(b, 7, 3);

    return 0;  // Return 0 to indicate successful execution
}
