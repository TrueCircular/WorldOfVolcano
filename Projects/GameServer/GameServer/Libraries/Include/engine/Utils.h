#pragma once
#include <dxgi1_2.h>
#include <wincodec.h>
#include <dxtk/ScreenGrab.h>
#include <dxtk/WICTextureLoader.h>

class Utils
{
public:
	static bool StartsWith(std::string str, std::string comp);
	static bool StartsWith(std::wstring str, std::wstring comp);

	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

	static std::wstring ToWString(std::string value);
	static std::string ToString(std::wstring value);

	static float Randstep(float min, float max);

	static void ScreenShot(ComPtr<ID3D11DeviceContext> context, const wstring& fileName);
	static Vec3 QuadToYawPitchRoll(Quaternion& q) {
		float sqw = q.w * q.w;
		float sqx = q.x * q.x;
		float sqy = q.y * q.y;
		float sqz = q.z * q.z;
		Vec3 vec;
		vec.x = atan2f(2.0f * (q.x * q.z + q.w * q.y), (-sqx - sqy + sqz + sqw)); //Yaw
		vec.y = asinf(2.0f * (q.w * q.x - q.y * q.z));//Pitch
		vec.z = atan2f(2.0f * (q.x * q.y + q.w * q.z), (-sqx + sqy - sqz + sqw));//Roll
		return vec;
	}
};