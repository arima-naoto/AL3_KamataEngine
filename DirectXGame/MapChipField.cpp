#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

namespace {

map<string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipDate() 
{ 
    mapChipDate_.date.clear();
	mapChipDate_.date.resize(kNumBlockVirtical);

    for (vector<MapChipType>& MapChipDateLine : mapChipDate_.date) {
		MapChipDateLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const string& filePath) { 
    ResetMapChipDate();

    ifstream file;

    file.open(filePath);

    assert(file.is_open());

    stringstream mapChipCsv;

    mapChipCsv << file.rdbuf();

    file.close();

    for (uint32_t i = 0; i < kNumBlockVirtical; ++i) 
    {
		string line;
		getline(mapChipCsv, line);

        istringstream line_stream(line);

        for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) 
        {
			string word;

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
    return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockHorizontal - 1 - yIndex), 0); 
}

