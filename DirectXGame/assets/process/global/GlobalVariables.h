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
	/// マイフレーム処理
	void Update();
	/// グループの作成 
	void CreateGroup(const std::string& groupName);

	///値のセット(int32_t型)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	///値のセット(float型)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	///値のセット(Vector3型)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);
	/// 値のセット(bool型)
	void SetValue(const std::string& groupName, const std::string& key, bool value);

	/// ファイル書き出し
	void SaveFile(const std::string& groupName);

	/// ディレクトリの全ファイル読み込み
	void LoadFiles();

	/// ファイルから読み込む
	void LoadFile(const std::string& groupName);

	///項目の追加
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3 &value);
	void AddItem(const std::string& groupName, const std::string& key, bool value);

	///値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
    float GetfloatValue(const std::string& groupName, const std::string& key) const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key) const;
	bool GetBoolValue(const std::string& groupName, const std::string& key) const;
	
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
		std::variant<int32_t, float, Vector3,bool> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

	std::map<std::string, Group> dates_;

	using json = nlohmann::json;

	//グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalVariables/";


};
