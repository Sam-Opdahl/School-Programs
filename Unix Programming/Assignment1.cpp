/*
* Comp 376 - Assignment 1
* Last Updated: 9/16/13
* Author: Sam Opdahl
*
* Description: This program takes three sets of coordinates from the user. The first two are used to construct a rectangle,
*		and the third is used as a single point. The point is then compared to the rectangle to find if it is within
*		the rectangle's bounds. This information is then sent as output to the user.
*
*		This program also implements simple input validation to make sure the user's input is a valid set of coords.
*
*		To exit, the user can enter all zeroes for the coordinates of the rectangle.
*
*/

#include <iostream>
#include <limits>
#include <string>
#include <sstream>

using namespace std;

struct Point
{
    int x;
    int y;
};


// -- Function Headers --

//Post: The user's input has been read and returned as a Point.
Point getCoordsFromInput();

//Post: The user has pressed [enter] and the program can continue.
void waitForInput();

//Pre: 'p' is a Point where the x and y coords have been initialized.
//Post: a formatted string, "(x, y)", is returned based on the point.
string pointToString(Point p);


int main()
{
    cout << "To exit, enter all zeroes for the rectangle's coordinates.\n\n";

    while (true)
    {
        Point upperLeft;
        Point lowerRight;
        //The point to be checked against the rectangle.
        Point pointToCheck;

        cout << "Enter the coords of the upper left corner of the rectangle \"x y\": ";
        upperLeft = getCoordsFromInput();

        cout << "Enter the coords of the lower right corner of the rectangle \"x y\": ";
        lowerRight = getCoordsFromInput();

        //If user entered all zeroes for the coords, exit the program.
        if (upperLeft.x == 0 && upperLeft.y == 0 && lowerRight.x == 0 && lowerRight.y == 0)
            return 0;

        cout <<"Enter the coords of the point to check \"x y\": ";
        pointToCheck = getCoordsFromInput();

        //the keyword indicates if the point is outside or within the rectangle.
        string keyword = "OUTSIDE";
        if (pointToCheck.x >= upperLeft.x && pointToCheck.x <= lowerRight.x)
            if (pointToCheck.y >= upperLeft.y && pointToCheck.y <= lowerRight.y)
                keyword = "WITHIN";

        cout << "\nThe point " << pointToString(pointToCheck) << " is " << keyword << " the bounds of the rectangle ";
        cout << "whose upper left corner is " << pointToString(upperLeft) << " and whose lower right ";
        cout << "corner is " << pointToString(lowerRight) << ".\n\n";
        cout << "Press [enter] to continue...";
        waitForInput();
    }
}

Point getCoordsFromInput()
{
    Point p;

    while (true)
    {
        if (cin >> p.x >> p.y)
            return p;

        //Format of user's input was invalid.
        cout << "Invalid format. Please enter the coords in this format: \"x y\" (without quotes)\n";
        //reset stream error flag
        cin.clear();
        //clear remainder of input stream
        cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
}

string pointToString(Point p)
{
    //create a new stream to help format the string to return.
    stringstream stream;
    stream << "(" << p.x << ", " << p.y << ")";
    return stream.str();
}

void waitForInput()
{
    //ignore any characters in the stream until we hit a newline character.
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    //Newline character hit, extract it and continue.
    cin.get();
}
