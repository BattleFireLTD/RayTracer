#include "utils.h"
#include <windows.h>
unsigned char * LoadFileContent(const char *path, int &filesize) {
	filesize = 0;
	unsigned char *filecontent = nullptr;
	FILE *pFile = fopen(path, "rb");
	if (pFile){
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen>0){
			rewind(pFile);
			filecontent = new unsigned char[nLen+1];
			fread(filecontent, sizeof(unsigned char), nLen, pFile);
			filecontent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return filecontent;
}
unsigned char *DecodeBMP(unsigned char*filecontent, int&width, int&height) {
	if (0x4D42==*((unsigned short*)filecontent)){
		int pixelOffset = *((int*)(filecontent + 10));
		width = *((int*)(filecontent + 18));
		height = *((int*)(filecontent + 22));
		unsigned char*pixelData = filecontent + pixelOffset;
		for (int i = 0; i < width*height * 3;i+=3) {
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i+2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	return nullptr;
}
float randf(){
	return (float)rand() / (float)RAND_MAX;
}
float srandf(){
	return (randf() - 0.5f)*2.0f;
}
void SavePixelToFile(const char*filePath,int width, int height,unsigned char * pixel){
	FILE*pFile = fopen(filePath, "wb");
	if (pFile){
		BITMAPFILEHEADER bfh;
		memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
		bfh.bfType = 0x4D42;
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, pFile);

		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(BITMAPINFOHEADER));
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biBitCount = 24;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, pFile);
		//rgb -> bgr
		for (int i = 0; i < width*height * 3; i += 3){
			unsigned char temp = pixel[i];
			pixel[i] = pixel[i + 2];
			pixel[i + 2] = temp;
		}
		fwrite(pixel, 1, width*height * 3, pFile);
		fclose(pFile);
	}
}