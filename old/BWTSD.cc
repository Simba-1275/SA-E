#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include <cstdlib>
#include <string.h>

using namespace std;

void _regen_rotations(int size, char* in, char output[])
{

    // creates char array alpha, for the alphabeticall sorted array of the encoded matrix.
    char* alpha;
    alpha = new char [size];

    for(int i = 0; i < size; i++)
        alpha[i] = in[i];

    sort(alpha, alpha+size);

    //creates the two int arrays alpha count and matrix count. these are used to store the count of each charecter... eg the third a in the text, will hold 3 in the same location as the a in the encoded matrix
    int* alphacount;
    alphacount = new int [size];
    int* matrixcount;
    matrixcount = new int [size];

    int numchar = 1;
    alphacount[0] = 1;

    //gets char count for alpha count
    for(int i = 1; i < size; i++){
        if(alpha[i] == alpha[i-1])
            alphacount[i] = alphacount[i-1] + 1;
        else
        {
            alphacount[i] = 1;
            numchar += 1;
        }
    }

    //counts how many of each charecter there are and stores it respectivly in charcount. Reference another array that hold the charecter in the same memory slot in another array.
    int charcount[numchar];
    char reference[numchar];
    charcount[0] = 1;
    int count = 0;

    for(int i = 1; i < size; i++){

        if(alpha[i] == alpha[i-1])
            charcount[count] += 1;
        else{
            count += 1;
            charcount[count] = 1;
        }
    }

    //re-using variable count
    count = 0;

    // gets the reference charecters to compare against the input string
    for(int i=0; i<numchar; i++)
    {
        reference[i] = alpha[count];
        count += charcount[i+1];
    }

    //using the charcount data, from the bottom of the array up, the count of charectes are placed in the matrixcount array respectivly.
    for(int i = 0; i < numchar; i++)
    {
        for(int j = size-1; j > -1; j--)
        {
            if(in[j] == reference[i])
            {
                matrixcount[j] = charcount[i];
                charcount[i] -= 1;
            }
        }
    }


    // This is the core algorithm that uses all of the data we created earlier to regenerate the origonal text.
    int outputidx = 0;

    //the begining of this function finds the starting marking charecter that we placed in the encode. in this case and for now this is (char)7.
    for(int i = 0; i < size; i++)
    {
        if (in[i] == (char)7)
        {
            //now the algorithm looks at the count and charecter located in the same location in the alphacount and alpha char arrays.
            //have been stored in temporary variables, the charecter with that count and that charecter signature will be searched for in the encoded matrix.
            // this is repeated untill all charecters have been discovered.

            int count = alphacount[i];

            char ref = alpha[i];

            for(int j=0; j<size-1; j++)
            {
                for(int k=0; k<size; k++)
                {
                    if((matrixcount[k] == count) && (in[k] == ref) )
                    {
                        output[outputidx] = in[k];
                        count = alphacount[k];
                        ref = alpha[k];
                        outputidx += 1;
                        break;
                    }
                //once the algorithm has found all of the charecters, the for loops are escaped using breaks.
                }
                if (outputidx == size-1)
                    break;
            }
            if (outputidx == size-1)
                break;
        }
    }

    //the heap memory variables used are deleted.
    delete [] alpha;
    delete [] alphacount;
    delete [] matrixcount;

}

void _write_output(int size, char output[])
{

    //to ensure that the input is exactly the same, a newline charecter is added at the very end of the file
    output[size-1] = '\n';

    //The output (regenerated text) is placed and saved in the file regen.txt. if an error occurs, an error message is thrown and the program is exited.

    ofstream  outputfile("regen.txt", ios::out | ios::binary);
    if (!outputfile)
    {
        cout << "ERR: -- Charecters cannot be written to file" << endl;
        exit(1);
    }

    outputfile.write(output, size);

}


int Begin_Decode()
{
    //This section gets the text file "encode.txt" and places it into a variable on heap memory called in[]. If there is no file called encode, it will throw an error message
    int size;
    ifstream inputfile("txt-transform.txt", ios::binary);


    if(!inputfile.is_open())
    {
        cout << "ERR: -- Incorrect file name\n";
        exit(1);
    }

    //calculates how many charecters are in the file
    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    //variable in[] located in heap memory
    char* in = new char [size];

    inputfile.read(in,size);
    if (!inputfile)
    {
        cout << "ERR: -- Charecters have not been read successfully" << endl;
        exit(1);
    }
    inputfile.close();

    //A variable output, that will stored on heap memory will holf the regenerated text
    char* output = new char [size];

    //A function that is able to regenerate the input text from the encoded matrix, and store it in the char array output[].
    _regen_rotations(size, in, output);

    //A function that writes the variable output to a file "regen.txt"
    _write_output(size, output);

    //Clears the heap memory usage from in[] and outpu[] char arrays.
    delete [] output;
    delete [] in;


    return 0;
}
