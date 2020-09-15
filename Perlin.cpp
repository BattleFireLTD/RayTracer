#include "Perlin.h"
#include "utils.h"
namespace Alice {
	static float * GenPerlin() {
		float *p = new float[256];
		for (int i = 0; i < 256; i++){
			p[i] = randf();
		}
		return p;
	}
	void Permute(int*p, int n) {
		for (int i=n-1;i>0;--i){
			int target = int(randf()*(i + 1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}
	static int * PerlinGenPermute() {
		int *p = new int[256];
		for (int i=0;i<256;++i){
			p[i] = i;
		}
		Permute(p, 256);
		return p;
	}
	float*Perlin::mRandomFloat = GenPerlin();
	int*Perlin::mPermX = PerlinGenPermute();
	int*Perlin::mPermY = PerlinGenPermute();
	int*Perlin::mPermZ = PerlinGenPermute();
	static float TrilinearInterpolate(float c[2][2][2], float u, float v, float w) {
		float accm = 0.0f;
		for (int i=0;i<2;++i){
			for (int j=0;j<2;++j){
				for (int k=0;k<2;++k){
					accm += (i*u+(1-i)*(1-u))*(j*v+(1-j)*(1-v))*(k*w+(1-k)*(1-w))*c[i][j][k];
				}
			}
		}
		return accm;
	}
	float Perlin::Noise0(const Vector3 & p) const {
		int i = int(4 * p.x) & 255;
		int j = int(4 * p.y) & 255;
		int k = int(4 * p.z) & 255;
		return mRandomFloat[mPermX[i] ^ mPermY[j] ^ mPermZ[k]];
	}
	float Perlin::Noise1(const Vector3 & p) const {
		float u = p.x - floorf(p.x);
		float v = p.x - floorf(p.y);
		float w = p.x - floorf(p.z);
		int i = int(floorf(p.x)); //int(4 * p.x) & 255;
		int j = int(floorf(p.y)); //int(4 * p.y) & 255;
		int k = int(floorf(p.z)); //int(4 * p.z) & 255;
		float c[2][2][2];
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = mRandomFloat[mPermX[(i+di)&255] ^ mPermY[(j + dj) & 255] ^ mPermZ[(k + dk) & 255]];
				}
			}
		}
		return TrilinearInterpolate(c,u,v,w);
	}
}