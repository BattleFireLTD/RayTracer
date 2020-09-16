#include "Texture.h"
#include "utils.h"
namespace Alice {
	TextureRGB::TextureRGB() {
		mImageData = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
	void TextureRGB::SetImage(const char * path) {
		int file_size = 0;
		unsigned char * file_content=LoadFileContent(path, file_size);
		mImageData = DecodeBMP(file_content, mWidth, mHeight);
	}
	Vector3 TextureRGB::Sample(float u, float v, const Vector3 & p) const {
		if (mImageData==nullptr){
			return Vector3(0.0f,0.0f,0.0f);
		}
		int i = int(u*mWidth);
		int j = int(v*mHeight);
		i = i >= 0 ? i : 0;
		j = j >= 0 ? j : 0;
		i = i <= mWidth - 1 ? i : mWidth - 1;
		j = j <= mHeight - 1 ? j : mHeight - 1;
		int pixel_index = i + j * mWidth;
		float r = float(mImageData[pixel_index * 3]) / 255.0f;
		float g = float(mImageData[pixel_index * 3 + 1]) / 255.0f;
		float b = float(mImageData[pixel_index * 3 + 2]) / 255.0f;
		return Vector3(r,g,b);
	}
}