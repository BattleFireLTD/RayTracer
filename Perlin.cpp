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
	static Vector3 * GenPerlinVector3() {
		Vector3 *p = new Vector3[256];
		for (int i = 0; i < 256; i++) {
			p[i].x = -1.0f + 2.0f*randf();
			p[i].y = -1.0f + 2.0f*randf();
			p[i].z = -1.0f + 2.0f*randf();
			p[i].normalize();
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
	float*Perlin::mRandomFloats = GenPerlin();
	Vector3*Perlin::mRandomVectors = GenPerlinVector3();
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
	static float TrilinearInterpolateVector3(Vector3 c[2][2][2], float u, float v, float w) {
		float uu = u * u*(3 - 2 * u);
		float vv = v * v*(3 - 2 * v);
		float ww = w * w*(3 - 2 * w);
		float accm=0.0f;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				for (int k = 0; k < 2; ++k) {
					Vector3 weight(u-i,v-j,w-k);
					accm = accm + (i*uu + (1 - i)*(1 - uu))*(j*vv + (1 - j)*(1 - vv))*(k*ww + (1 - k)*(1 - ww))*(c[i][j][k]* weight);
				}
			}
		}
		return accm;
	}
	Perlin*Perlin::mSelf = nullptr;
	Perlin*Perlin::Singleton() {
		if (nullptr==mSelf){
			mSelf = new Perlin;
		}
		return mSelf;
	}
	float Perlin::NoiseOriginal(const Vector3 & p) const {
		int i = int(4 * p.x) & 255;
		int j = int(4 * p.y) & 255;
		int k = int(4 * p.z) & 255;
		return mRandomFloats[mPermX[i] ^ mPermY[j] ^ mPermZ[k]];
	}
	float Perlin::NoiseTrilinear(const Vector3 & p) const {
		float u = p.x - floorf(p.x);
		float v = p.y - floorf(p.y);
		float w = p.z - floorf(p.z);
		int i = int(floorf(p.x));
		int j = int(floorf(p.y));
		int k = int(floorf(p.z));
		float c[2][2][2];
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = mRandomFloats[mPermX[(i+di)&255] ^ mPermY[(j + dj) & 255] ^ mPermZ[(k + dk) & 255]];
				}
			}
		}
		return TrilinearInterpolate(c,u,v,w);
	}
	float Perlin::NoiseTrilinearHermiteCubic(const Vector3 & p) const {
		float u = p.x - floorf(p.x);
		float v = p.y - floorf(p.y);
		float w = p.z - floorf(p.z); 
		u = u * u*(3 - 2 * u);
		v = v * v*(3 - 2 * v);
		w = w * w*(3 - 2 * w);
		int i = int(floorf(p.x));
		int j = int(floorf(p.y));
		int k = int(floorf(p.z));
		float c[2][2][2];
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = mRandomFloats[mPermX[(i + di) & 255] ^ mPermY[(j + dj) & 255] ^ mPermZ[(k + dk) & 255]];
				}
			}
		}
		return TrilinearInterpolate(c, u, v, w);
	}
	float Perlin::NoiseVector3TrilinearHermiteCubic(const Vector3 & p) const {
		float u = p.x - floorf(p.x);
		float v = p.y - floorf(p.y);
		float w = p.z - floorf(p.z);
		int i = int(floorf(p.x));
		int j = int(floorf(p.y));
		int k = int(floorf(p.z));
		Vector3 c[2][2][2];
		for (int di = 0; di < 2; ++di) {
			for (int dj = 0; dj < 2; ++dj) {
				for (int dk = 0; dk < 2; ++dk) {
					c[di][dj][dk] = mRandomVectors[mPermX[(i + di) & 255] ^ mPermY[(j + dj) & 255] ^ mPermZ[(k + dk) & 255]];
				}
			}
		}
		return TrilinearInterpolateVector3(c, u, v, w);
	}
	float Perlin::Turb(const Vector3 &p, int depth/* =7 */) const {
		float accum = 0.0f;
		Vector3 temp_p = p;
		float weight = 1.0f;
		for (int i = 0; i < depth; i++){
			accum += weight * NoiseVector3TrilinearHermiteCubic(temp_p);
			weight *= 0.5f;
			temp_p = temp_p * 2.0f;
		}
		return fabs(accum);
	}
}