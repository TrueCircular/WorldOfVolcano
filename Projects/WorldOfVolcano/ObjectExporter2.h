#pragma once

class ObjectExporter2
{
	bool isLoaded = false;
public:
	vector<pair<wstring, Vec3>> enemyListforServer;
public:
	bool OpenFile(wstring filename);
};

