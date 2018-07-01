#include "benchmark/benchmark.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include "BWTSE.cc"
#include "BWTSD.cc"
#include "includes.h"

using namespace std;

static void BM_BWT_Encode1(benchmark::State& state) {
    ifstream t("8.txt");
    string input((istreambuf_iterator<char>(t)),
                     istreambuf_iterator<char>());

    int size = input.length();
    char* output = new char[size];


    for (auto _ : state){
        get_rotations(/*int*/size, /*string*/input, /*char[size]*/output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));

    delete[] output;
}

static void BM_BWT_Encode2(benchmark::State& state) {
    ifstream t("512.txt");
    string input((istreambuf_iterator<char>(t)),
                     istreambuf_iterator<char>());

    int size = input.length();
    char* output = new char[size];


    for (auto _ : state){
        get_rotations(/*int*/size, /*string*/input, /*char[size]*/output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));

    delete[] output;
}

static void BM_BWT_Encode3(benchmark::State& state) {
    ifstream t("1024.txt");
    string input((istreambuf_iterator<char>(t)),
                     istreambuf_iterator<char>());

    int size = input.length();
    char* output = new char[size];

    for (auto _ : state){
        get_rotations(/*int*/size, /*string*/input, /*char[size]*/output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));

    delete[] output;
}

static void BM_BWT_Encode4(benchmark::State& state) {
    ifstream t("2048.txt");
    string input((istreambuf_iterator<char>(t)),
                     istreambuf_iterator<char>());

    int size = input.length();
    char* output = new char[size];

    for (auto _ : state){
        get_rotations(/*int*/size, /*string*/input, /*char[size]*/output);
    }
    //variable in[] located in heap memory

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));

    delete[] output;
}


static void BM_BWT_Decode1(benchmark::State& state) {
    int size;
    ifstream inputfile("d8.txt", ios::binary);

    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    char* in = new char [size];

    inputfile.read(in,size);
    inputfile.close();

    char* output = new char [size];

    for (auto _ : state){
        regen_rotations(size, in, output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));
    delete[] output;
    delete[] in;
}

static void BM_BWT_Decode2(benchmark::State& state) {
    int size;
    ifstream inputfile("d512.txt", ios::binary);

    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    char* in = new char [size];

    inputfile.read(in,size);
    inputfile.close();

    char* output = new char [size];

    for (auto _ : state){
        regen_rotations(size, in, output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));
    delete[] output;
    delete[] in;
}

static void BM_BWT_Decode3(benchmark::State& state) {
    int size;
    ifstream inputfile("d1024.txt", ios::binary);

    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    char* in = new char [size];

    inputfile.read(in,size);
    inputfile.close();

    char* output = new char [size];

    for (auto _ : state){
        regen_rotations(size, in, output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));
    delete[] output;
    delete[] in;
}

static void BM_BWT_Decode4(benchmark::State& state) {
    int size;
    ifstream inputfile("d2048.txt", ios::binary);

    inputfile.seekg(0, inputfile.end);
    size = inputfile.tellg();
    inputfile.seekg(0, inputfile.beg);

    char* in = new char [size];

    inputfile.read(in,size);
    inputfile.close();

    char* output = new char [size];

    for (auto _ : state){
        regen_rotations(size, in, output);
    }

    state.SetBytesProcessed(int64_t(state.iterations()) *  int64_t(state.range(0)));
    delete[] output;
    delete[] in;
}


// Register the function as a benchmark
BENCHMARK(BM_BWT_Encode1)->Arg(8);
BENCHMARK(BM_BWT_Encode2)->Arg(512);
BENCHMARK(BM_BWT_Encode3)->Arg(1024);
BENCHMARK(BM_BWT_Encode4)->Arg(2048);

BENCHMARK(BM_BWT_Decode1)->Arg(8);
BENCHMARK(BM_BWT_Decode2)->Arg(512);
BENCHMARK(BM_BWT_Decode3)->Arg(1024);
BENCHMARK(BM_BWT_Decode4)->Arg(2048);
BENCHMARK_MAIN();
