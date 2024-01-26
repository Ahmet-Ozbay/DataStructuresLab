#include <stdio.h>
#include "Poly.h"
#include <sstream>
#include <cmath>
#include <iostream>

using namespace std;

string removeSpaces(char* expr) {
    // Convert the input array into a string
    string expression(expr);

    // Set up a temporary string
    string temp;

    // Iterate over the string
    for (int i = 0; i < expression.size(); i++) {

        // Remove spaces and store the new string in temp
        if (expression[i] != ' ') {

            temp += expression[i];
        }
    }
    // Set the expression to temp
    expression = temp;
    return expression;
}

//-------------------------------------------------
// Creates a polynomial from an expression.
// Returns a pointer to the first PolyNode in the list (head of the list)
//
// Expression will be of the following sort:
// Ex1: 2.3x^4 + 5x^3 - 2.64x - 4
// Ex2: -4.555x^10 - 45.44
// Ex3: x^6 + 2x^4 - x^3 - 6.3x + 4.223 
// Ex4: 34
// Ex5: -x+1
// Ex6: -3x^4    +   4x
// Ex7: -2x  - 5
//
PolyNode* CreatePoly(char* expr) {

    // Initialize the pointer
    struct PolyNode* head = NULL;

    // Remove spaces from the array
    string expression = removeSpaces(expr);

    // Add a positive sign if the first character is positive x.
    if (expression[0] != '-' && expression[0] == 'x') {

        expression = "+" + expression;
    }


    // Iterate over the expression
    for (int i = 0; i < expression.size(); i++) {

        bool isNegative;

        if (expression[i] == '-') {
            isNegative = true;
        }
        else {
            isNegative = false;
        }


        // Initialize empty strings 'coef' and 'exp'
        string coef = "";
        string exp = "";

        // Move forward
        i++;

        // Get the coefficient up to the x, or to the end of the expression
        while (expression[i] != 'x' && expression[i] != NULL) {

            coef += expression[i];
            i++;
        }

        // Default coefficient is 1, if nothing specified
        if (coef == "-" || coef == "+" || coef == "") {

            coef = "1";
        }

        // Convert the coefficient
        double coefficient = stod(coef);

        // Don't forget the signs
        if (isNegative) {

            coefficient *= -1.0;
        }

        // Initialize the new node
        struct PolyNode* term, * ptr;

        // Check if it is the end of the string
        if (expression[i] == NULL) {

            // Check if the list is empty
            if (head == NULL) {

                // Allocate memory for a new PolyNode and set its values
                term = new PolyNode[sizeof(PolyNode)];
                term->coef = coefficient;
                term->exp = 0;
                term->next = NULL;
                head = term;
            }
            else {  // Add the new node to the end

                // Point the head of the list
                ptr = head;

                // Go to the end of the list
                while (ptr->next != NULL) {

                    ptr = ptr->next;
                }

                // Create the term node and set its values
                term = new PolyNode[sizeof(PolyNode)];
                term->coef = coefficient;
                term->exp = 0;
                term->next = NULL;
                ptr->next = term;
            }   break;
        }

        // Move forward to the next character
        i++;

        // Check for the exponent sign
        if (expression[i] == '^') {

            i++;
        }

        // Extract the exponent
        while (expression[i] != NULL && expression[i] != '-' && expression[i] != '+')
        {
            exp += expression[i];
            i++;
        }

        // Make sure the exponent is correct,
        // If it's empty or negative, set it to 1.
        if (exp == "-" || exp == "+" || exp == "") {

            exp = "1";
        }

        // Convert exponent to an integer
        int exponent = stoi(exp);

        // Create a new node if list is empty
        if (head == NULL)
        {
            term = new PolyNode[sizeof(PolyNode)];
            term->coef = coefficient;
            term->exp = exponent;
            term->next = NULL;
            head = term;
        }
        else { // Add to the end of the list

            // Point to the head
            ptr = head;

            // Move forward on the list
            while (ptr->next != NULL) {

                ptr = ptr->next;
            }

            // Add the node to the end of the list
            term = new PolyNode[sizeof(PolyNode)];
            term->coef = coefficient;
            term->exp = exponent;
            term->next = NULL;
            ptr->next = term;
        }

        // Move back one character to continue processing
        i--;
    }

    // Return the pointer to the head of the linked list
    return head;
} //end-CreatePoly

/// -------------------------------------------------
/// Walk over the poly nodes & delete them
///
void DeletePoly(PolyNode* poly) {

    PolyNode* current = poly;
    PolyNode* next;

    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
} // end-DeletePoly

//-------------------------------------------------
// Adds a node (coefficient, exponent) to poly. If there already 
// is a node with the same exponent, then you simply add the coefficient
// to the existing coefficient. If not, you add a new node to polynomial
// Returns a pointer to the possibly new head of the polynomial.
//
PolyNode* AddNode(PolyNode* head, double coef, int exp) {

    PolyNode* node = new PolyNode; // Create a new node
    node->coef = coef;
    node->exp = exp;
    node->next = nullptr;

    // Handle exception
    if (head == nullptr) {
        return node; // node becomes the head if there weren't any nodes in the list.
    }

    // Go to the last node
    PolyNode* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }

    // Add the node to the end of the list
    current->next = node;

    // Return new head;
    return head;
} // end-AddNode

//-------------------------------------------------
// Adds two polynomials and returns a new polynomial that contains the result
// Computes: poly3 = poly1 + poly2 and returns poly3
//
PolyNode* Add(PolyNode* poly1, PolyNode* poly2) {

    // New poly node to be returned
    PolyNode* result = nullptr;

    // Check if one of the polynoms contains more terms than the other.
    while (poly1 != nullptr || poly2 != nullptr) {

        // Variables to store coefficient and exponent.
        double coef = 0.0;
        int exp = 0;

        // Handle the case where both nodes contains polynomial expressions.
        if (poly1 != nullptr && poly2 != nullptr) {

            // Compare exponents
            if (poly1->exp == poly2->exp) {

                // Add the coefficients of x with same exponents.
                coef = poly1->coef + poly2->coef;

                // Set the exponent of x.
                exp = poly1->exp;

                // Move forward to the next term.
                poly1 = poly1->next;
                poly2 = poly2->next;
            }

            // Handle the case where the first polynomial has bigger exponent than the second.
            else if (poly1->exp > poly2->exp) {

                // Add the poly1 term as it is.
                coef = poly1->coef;
                exp = poly1->exp;

                // Move on to the next term of poly1.
                poly1 = poly1->next;
            }
            else {

                // If second poly has bigger exponent,
                // Add the poly2 term as it is.
                coef = poly2->coef;
                exp = poly2->exp;

                // Move on to the next term of poly2.
                poly2 = poly2->next;
            }
        }

        // Handle the case where poly1 contains more terms than poly2.
        else if (poly1 != nullptr) {

            // Add the excess term of poly1.
            coef = poly1->coef;
            exp = poly1->exp;

            // Move on to the next term.
            poly1 = poly1->next;
        }

        // Handle the case where poly2 contains more terms than poly1.
        else if (poly2 != nullptr) {

            // Add the excess term of poly2.
            coef = poly2->coef;
            exp = poly2->exp;

            // Move on to the next term.
            poly2 = poly2->next;
        }

        result = AddNode(result, coef, exp);
    }

    return result;
} //end-Add

//-------------------------------------------------
// Subtracts poly2 from poly1 and returns the resulting polynomial
// Computes: poly3 = poly1 - poly2 and returns poly3
//
PolyNode* Subtract(PolyNode* poly1, PolyNode* poly2) {

    /**
    * Whole logic is the same as Add function,
    * Except; instead of adding we are now subtracting nodes.
    */

    PolyNode* result = nullptr;

    while (poly1 != nullptr || poly2 != nullptr) {

        double coef = 0.0;
        int exp = 0;

        if (poly1 != nullptr && poly2 != nullptr) {

            if (poly1->exp == poly2->exp) {

                coef = poly1->coef - poly2->coef;
                exp = poly1->exp;
                poly1 = poly1->next;
                poly2 = poly2->next;
            }
            else if (poly1->exp > poly2->exp) {

                coef = poly1->coef;
                exp = poly1->exp;
                poly1 = poly1->next;
            }
            else {

                coef = -poly2->coef;
                exp = poly2->exp;
                poly2 = poly2->next;
            }
        }
        else if (poly1 != nullptr) {

            coef = poly1->coef;
            exp = poly1->exp;
            poly1 = poly1->next;
        }
        else if (poly2 != nullptr) {

            coef = -poly2->coef;
            exp = poly2->exp;
            poly2 = poly2->next;
        }

        result = AddNode(result, coef, exp);
    }

    return result;
} //end-Substract

//-------------------------------------------------
// Multiplies poly1 and poly2 and returns the resulting polynomial
// Computes: poly3 = poly1 * poly2 and returns poly3
//
PolyNode* Multiply(PolyNode* poly1, PolyNode* poly2) {

    // New poly node to be returned.
    PolyNode* result = nullptr;

    // Loop over poly1's terms.
    while (poly1 != nullptr) {

        // Temporary poly2 pointer.
        PolyNode* tmpPoly2 = poly2;

        // Loop over the poly2's terms.
        while (tmpPoly2 != nullptr) {

            // Multiply coefficients of poly1 and poly2, add the exponents.
            double coef = poly1->coef * tmpPoly2->coef;
            int exp = poly1->exp + tmpPoly2->exp;

            // Push the product to the result.
            result = AddNode(result, coef, exp);

            // Move forward on poly2.
            tmpPoly2 = tmpPoly2->next;
        }

        // Move forward on poly1.
        poly1 = poly1->next;
    }

    return result;
} //end-Multiply

//-------------------------------------------------
// Evaluates the polynomial at a particular "x" value and returns the result
//
double Evaluate(PolyNode* poly, double x) {

    // Initialize return value
    double result = 0.0;

    // Iterate over the terms
    while (poly != nullptr) {

        // Multiply the coefficient with the power of x, push it to the result.
        result += poly->coef * pow(x, poly->exp);

        // Move on to the next term
        poly = poly->next;
    }

    return result;
} //end-Evaluate

//-------------------------------------------------
// Computes the derivative of the polynomial and returns it
// Ex: poly(x) = 3x^4 - 2x + 1-->Derivative(poly) = 12x^3 - 2
//
PolyNode* Derivative(PolyNode* poly) {

    // Set up a result pointer.
    PolyNode* result = nullptr;

    // Set up coefficient and exponent
    int exp;
    double coef;

    // Check if node contains terms.
    while (poly != nullptr) {

        // Check if power of x is greater than 0.
        if (poly->exp > 0) {

            // Multiply coefficient with the exponent.
            coef = poly->coef * poly->exp;

            // Reduce exponent by one.
            exp = poly->exp - 1;

            // Push it to the result node.
            result = AddNode(result, coef, exp);
        }
        poly = poly->next;
    }

    return result;
} //end-Derivative

//-------------------------------------------------
// Plots the polynomial in the range [x1, x2].
// -39<=x1<x2<=39
// -12<=y<=12
// On the middle of the screen you gotta have x & y axis plotted
// During evaluation, if "y" value does not fit on the screen,
// then just skip it. Otherwise put a '*' char depicting the curve
//
void Plot(PolyNode* poly, int x1, int x2) {

    const int WIDTH = 12; // Width of the graph window
    const int HEIGHT = 12; // Height of the graph window
    int x, y;

    for (y = HEIGHT; y >= -HEIGHT; y--) {

        // Evaluate x values
        for (x = x1; x <= x2; x++) {
            double value = Evaluate(poly, x);

            // Draw the curve with *
            if (value >= y - 0.5 && value <= y + 0.5) {
                cout << '*';
            }

            //Origin
            else if (y == HEIGHT && x == x1) {
                cout << " ";
            }
            else if (y == HEIGHT && x == x2) {
                cout << " ";
            }
            //Origin end

            // Draw the x axis
            else if (y == 0 && x != 0) {
                cout << '-';
            }

            // Draw the y axis
            else if (x == 0 && y != 0) {
                cout << '|';
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }
} //end-Plot
