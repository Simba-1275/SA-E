#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

void testing(int size, int newsize, char* input, char* sorted_input, int charcount, int* number_rep, char* char_ref){

    cout << "\n";
    cout << "Input Array:                   ";
    for(int i=0; i<size; i++)
        cout << input[i];
    cout << endl;

    cout << "Size of Text:                  " << size << endl;

    cout << "Sorted Input Array:            ";
    for(int i=0; i<size; i++)
        cout << sorted_input[i];
    cout << endl;

    cout << "Number of Unique Charecters:   ";
    cout << charcount << endl;

    cout << "Unique Charecters in order:    ";
    for(int i=0; i<charcount; i++)
        cout << char_ref[i] << " ";
    cout << endl;

    cout << "Added 0's                      ";
    cout << newsize << endl;

    cout << "Number Representation:         ";
    for(int i=0; i<size+newsize; i++)
        cout << number_rep[i] << " ";
    cout << endl;

}

void get_size(int& size){
    ifstream inputfile("test.txt", ios::binary);

    if(!inputfile.is_open())
    {
        cout << "ERR: -- Incorrect file name\n";
        exit(1);
    }

    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);
    size--;
    inputfile.close();
}

void get_char(int size, char* input){
    ifstream inputfile("test.txt", ios::binary);
    inputfile.read(input,size);
    if (!inputfile.is_open())
    {
        cout << "ERR: -- Charecters have not been read successfully\n";
        exit(1);
    }
    inputfile.close();
}

void Radix(int size, char input[], char sorted_input[]){
    int index = 0;
    int* bit = new int [size];
    char* replace = new char [size];

    for(int i=0; i<size; i++)
    {
        sorted_input[i] = input[i];
    }

    for(int j=0; j<8; j++)
    {
        for(int i=0; i<size; i++)
        {
            bit[i] = (sorted_input[i] & (1 << j)) ? 1 : 0;
        }
        for(int i=0; i<size; i++)
        {
            if(bit[i] == 0)
            {
                replace[index] = sorted_input[i];
                ++index;
            }
        }
        for(int i=0; i<size; i++)
        {
            if(bit[i] == 1)
            {
                replace[index] = sorted_input[i];
                ++index;
            }
        }
        for(int i=0; i<size; i++)
        {
            sorted_input[i] = replace[i];
        }
        index = 0;
    }

    delete[] bit;
    delete[] replace;
}

void get_charcount(int size, char* sorted_input, int& charcount){
    for (int i=0; i<size-1; i++)
    {
        if (sorted_input[i] != sorted_input[i+1])
            charcount++;
    }
}

void get_newsize(int size, int& newsize){

    if(size%3 == 0)
        newsize = 3;

    if(size%3 == 1)
        newsize = 2;

    if(size%3 == 2)
        newsize = 4;
}

void get_number_rep(int size, int newsize, char* input, char* sorted_input, int* number_rep, int charcount, char* char_ref){
    int index =1;
    char_ref[0] = sorted_input[0];
    for(int i=1; i<size; i++)
    {
        if(sorted_input[i] != sorted_input[i-1])
        {
            char_ref[index] = sorted_input[i];
            index++;
        }
    }

    for(int i=0; i<charcount; i++)
    {
        for(int j=0; j<size; j++)
        {
            if(input[j] == char_ref[i])
            {
                number_rep[j] = (i+1);
            }
        }
    }
    for(int i=size; i<size+newsize; i++)
        number_rep[i] = 0;

}

int main(){
    int size;
    get_size(size);

    char* input = new char [size];
    get_char(size, input);

    char* sorted_input = new char [size];
    Radix(size, input, sorted_input);

    int charcount = 1;
    get_charcount(size, sorted_input, charcount);

    int newsize = 0;
    get_newsize(size, newsize);

    char* char_ref = new char[charcount];
    int* number_rep = new int[size + newsize];
    get_number_rep(size, newsize, input, sorted_input, number_rep, charcount, char_ref);

    testing(size, newsize, input, sorted_input, charcount, number_rep, char_ref);

    delete[] input;
    delete[] sorted_input;
    delete[] number_rep;
    delete[] char_ref;


    return 0;
}
