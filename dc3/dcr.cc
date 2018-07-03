#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;


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
void find_B12_length(int size, int& B12_length){
    int num;
    num = (size / 3) + 2;

    int index1 = 1;
    int index2 = 2;

    for (int i=0; i<num; i++)
    {
        if(index1 <= size)
            B12_length += 1;
        if(index2 <= size)
            B12_length += 1;

        index1 += 3;
        index2 += 3;
    }
}

void initialise_ranks(int B12_length, int B12[]){
    for(int i=0; i<B12_length; i++)
    {
        B12[i] = 1;
    }


}

int main(){
    int size;
    get_size(size);

    char* input = new char [size];
    get_char(size, input);

    int B12_length = 0;
    find_B12_length(size, B12_length);

    int B12 = new int [B12_length];
    initialise_ranks(B12_length, B12[]);

    char* sorted_input = new char [size];
    Radix(size, input, sorted_input);

    delete[] input;
    delete[] sorted_input;


    return 0;
}
