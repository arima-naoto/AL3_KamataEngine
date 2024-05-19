#pragma once
#include <vector>
#include <string>
#include <Vector3.h>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipDate 
{
	std::vector<std::vector<MapChipType>> date;
};

class MapChipField 
{
private:

	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipDate mapChipDate_;

public:

	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; };

	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; };

public:

	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
};
