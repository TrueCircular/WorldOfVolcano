#pragma once

#include "engine/EnginePch.h"
#pragma comment(lib, "engine/EngineLib.lib")

#pragma comment(lib, "../../Libraries/lib/server/ServerCore.lib")
#include "CorePch.h"


#include "DamageIndicator.h" 


struct FresnelDesc {
	Vec4 eyePos;
	Vec4 eyeLook;
};
struct ColorDesc {
	Vec4 baseColor;
	Vec4 subColor;
};