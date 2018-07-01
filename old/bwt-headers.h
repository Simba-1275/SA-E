#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include <cstdlib>
#include <string.h>

using namespace std;

int Begin_Encode();
int Begin_Decode();
int Begin_Reference();

void get_rotations(int size, string input, char* output); //  -- Encode --
void regen_rotations(int /*size*/, char /*in[]*/, char /*output[]*/); //  -- Decode --
void write_output(int size, char* output); //  -- Encode --
void _write_output(int /*size*/, char /*output[]*/);//  -- Decode --
