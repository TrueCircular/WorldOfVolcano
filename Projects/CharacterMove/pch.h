#pragma once

#include "engine/EnginePch.h"
#pragma comment(lib, "engine/EngineLib.lib")

#pragma comment(lib, "../../Libraries/lib/server/ServerCore.lib")
#include "CorePch.h"


#include "DamageIndicator.h" 



static Vec3 QuadToYawPitchRoll(Quaternion& q) {
	float sqw = q.w * q.w;
	float sqx = q.x * q.x;
	float sqy = q.y * q.y;
	float sqz = q.z * q.z;
	Vec3 vec;
	vec.x = atan2f(2.0f * (q.x * q.z + q.w * q.y), (-sqx - sqy + sqz + sqw)); //Yaw
	vec.y = asinf(2.0f * (q.w*q.x-q.y*q.z));//Pitch
	vec.z = atan2f(2.0f * (q.x * q.y + q.w * q.z), (-sqx + sqy - sqz + sqw));//Roll
	return vec;
}