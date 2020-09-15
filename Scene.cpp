#include "Scene.h"
#include "Alice.h"
#include "utils.h"
#include "AliceRay.h"
#include "AliceSphere.h"
#include "AliceVector3.h"
#include "AliceMaterial.h"
#include "AliceCamera.h"
#include "AliceObject.h"
#include "Texture.h"
static int sViewportWidth = 0, sViewportHeight = 0;
static Alice::Object*sHeadObject = nullptr;
static Alice::Object*sSceneObjects[1024];
static Alice::BVHNode*sRootNode = nullptr;
static int sSceneObjectCount = 0;
static int sSampleCountPerPixel = 32;
static int sCurrnetRenderPixelX = 0;
static int sCurrentRenderPixelY = 0;
static bool sIsRenderDone = false;

static DWORD_PTR sSystemMask;
static unsigned long sProcessAffinityMask;
static __int64 sFrequency;
static __int64 sStartTime;
static void* sThreadHandle;

void AppendSceneObject(Alice::Object*object) {
	if (sHeadObject == nullptr) {
		sHeadObject = object;
	}
	else {
		sHeadObject->PushBack(object);
	}
	sSceneObjects[sSceneObjectCount++] = object;
}
Alice::Vector3 GetBackgroundColor(Alice::Ray & input_ray) {
	float t = 0.5f*(input_ray.mDirection.y + 1.0f);
	return (1.0f - t)*Alice::Vector3(1.0f, 1.0f, 1.0f) + t * Alice::Vector3(0.5f, 0.7f, 1.0f);
}
Alice::Vector3 RenderOneSample(Alice::Ray & input_ray, int bounce_time) {
	Alice::HitPoint hitPoint;
	if (sRootNode->HitTest(input_ray, 0.001f, 100.0f, hitPoint)) {
		if (bounce_time < 50) {
			Alice::Ray scattered;
			if (hitPoint.mMaterial->Scatter(input_ray, scattered, hitPoint)) {
				Alice::Vector3 color = RenderOneSample(scattered, bounce_time + 1);
				return  Alice::MultiplyComponentWise(scattered.mLightingAttenuation, color);
			}
		}
		return Alice::Vector3(0.0f, 0.0f, 0.0f);
	}
	return Alice::Vector3(1.0f,1.0f,1.0f);// GetBackgroundColor(input_ray);
}
Alice::Vector3 RenderOnePixel(int x, int y, int sample_count_per_pixel) {
	Alice::Vector3 color(0.0f, 0.0f, 0.0f);
	for (int s = 0; s < sample_count_per_pixel; s++) {
		float u = (float(x) + randf()) / float(sViewportWidth);
		float v = (float(y) + randf()) / float(sViewportHeight);
		Alice::Ray ray = Alice::Camera::Singleton()->GetRay(u, v);
		color = color + RenderOneSample(ray, 0);
	}
	color = color * (1.0f / float(sample_count_per_pixel));
	return color;
}
void InitManualScene() {
	Alice::Camera::Singleton()->LookAt(Alice::Vector3(10.0f, 5.0f, 5.0f), Alice::Vector3(0.0f, 0.0f, 0.0f), Alice::Vector3(0.0f, 1.0f, 0.0f));
	Alice::Texture*noise_texture = new Alice::NoiseTexture(5.0f);
	Alice::Object*object = new Alice::Object(new Alice::Sphere(Alice::Vector3(0.0f, 2.0f, 0.0f), 2.0f), new Alice::LambertMaterial(noise_texture));
	object->SetName("root ball");
	AppendSceneObject(object);
	object = new Alice::Object(new Alice::Sphere(Alice::Vector3(0.0f, -1000.0f, 0.0f), 1000.0f), new Alice::LambertMaterial(noise_texture));
	object->SetName("bottom");
	AppendSceneObject(object);
	sRootNode = new Alice::BVHNode;
	sRootNode->BuildBoundingVolumeHierarchy(sSceneObjects, sSceneObjectCount);
}
void InitRandomScene() {
	Alice::Camera::Singleton()->LookAt(Alice::Vector3(13.0f, 2.0f, 3.0f), Alice::Vector3(0.0f, 0.0f, 0.0f), Alice::Vector3(0.0f, 1.0f, 0.0f));
	Alice::Texture*green_texture = new Alice::ConstantTexture(Alice::Vector3(0.2f, 0.3f, 0.1f));
	Alice::Texture*white_texture = new Alice::ConstantTexture(Alice::Vector3(0.9f, 0.9f, 0.9f));
	Alice::Texture*checker_texture = new Alice::CheckerTexture(green_texture, white_texture);
	Alice::Texture*noise_texture = new Alice::NoiseTexture(0.3f);
	for (int x = -10; x < 10; ++x) {
		for (int z = -10; z < 10; ++z) {
			float choose_material = randf();
			Alice::Vector3 center(x + 0.9f*randf(), 0.2f, z + 0.9f*randf());
			if ((center - Alice::Vector3(4.0f, 0.2f, 0.0f)).magnitude() > 0.9f) {
				if (choose_material < 0.4f) {
					Alice::Object*object = new Alice::Object(
						new Alice::Sphere(center, 0.2f),
						new Alice::LambertMaterial(new Alice::ConstantTexture(Alice::Vector3(randf()*randf(), randf()*randf(), randf()*randf()))));
					AppendSceneObject(object);
				}
				else if (choose_material < 0.85f) {
					Alice::Object*object = new Alice::Object(
						new Alice::Sphere(center, 0.2f),
						new Alice::MetalMaterial(Alice::Vector3(0.5f*(1.0f + randf()), 0.5f*(1.0f + randf()), 0.5f*(1.0f + randf())), 0.5f*randf()));
					AppendSceneObject(object);
				}
				else {
					Alice::Object*object = new Alice::Object(
						new Alice::Sphere(center, 0.2f),
						new Alice::DielectricMaterial(1.5f));
					AppendSceneObject(object);
				}
			}
		}
	}
	Alice::Object*object = new Alice::Object(new Alice::Sphere(Alice::Vector3(0.0f, -1000.0f, 0.0f), 1000.0f), new Alice::LambertMaterial(noise_texture));
	object->SetName("bottom");
	AppendSceneObject(object);
	sRootNode = new Alice::BVHNode;
	sRootNode->BuildBoundingVolumeHierarchy(sSceneObjects, sSceneObjectCount);
}
void InitScene() {
	//InitRandomScene();
	InitManualScene();
	aClearColor(0, 0, 0, 255);
	aClear(A_COLOR_BUFFER_BIT);
	GetProcessAffinityMask(GetCurrentProcess(), &sProcessAffinityMask, &sSystemMask);
	SetThreadAffinityMask(sThreadHandle, 1);
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&sFrequency));
	SetThreadAffinityMask(sThreadHandle, sProcessAffinityMask);
}
void OnViewportChanged(int w, int h) {
	sViewportWidth = w;
	sViewportHeight = h;
	aViewport(0, 0, w, h);
	Alice::Camera::Singleton()->Set(45.0f, float(sViewportWidth) / float(sViewportHeight), 2.0f);
}
__int64 GetElapsedTime(__int64 start_time) {
	LARGE_INTEGER curTime;
	SetThreadAffinityMask(sThreadHandle, 1);
	QueryPerformanceCounter(&curTime);
	SetThreadAffinityMask(sThreadHandle, sProcessAffinityMask);
	return curTime.QuadPart - start_time;
}
bool Render() {
	if (sIsRenderDone){
		return false;
	}
	static int sPixelCount = sViewportWidth * sViewportHeight;
	__int64 start_time = GetElapsedTime(0);
	bool needupdate = false;
	while (true){
		if (sCurrnetRenderPixelX<sViewportWidth){
			Alice::Vector3 color = RenderOnePixel(sCurrnetRenderPixelX, sCurrentRenderPixelY, sSampleCountPerPixel);
			int ir = int(255.99f*color.x);
			int ig = int(255.99f*color.y);
			int ib = int(255.99f*color.z);
			aSetColor(sCurrnetRenderPixelX, sCurrentRenderPixelY, ir, ig, ib, 255);
			sCurrnetRenderPixelX++;
			needupdate = true;
		} else {
			if (sCurrentRenderPixelY<sViewportHeight){
				sCurrentRenderPixelY++;
				sCurrnetRenderPixelX = 0;
			}
			else {
				sIsRenderDone = true;
				break;
			}
		}
		__int64 time_elapsed = GetElapsedTime(start_time);
		double seconds = double(time_elapsed) / double(sFrequency);
		if (seconds>0.033){
			return needupdate;
		}
	}
	return needupdate;
}