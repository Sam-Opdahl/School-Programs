/***********************************************************
*
*	Sam Opdahl
*	Comp 376 - C++ Assignment 5
*	11/16/13
*
*	To use this program, you must provide a file for input.
*	This file should contain post-fix expressions with each
*		element of the expression seperated by a space and 
*		each expression seperated by a new line. 
*	Output can be sent to the screen or an output file.
*
*	Input file example: 5 12 + 2 *
*		translates to-> (5 + 12) * 2
*					  = 34
*
************************************************************/

#include <iostream>
#include <stack>
#include <sstream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

// -- Function Headers --

//Pre: Location/name of the file to validate.
//Post: Checks if the output file exists. If it does, a warning it prompted to the user.
//      if user opts to quit, the program exits.
void validateOutFile(const char fileName[]);

//Pre: Location/name of the file to validate.
//Post: If input file doesn't exist or we are denied permissions to access it, an error message is returned to the user
//      and the program exits.
void validateInFile(const char fileName[]);

//Pre: 'iStream' is the memory address of the ifstream to open.
//      'fileName' is the location/name of the file to open the stream with.
//Post: 'iStream' has been created OR opening of stream failed and program has exited.
void createInStream(ifstream &iStream, const char fileName[]);


int main() {
	string inFile;

	cout << "~~ Postfix Expression File Processor - v1.0 ~~\n\n";
	cout << "Enter the name of the input file to process: ";
	cin >> inFile;
	validateInFile(inFile.c_str());

	int preferredOutput = 1;
	string outFile;
	ofstream ofStream;
	cout << "Where would you like the output to be sent to? (Defaults to screen output)\n";
	cout << "1. Screen output\n";
	cout << "2. Output file\n";
	cout << "Your input: ";
	cin >> preferredOutput;

	switch (preferredOutput) {
		//If user opted for file output, validate the output file and create the stream.
		case 2:
			cout << "Please enter the name of the output file: ";
			cin >> outFile;
			validateOutFile(outFile.c_str());
			ofStream.open(outFile.c_str());
			cout << "Sending output to file \"" << outFile << "\"...\n\n";
			break;
		//User either opted for screen output or typed and invalid entry,
		//which then defaults to screen output
		default:
			cout << "Sending output to screen...\n\n";
			break;
	}

	ostream &outStream = ofStream.is_open() ? ofStream : cout;

	ifstream iStream;
	createInStream(iStream, inFile.c_str());

	string currentLine;

	while (getline(iStream, currentLine)) {
		istringstream lineStream(currentLine);
		stack<int> currentStack;
		bool error = false;

		string input;

		//Loop through each operand/operator in the line
		while (lineStream >> input) {
			//Check if the current input is an integer value
			int val;
			if ((val = atoi(input.c_str()))) {
				//if the input is an int, push it to the stack.
				currentStack.push(val);
			}
			//If the input isn't an int value...
			else {
				//Check if we have at least two operand to work with
				if (currentStack.size() >= 2) {
					//pop two operands off the stack
					int b = currentStack.top();
					currentStack.pop();
					int a = currentStack.top();
					currentStack.pop();

					//Push the new value back to the stack based on the operator
					if (input == "+")
						currentStack.push(a + b);
					else if (input == "-")
						currentStack.push(a - b);
					else if (input == "*")
						currentStack.push(a * b);
					else if (input == "/")
						currentStack.push(a / b);
					else {
						outStream << "Illegal operation: \"" << input << "\"\n";
						error = true;
						break;
					}
				}
				//if we don't have at least two oeprands to work with...
				else {
					outStream << "Too few operands\n";
					error = true;
					break;
				}
			}
		}
		if (!error) {
			//More than one operand in the stack?
			if (currentStack.size() > 1)
				outStream << "Too few operators\n";
			//No errors occured
			else
				outStream << currentStack.top() << "\n";
		}
	}

	iStream.close();
	//if we did output to a file, let the user know it was a success.
	if (ofStream.is_open()) {
		ofStream.close();
		cout << "Output file \"" << outFile << "\" successfully created.\n";
	}

	exit(EXIT_SUCCESS);
}

void validateOutFile(const char fileName[])
{
    ifstream stream(fileName);
    if (stream.good())
    {
        string userInput;
        cout << "File \"" << fileName << "\" already exists.\n";
        cout << "If you continue, this file will be overwritten.\n";
        cout << "Do you wish to continue (y/n)? ";
        cin >> userInput;
        if (userInput == "y")
        {
            stream.close();
            return;
        }
        else
            exit(EXIT_SUCCESS);
    }
    stream.close();
}

void validateInFile(const char fileName[])
{
    ifstream stream;
    createInStream(stream, fileName);
    stream.close();
}

void createInStream(ifstream &iStream, const char fileName[])
{
    iStream.open(fileName);
    if (!iStream.good())
    {
        cout << "File \"" << fileName << "\" doesn't exist or has invalid permissions.\n";
        cout << "Program will now exit.\n";
        exit(EXIT_FAILURE);
    }
}