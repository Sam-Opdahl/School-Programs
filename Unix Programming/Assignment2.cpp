/*
*   Comp 376 - Assignment 2
*   Sam Opdahl
*   Last Modified: 9/21/13
*
*   This program will take a file and encrypt it by splitting it into two seperate files.
*   This program can also decrypt the files, by re-attaching them into a single file.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// -- Function Headers --

//Pre: 'sourceFile' is location of the file to encrypt.
//      'destFile1' and 'destFile2' are the locations of the files to store the encrypted data.
//Post: Files 'destFile1' and 'destFile2' have been created/overwritten with the encrypted data.
void split(const char sourceFile[], const char destFile1[], const char destFile2[]);

//Pre: 'sourceFile1' and 'sourceFile2' are the location of the encrypted data.
//      'destFile' is the location used to store the decrypted file.
//Post: 'destFile' has been created/overwritten with the decrypted data.
void merge(const char sourceFile1[], const char sourceFile2[], const char destFile[]);

// * Helper Functions *

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

// -- Program entry point --

int main()
{
    int input;
    //Will be used to hold the locations/names of the files to be used in the split/merge functions.
    string fileName[3];

    cout << "This program can be used for file encryption/decryption\n";
    cout << "The program can encrypt a file by splitting it's\n";
    cout << "  characters into two seperate files.\n";
    cout << "The program can also decrypt your encrypted files \n";
    cout << "  by re-attaching them back into one file.\n\n";

    cout << "What would you like to do?\n";
    cout << "1. Encrypt a file\n";
    cout << "2. Decrypt a set of files\n";
    cout << "3. Exit\n";
    cout << "Your input: ";

    if (!(cin >> input))
    {
        cout << "Invalid input. Program will now exit\n";
        return 0;
    }

    switch (input)
    {
        //user has opted to encrypt an existing file.
        case 1:
            cout << "\nEnter the name of the source file to encrypt: ";
            cin >> fileName[0];
            validateInFile(fileName[0].c_str());

            cout << "Enter the name of the first destination file for output: ";
            cin >> fileName[1];
            //Warn user if output already exists and that it will be overwritten. If user opts to quit, program will exit.
            validateOutFile(fileName[1].c_str());

            cout << "Enter the name of the second destination file for output: ";
            cin >> fileName[2];
            validateOutFile(fileName[2].c_str());

            split(fileName[0].c_str(), fileName[1].c_str(), fileName[2].c_str());
            break;
        //User has opted to decrypt an already encrypted file.
        case 2:
            cout << "\nEnsure the source files are in the correct order, or decryption will fail.\n";
            cout << "Enter the name of the first source file to decrypt: ";
            cin >> fileName[0];
            validateInFile(fileName[0].c_str());

            cout << "Enter the name of the second source file to decrypt: ";
            cin >> fileName[1];
            validateInFile(fileName[1].c_str());

            cout << "Enter the name of the destination file for output: ";
            cin >> fileName[2];
            validateOutFile(fileName[2].c_str());

            merge(fileName[0].c_str(), fileName[1].c_str(), fileName[2].c_str());
            break;
        //User has opted to exit program.
        case 3:
            return 0;
        default:
            cout << "Option " << input << " is non-existant. Program will now exit\n";
            return 0;
    }
    return 0;
}

void split(const char sourceFile[], const char destFile1[], const char destFile2[])
{
    ifstream inStream;
    createInStream(inStream, sourceFile);

    ofstream outStream1;
    createOutStream(outStream1, destFile1);

    ofstream outStream2;
    createOutStream(outStream2, destFile2);

    //charCount is used to determine odd/even characters which will determine which stream the
    //input character will be written to.
    int charCount = 1;
    char currentRead;
    while (inStream.get(currentRead))
    {
        if ((charCount++) % 2 != 0)
            outStream1.put(currentRead);
        else
            outStream2.put(currentRead);
    }

    cout << "\n\"" << sourceFile << "\" has been successfully encrypted and\n";
    cout << "Files \"" << destFile1 << "\" and \"" << destFile2 << "\" have been successfully created.\n";

    inStream.close();
    outStream1.close();
    outStream2.close();
}

void merge(const char sourceFile1[], const char sourceFile2[], const char destFile[])
{
    ifstream inStream1;
    createInStream(inStream1, sourceFile1);

    ifstream inStream2;
    createInStream(inStream2, sourceFile2);

    ofstream outStream;
    createOutStream(outStream, destFile);

    char inChar1, inChar2;
    while (inStream1.get(inChar1))
    {
        outStream.put(inChar1);
        //Since second stream may possibly be one character shorter than the first stream,
        //do a seperate check to ensure we are still getting a character from it as well.
        if (inStream2.get(inChar2))
            outStream.put(inChar2);
    }

    cout << "\nFiles \"" << sourceFile1 << "\" and \"" << sourceFile2 << "\" have been successfully decrypted and\n";
    cout << "\"" << destFile << "\" has been successfully created.\n";

    inStream1.close();
    inStream2.close();
    outStream.close();
}

void validateInFile(const char fileName[])
{
    ifstream stream;
    //create the stream. if stream creation was successful, the file in turn is valid.
    //if stream fails, file is invalid and program will exit.
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
