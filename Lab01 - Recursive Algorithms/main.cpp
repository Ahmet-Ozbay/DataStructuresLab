#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <iomanip>

using namespace std;

struct Pixel {
    int x;
    int y;
};

/**
* Function to count the blobs.
* @param input is 2d vector.
* @param i is the number of rows vector has.
* @param j is the number of columns vector has.
*/
int blobCounter(vector<vector<char>>& input, int i, int j) {

    // Determine the number of rows and columns of the vector.
    int row = input.size();
    int col = input[0].size();

    // Check for empty pixels.
    if (i < 0 || i >= row || j < 0 || j >= col || input[i][j] == ' ')
        return 0;

    input[i][j] = ' ';
    int count = 1;

    // Recursively look up for the neighboring cells.
    count += blobCounter(input, i + 1, j);
    count += blobCounter(input, i - 1, j);
    count += blobCounter(input, i, j + 1);
    count += blobCounter(input, i, j - 1);

    // Return the number of blobs found in the vector.
    return count;
}

/**
* Function to count pixels.
* @param input is the vector to look up for "x" characters.
* @param i is the number of rows the vector has.
* @param j is the number of columns the vector has.
* @param pixelCoordinates is the array which the x characters' coordinates are stored.
*/
int pixelCounter(vector<vector<char>>& input, int i, int j, vector<Pixel>& pixelCoordinates) {

    // Initial number of pixels.
    int count = 0;

    // Check if there are pixels.
    if (i < 0 || i >= input.size() || j < 0 || j >= input[i].size() || input[i][j] != 'x')
        return 0;

    input[i][j] = ' ';
    count = 1;

    // Send the coordinates of the fount pixel.
    Pixel pixel;
    pixel.x = i;
    pixel.y = j;
    pixelCoordinates.push_back(pixel);

    // Use recursion to check neighboring cells.
    count += pixelCounter(input, i + 1, j, pixelCoordinates);
    count += pixelCounter(input, i - 1, j, pixelCoordinates);
    count += pixelCounter(input, i, j + 1, pixelCoordinates);
    count += pixelCounter(input, i, j - 1, pixelCoordinates);

    // Return the number of pixels.
    return count;
}

/**
* Function that calculates the center of mass for the row.
* @param pixels is the array where the pixel coordinates are stored.
*/
double rowCenterOfMass(vector<Pixel>& pixels) {

    // Initial sum of the coordinates.
    int sum = 0;

    // Sum the row coordinates of each pixel
    for (const auto& pixel : pixels) {
        sum += pixel.x;
    }

    // Divide the sum by the number of pixels and return center of mass.
    return static_cast<double>(sum) / pixels.size();
}

/**
* Function that calculates the center of mass for the column.
* @param pixels is the array where the pixel coordinates are stored.
*/
double colCenterOfMass(vector<Pixel>& pixels) {

    // Initial sum of the coordinates.
    int sum = 0;

    // Sum the row coordinates of each pixel
    for (const auto& pixel : pixels) {
        sum += pixel.y;
    }

    // Divide the sum by the number of pixels and return center of mass.
    return static_cast<double>(sum) / pixels.size();
}

/**
* Function that asks user the name of the file.
*/
string getFileName() {
    string fileName;
    cout << "Enter the name of the file (with extension): ";
    cin >> fileName;
    return fileName;
}

/**
* Function that prints out the horizontal borders.
* @param n : The number of columns.
*/
void horizontalBorder(int n) {
    for (int i = 0; i < n; i++) {
        cout << "-";
    }
}

/**
* Function that prints out the coordinate numbers.
* @param n : How many numbers we want to print. Use the number rows or columns.
*/
void indexer(int n) {
    int count = 0;

    while (count < n) {
        for (int i = 0; i <= 9 && count < n; i++) {
            cout << i;
            count++;
        }
    }
}

/**
* Function that prints the input file.
* @param fileName is the name of the text file to be opened and printed to the screen.
*/
void printFile(string fileName) {

    // Initialize variables to start printing the file.
    int lineIndex = -1;
    bool isFirstLine = true;
    string line;

    // Open the file
    fstream file(fileName);

    // Start printing the file line by line.
    while (getline(file, line)) {

        // Don't print left and right borders if it is the first line.
        if (isFirstLine) {
            cout << endl;
            isFirstLine = false;
        }
        else {

            // Starting from the second line, print index(coordinate) and borders at the beginning and the end of the line.
            cout << lineIndex << "|" << line << "|" << lineIndex << endl;
        }

        lineIndex++;

        // Make sure to only print numbers from 0 to 9.
        if (lineIndex == 10) {
            lineIndex = 0;
        }
    }
}

/**
* Function to draw top half of the border.
* @param col is the number of columns the table has.
*/
void drawTop(int col) {

    // Write top-index numbers.
    cout << "  ";
    indexer(col);
    cout << endl;

    // Draw the top-border.
    cout << " +";
    horizontalBorder(col);
    cout << "+";
}

/*
* Function to draw the bottom half of the border.
* @param col is the number of columns the table has.
*/
void drawBottom(int col) {
    // Draw the bottom border.
    cout << " +";
    horizontalBorder(col);
    cout << "+" << endl;

    // Write bottom-index numbers.
    cout << "  ";
    indexer(col);
    cout << endl << endl;
}

int checkErrors(int r, int c, vector<vector<char>> arr) {
    if (r <= 0 || c <= 0 || r != arr.size() || c != arr[0].size()) {
        cerr << "Error: Mismatch between provided dimensions and actual input array." << endl;
        cerr << "Make sure that the first line of the file consists the exact number of rows and columns." << endl;
        return 1; // Exit the program gracefully
    }
}

int main() {

    // Define variables.
    int row;
    int col;
    string content;
    vector<vector<char>> inputArray;

    // Prompt user for the file name.
    string fileName = getFileName();

    // Open the specified file.
    fstream file(fileName);

    // Check if file exist, throw an error if file can't be opened.
    if (file.is_open()) {

        // Get the # of rows and # of columns from the first line of the file.
        file >> row;
        file >> col;

        // Print out the amount rows and columns.
        cout << "Row: " << row << endl;
        cout << "Column: " << col << endl << endl;

        // Skip the first line
        getline(file, content);

        // Read and store the rest of the lines in inputArray
        while (getline(file, content)) {
            vector<char> row(content.begin(), content.end());
            inputArray.push_back(row);
        }

        // Close the file
        file.close();
    }
    else {

        // Show the error.
        cerr << "The file doesn't exist or it's corrupt." << endl;
        cerr << "Make sure you enter the right name and file extension." << endl; 
        cerr << "Make sure to give the exact path of the file if it's not within the same directory as the program." << endl << endl;
        return 1;
    }

    // Print the file within a border
    drawTop(col);
    printFile(fileName);
    drawBottom(col);

    // Check if the input file gives correct number of rows and columns before proceeding with the calculations.
    if (checkErrors(row, col, inputArray) == 1) {

        // Exit the program.
        return 1;
    };

    // Table format
    cout << setw(10) << "Blob" << setw(10) << "Pixels" << setw(10) << "CoM Row" << setw(10) << "CoM Col" << endl;

    // Start calculating the # of blobs inside the array.
    // Initialize blob count.
    int blobCount = 0;

    // Loop through the rows.
    for (int i = 0; i < row; ++i) {

        // Loop through the columns. 
        for (int j = 0; j < col; ++j) {

            // Count the pixels.
            if (inputArray[i][j] == 'x') {
                vector<Pixel> pixels;
                int pixelsInBlob = pixelCounter(inputArray, i, j, pixels);

                // Print out the amount of pixels.
                cout << setw(10) << ++blobCount << setw(8) << pixelsInBlob;

                // Calculate the center of mass for the row.
                double rowCenter = rowCenterOfMass(pixels);

                if (rowCenter >= 0) {

                    // Print out the CoM Row.
                    cout << setw(10) << setprecision(4) << rowCenter;
                }
                else {

                    // Throw an error if it can't be calculated. 
                    cout << "Row's Center of Mass for Blob (CoM Row)" << blobCount << " could not be calculated." << endl;
                }

                // Calculate the center of mass for the column.
                double colCenter = colCenterOfMass(pixels);

                if (colCenter >= 0) {

                    // Print out the CoM Col.
                    cout << setw(10) << setprecision(4) << colCenter << endl;
                }
                else {

                    // Throw an error if it can't be calculated
                    cout << "Column's Center of Mass for Blob (CoM Col)" << blobCount << " could not be calculated." << endl;
                }
            }
        }
    }
    return 0;
}