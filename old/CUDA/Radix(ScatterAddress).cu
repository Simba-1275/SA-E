#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;


__global__ void Radix(char * d_in, char * d_out, int * d_scan,  int * d_scatter, int * d_past, int size)
{
    int idx = threadIdx.x ;
    int total;

    for(int j = 0; j<8; j++)
    {
        if (j==0)
        {
            //Creates a Inclusive scan of where there are 0's (if there is a 0, it is incremented)
            d_past[idx] = (d_in[idx] & (1 << j)) ? 0 : 1;
            d_scan[idx] = d_past[idx];
        }
        else
        {
            //Creates a Inclusive scan of where there are 0's (if there is a 0, it is incremented)
            d_past[idx] = (d_in[d_scatter[idx]] & (1 << j)) ? 0 : 1;
            d_scan[idx] = d_past[idx];
        }

        __syncthreads();
        
        //(Scan jumping operation)
        for(int jump = 1; jump < size; jump *= 2)
        {

            if(idx - jump >= 0)
                d_scan[idx] = d_scan[idx] + d_past[idx-jump];

            d_past[idx] = d_scan[idx];

            __syncthreads();
        }

        //From the inclusive scan, the total amount of 0's is equal to the last place in the scan.
        total = d_scan[size - 1];

        __syncthreads();

        //Converts to a exclusive scan, will change to a belloch scan that is more work efficient
        if (idx != 0)
            d_scan[idx] = d_past[idx-1];
        else
            d_scan[0] = 0;

        __syncthreads();


        if (j==0)
        {
            if ((d_in[idx] & (1 << j)) == false)
                d_scatter[d_scan[idx]] = idx;

            else
                d_scatter[(idx + total) - d_scan[idx]] = idx;
        }
        else
        {
            if ((d_in[d_scatter[idx]] & (1 << j)) == false)
                d_past[d_scan[idx]] = idx;

            else
                d_past[(idx + total) - d_scan[idx]] = idx;
        }

        __syncthreads();

        if (j != 0)
            d_scatter[idx] = d_scatter[d_past[idx]];

        __syncthreads();

    }

    d_out[idx] = d_in[d_scatter[idx]];
}

int main() {
    int size;

    //This grabs the file named 'test.txt' as input data for the BWT encode, if there is no filed named test.txt, it will throw an error message
    ifstream inputfile("1024.txt", ios::binary);

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
    char* input = new char [size];

    //reads the data into the char array, if for any reason the chars cannoty be read, it will throw an error message

    inputfile.read(input,size);
    if (!inputfile)
    {
        cout << "ERR: -- Charecters have not been read successfully" << endl;
        exit(1);
    }
    inputfile.close();

    //Declare the amount of memory needed for both types of arrays
    const int Array_char = size * sizeof(char);
    const int Array_int = size * sizeof(int);

    // declare GPU memory pointers
    char * d_in;
    char * d_out;
    int * d_scan;
    int * d_scatter;
    int * d_past;

    //declare array to hold to sorted list
    char h_out[size];

    // allocate GPU memory
    cudaMalloc((void**) &d_in, Array_char);
    cudaMalloc((void**) &d_out, Array_char);
    cudaMalloc((void**) &d_scan, Array_int);
    cudaMalloc((void**) &d_scatter, Array_int);
    cudaMalloc((void**) &d_past, Array_int);

    // transfer the array to the GPU
    cudaMemcpy(d_in, input, Array_char, cudaMemcpyHostToDevice);

    // launch the kernel
    Radix<<< 1, size >>>(d_in, d_out, d_scan, d_scatter, d_past, size);

    // copy back the GPU array to the CPU array
    cudaMemcpy(h_out, d_out, Array_char, cudaMemcpyDeviceToHost);

    cudaFree(d_in);
    cudaFree(d_out);
    cudaFree(d_scan);
    cudaFree(d_scatter);
    cudaFree(d_past);

    delete [] input;

    for(int i=0; i<size; i++)
        printf("%c", h_out[i]);
    printf("\n");


    return 0;
}
