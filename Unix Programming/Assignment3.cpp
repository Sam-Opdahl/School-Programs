/*
*   Comp 376 - Assignment 3
*   Sam Opdahl
*   Last Modified: 9/27/13
*
*   This program can be used to encrypt and decrypt files by transposing an array
*    of characters that are given from the input file.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;


// -- Global Variables --

static const char FILLER_CHAR = ' ';


// -- Function Headers --

//This function serves a dual purpose. It will both encrypt and decrypt files.
//This is because the method for encrypting and decrypting is exactly the same.
//
//Pre: 'inFile' is the location/name of the file to encrypt/decrypt
//      'outFile' is the location/name of where to save the output
//Post: 'inFile' has been either encrypted or decrypted.
void reverseFileContents(const char inFile[], const char outFile[], int key);

// * Helper Functions *

//Pre: 'iStream' is the memory address of the ifstream to read from
//      'array' is the array to hold the input from the stream
//      'key' is the key the user entered
//Post: 'array' will hold the input from 'iStream' and will be size key^2.
//      Function will return true if more input is available from the stream
//      and will return false when the end of the stream is hit.
bool readIntoArray(ifstream &iStream, char** array, int key);

//Pre: 'array' is the 2d array to transpose
//      'key' is the row/column size of the array
//Post: 'array' has been transposed. (rows and columns have been exchanged.)
void transpose(char** array, int key);

//Pre: 'oStream' is the ofstream to write to
//      'array' is the output to write to the stream
//      'key' is the row/column size of the array
//Post: the contents of 'array' have been written to 'oStream'
void writeToFile(ofstream &oStream, char** array, int key);

//Pre: Location/name of the file to validate.
//Post: If input file doesn't exist or we are denied permissions to access it, an error message is returned to the user
//      and the program exits.
void validateInFile(const char fileName[]);

//Pre: 'iStream' is the memory address of the ifstream to open.
//      'fileName' is the location/name of the file to open the stream with.
//Post: 'iStream' has been created OR opening of stream failed and program has exited.
void createInStream(ifstream &iStream, const char fileName[]);

//Pre: Location/name of the file to validate.
//Post: Checks if the output file exists. If it does, a warning it prompted to the user.
//      if user opts to quit, the program exits.
void validateOutFile(const char fileName[]);

//Pre: 'oStream' is the memory address of the ofstream to open.
//      'fileName' is the location/name of the file to open the stream with.
//Post: 'oStream' has been created OR opening of stream failed and program has exited.
void createOutStream(ofstream &oStream, const char fileName[]);


int main()
{
    int input;
    string fileName[2];
    int key;
    //Shortcut for displaying "encryption" or "decryption" in the menu options, based on user's input.
    string prefix[] = {"", "en", "de"};

    //Print out shorts description of program
    cout << "This program can be used for file encryption/decryption\n";

    //Print menu for the user.
    cout << "What would you like to do?\n";
    cout << "1. Encrypt a file\n";
    cout << "2. Decrypt a file\n";
    cout << "3. Exit\n";
    cout << "Your input: ";

    if (!(cin >> input))
    {
        cout << "Invalid input. Program will now exit\n";
        return 0;
    }

    switch (input)
    {
        //user has opted to encrypt/decrypt an existing file.
        case 1:
        case 2:
            cout << "Enter the name of the source file to "<< prefix[input] << "crypt: ";
            cin >> fileName[0];
            validateInFile(fileName[0].c_str());

            cout << "Enter the name of the destination file for output: ";
            cin >> fileName[1];
            validateOutFile(fileName[1].c_str());

            //get key from user
            cout << "Enter the key to use to " << prefix[input] << "crypt the file,\n";
            if (input == 1)
                cout << "Do not forget this key! -> "; //encrypting msg
            else
                cout << "This key must match the encryption key! -> "; //decrypting msg

            //validate key
            if (!(cin >> key))
            {
                //Input is not a valid int, warn user and exit.
                cout << "Invalid key. The key must be a valid integer. Program will now exit\n";
                return 0;
            }

            reverseFileContents(fileName[0].c_str(), fileName[1].c_str(), key);
            cout << "\n\"" << fileName[0] << "\" has been successfully " << prefix[input] << "crypted and saved to \"" << fileName[1] <<"\".\n";
            break;
        case 3:
            return 0;
        default:
            cout << "Option " << input << " is non-existant. Program will now exit\n";
            return 0;
    }
    return 0;
}

void reverseFileContents(const char inFile[], const char outFile[], int key)
{
    ifstream inStream;
    createInStream(inStream, inFile);

    ofstream outStream;
    createOutStream(outStream, outFile);

    char** charArray = new char*[key];
    for (int i = 0; i < key; i++)
        charArray[i] = new char[key];

    while (readIntoArray(inStream, charArray, key))
    {
        transpose(charArray, key);
        writeToFile(outStream, charArray, key);
    }

    for (int i = 0; i < key; i++)
        delete [] charArray[i];
    delete [] charArray;

    inStream.close();
    outStream.close();
}


// * Helper Functions *

bool readIntoArray(ifstream &iStream, char** array, int key)
{
    //Before reading input, clear the array of the previous input, if any.
    //Also, if this is the last read from the file, this prevents current chars
    // from getting mixed up with previous ones.
    for (int x = 0; x < key; x++)
        for (int y = 0; y < key; y++)
            array[x][y] = FILLER_CHAR;

    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < key; j++)
        {
            char inChar;
            if (iStream.get(inChar))
                array[i][j] = inChar;
            else
                //check if any input was read before hitting the end of the file,
                //if so, we want to return true so that input can be processed.
                //otherwise if we didn't get any input, return false.
                return i > 0 || j > 0;
        }
    }
    return true;
}

void transpose(char** array, int key)
{
    char** temp = new char*[key];
    for (int i = 0; i < key; i++)
    {
        temp[i] = new char[key];
        memcpy(temp[i], array[i], sizeof(char) * key);
    }

    for (int x = 0; x < key; x++)
        for (int y = 0; y < key; y++)
            array[x][y] = temp[y][x];

    for (int i = 0; i < key; i++)
        delete [] temp[i];
    delete [] temp;
}

void writeToFile(ofstream &oStream, char** array, int key)
{
    for (int x = 0; x < key; x++)
        for (int y = 0; y < key; y++)
            oStream << array[x][y];
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

void createOutStream(ofstream &oStream, const char fileName[])
{
    oStream.open(fileName);
    if (!oStream.good())
    {
        cout << "Error creating output file\"" << fileName << "\".\n";
        cout << "Program will now exit.\n";
        exit(EXIT_FAILURE);
    }
}
