#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipDate() 
{ 
    mapChipDate_.date.clear();
	mapChipDate_.date.resize(kNumBlockVirtical);

    for (std::vector<MapChipType>& MapChipDateLine : mapChipDate_.date) {
		MapChipDateLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) { 
    ResetMapChipDate();

    std::ifstream file;

    file.open(filePath);

    assert(file.is_open());

    std::stringstream mapChipCsv;

    mapChipCsv << file.rdbuf();

    file.close();

    for (uint32_t i = 0; i < kNumBlockVirtical; ++i) 
    {
		std::string line;
		getline(mapChipCsv, line);

        std::istringstream line_stream(line);

        for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) 
        {
			std::string word;

            getline(line_stream, word, ',');

            if (mapChipTable.contains(word)) 
            {
				mapChipDate_.date[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) 
{
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}

    if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
    }

    return mapChipDate_.date[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) 
{ 
    return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical- 1 - yIndex), 0); 
}

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

    uint32_t xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);

    uint32_t beforeYindex = static_cast<uint32_t>((position.y + kBlockHeight / 2.0f) / kBlockHeight);

    uint32_t yindex = kNumBlockVirtical - 1 - beforeYindex;

    IndexSet indexSet{xIndex, yindex};

    return indexSet;

}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	// 指定ブロックの中心座標を取得する
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;    // 短形左端
	rect.right = center.x + kBlockWidth / 2.0f;   // 短形右端
	rect.bottom = center.y - kBlockHeight / 2.0f; // 短形下端
	rect.top = center.y + kBlockHeight / 2.0f;    // 短形上端
	return rect;
}
