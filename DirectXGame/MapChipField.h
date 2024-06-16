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


/// <summary>
/// マップチップフィールドクラスの宣言
/// </summary>
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

	/// <summary>
	/// マップチップ番号の構造体
	/// </summary>
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	/// <summary>
	/// 範囲短形構造体の宣言
	/// </summary>
	struct Rect {
		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

	void ResetMapChipDate();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);



};
