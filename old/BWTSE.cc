#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <string.h>

using namespace std;

void get_rotations(int size, string input, char* output)
{
    //Allocates an array of string pointers, to strings in heap memory
    string* rotation = new string [size];

    //Uses the rotate function in the std:: libary to get all of the permiatations of the input string
    for(int i = 0; i < size; i++)
    {
        rotate(input.begin(), input.begin()+1, input.end());
        rotation[i] = input;
    }

    //sorts all of the permiatations of the input string
    sort(rotation,rotation + size);

    //gets the output, (the last column of the soreted permiatations) and stores it in the array output.
    for(int i = 0; i < size; i++)
	   output[i] = rotation[i][size-1];

    //removes rotation from heap memory
    delete [] rotation;
}


void write_output(int size, char* output)
{
    //opens a file called "encode.txt" and stores the char array output into it.
    ofstream  outputfile("txt-transform.txt", ios::out | ios::binary);

    if (!outputfile)
    {
        cout << "ERR: -- Charecters cannot be written to file" << endl;
        exit(1);
    }

    outputfile.write(output, size);
}

int Begin_Encode(){
    int size;
    string input;

    //This grabs the file named 'test.txt' as input data for the BWT encode, if there is no filed named test.txt, it will throw an error message
    ifstream inputfile("test.txt", ios::binary);

    if(!inputfile.is_open())
    {
        cout << "ERR: -- Incorrect file name\n";
        exit(1);
    }

    //This calculates how many charecters are in the test.txt file. (size is decremented to exclude the EOF charecter)
    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);
    size--;


    //A dynamic memory pointer will be created to put the .txt file into memory so we can manipulate the data.
    char* in = new char [size+1];

    //reads the data into the char array, if for any reason the chars cannoty be read, it will throw an error message

    inputfile.read(in,size);
    if (!inputfile)
    {
        cout << "ERR: -- Charecters have not been read successfully" << endl;
        exit(1);
    }
    inputfile.close();

    //size is adjusted to the origonal, a marking charecter is added to the data, so we are able to undo the transform later
    ++size;
    in[size-1] = char(7);

    //the char array with the added marking charecter is converted to a string for simplicity in the actual algorithm
    ostringstream buffer;

    for(int i = 0; i < size; i++)
        buffer << in[i];
    input = buffer.str();

    //In is deleted from heap memory, as we no longer have any use for it.
    delete [] in;

    // a char pointer to heap memory is created to placed the converted string into.
    char* output = new char [size];

    //A function that manipulates the input text to get the rotations and sorts to get the output text.
    get_rotations(size, input, output);

    //A function that withes the output char array to a .txt file named "encode.txt".
    write_output(size, output);

    //deletes the output array from heap memory
    delete [] output;

    return 0;
}
