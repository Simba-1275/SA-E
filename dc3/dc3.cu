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
        //Creates a Inclusive scan of where there are 0's (if there is a 0, it is incremented)
        d_past[idx] = (d_in[idx] & (1 << j)) ? 0 : 1;
        d_scan[idx] = d_past[idx];
        __syncthreads();

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

        if ((d_in[idx] & (1 << j)) == false)
            d_scatter[d_scan[idx]] = idx;
        else
            d_scatter[(idx + total) - d_scan[idx]] = idx;
        __syncthreads();

        d_in[idx] = d_out[d_scatter[idx]];
        d_out[idx] = d_in[idx];
    }
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

int main(){
    int size;
    get_size(size);

    char* input = new char [size];
    get_char(size, input);

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
    cudaMemcpy(d_out, input, Array_char, cudaMemcpyHostToDevice);

    // launch the kernel
    Radix<<< 1, size >>>(d_in, d_out, d_scan, d_scatter, d_past, size);

    // copy back the GPU array to the CPU array
    cudaMemcpy(h_out, d_out, Array_char, cudaMemcpyDeviceToHost);

    for(int i=0; i<size; i++)
        printf("%c", h_out[i]);
    printf("\n");


    cudaFree(d_in);
    cudaFree(d_out);
    cudaFree(d_scan);
    cudaFree(d_scatter);
    cudaFree(d_past);

    delete [] input;

}
