// C program to find Burrows Wheeler transform of
// a given text
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <string.h>
#include "bwt-headers.h"

using namespace std;


// Structure to store data of a rotation
struct rotation
{
	int index;
	char *suffix;
};

// Compares the rotations and
// sorts the rotations alphabetically
int cmpfunc (const void *x, const void *y)
{
	struct rotation *rx = (struct rotation *)x;
	struct rotation *ry = (struct rotation *)y;
	return strcmp(rx -> suffix, ry -> suffix);
}

// Takes text to be transformed and its length as
// arguments and returns the corresponding suffix array
int *computeSuffixArray(char *input_text, int len_text)
{
	// Array of structures to store rotations and
	// their indexes
	struct rotation suff[len_text];

	// Structure is needed to maintain old indexes of
	// rotations after sorting them
	for(int i = 0; i < len_text; i++)
	{
		suff[i].index = i;
		suff[i].suffix = (input_text+i);
	}

	// Sorts rotations using comparison function defined above
	qsort(suff, len_text, sizeof(struct rotation), cmpfunc);

	// Stores the indexes of sorted rotations
	int *suffix_arr = (int *) malloc (len_text * sizeof(int));
	for (int i = 0; i < len_text; i++)
		suffix_arr[i] = suff[i].index;

	// Returns the computed suffix array
	return suffix_arr;
}

// Takes suffix array and its size as arguments and returns
// the Burrows - Wheeler Transform of given text
char *findLastChar(char *input_text, int *suffix_arr, int n)
{
	// Iterates over the suffix array to find
	// the last char of each cyclic rotation
	char *bwt_arr = (char *) malloc (n * sizeof(char));
	for (int i = 0; i < n; i++)
	{
		// Computes the last char which is given by
		// input_text[(suffix_arr[i] + n - 1) % n]
		int j = suffix_arr[i] - 1;
		if (j < 0)
			j = j + n;

		bwt_arr[i] = input_text[j];
	}

	bwt_arr[n-1] = '\n';


	// Returns the computed Burrows - Wheeler Transform
	return bwt_arr;
}

// Driver program to test functions above
int Begin_Reference()
{
	//The Same core program, i have just added txt file input/output and marking charecters for consistancy
	int len_text;

	ifstream inputfile("test.txt", ios::binary);

	if(!inputfile.is_open())
	{
		cout << "ERR: -- Incorrect file name\n";
		exit(1);
	}

	inputfile.seekg(0, inputfile.end);
	len_text = inputfile.tellg();
	inputfile.seekg(0, inputfile.beg);
	len_text--;

	char input_text[len_text+1];

	inputfile.read(input_text,len_text);
	if (!inputfile)
	{
		cout << "ERR: -- Charecters have not been read successfully" << endl;
		exit(1);
	}
	inputfile.close();

	++len_text;
	input_text[len_text-1] = char(7);

	// Computes the suffix array of our text
	int *suffix_arr = computeSuffixArray(input_text, len_text);

	// Adds to the output array the last char of each rotation
	char *bwt_arr = findLastChar(input_text, suffix_arr, len_text);

	//printf("Input text : %s\n", input_text);
	//printf("Burrows - Wheeler Transform : %s\n", bwt_arr);


	//opens a file called "encode.txt" and stores the char array output into it.
	ofstream  outputfile("reference.txt", ios::out | ios::binary);

	if (!outputfile)
	{
		cout << "ERR: -- Charecters cannot be written to file" << endl;
		exit(1);
	}

	outputfile.write(bwt_arr, len_text);

	free(bwt_arr);
	free(suffix_arr);
	return 0;
}
