#pragma once
#include <stdio.h>
#include <random>
unsigned char *LoadFileContent(const char *path, int &filesize);
unsigned char *DecodeBMP(unsigned char*filecontent, int&width, int&height); 
float randf();//0.0~1.0
float srandf();//-1.0~1.0
void SavePixelToFile(const char*filePath, int width, int height, unsigned char * pixel);

template<typename T>
inline T min_of(T a, T b) { return a < b ? a : b; }

template<typename T>
inline T max_of(T a, T b) { return a > b ? a : b; }