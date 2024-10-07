#pragma once
#include "Vector3.h"
#include "variant"
#include "map"
#include "string"
#include "Windows.h"

#include "json.hpp"

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {

public:

	//インスタンス
	static GlobalVariables* GetInstance();

	/// <summary>
	/// マイフレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成 
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// ファイル書き出し
	/// </summary>
	/// <param name="groupName"></param>
	void SaveFile(const std::string& groupName);

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GlobalVariables() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GlobalVariables() = default;
	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="other"></param>
	GlobalVariables(const GlobalVariables& other) = delete;
	/// <summary>
	/// コピー代入演算子
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	GlobalVariables& operator=(const GlobalVariables& other) = delete;

	struct Item {
		std::variant<int32_t, float, Vector3> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> dates_;

	using json = nlohmann::json;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";


};
