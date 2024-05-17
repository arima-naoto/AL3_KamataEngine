#pragma once
#include <vector>
#include <string>
#include <Vector3.h>
using namespace std;

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipDate 
{
	vector<vector<MapChipType>> date;
};

class MapChipField 
{
private:

	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;

public:

	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };

	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };

public:

	void ResetMapChipDate();

	void LoadMapChipCsv(const string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
};
