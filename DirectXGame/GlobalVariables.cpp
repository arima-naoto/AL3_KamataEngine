#include "GlobalVariables.h"

#ifdef _DEBUG
#include "imgui.h"
using namespace ImGui;
#endif // _DEBUG


GlobalVariables* GlobalVariables::GetInstance() { 
	//静的インスタンスを作成
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {

	if (!Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		End();
		return;
	}

	if (BeginMenuBar()) {

		// 各グループについて
		for (std::map<std::string, Group>::iterator itGroup = dates_.begin(); itGroup != dates_.end(); ++itGroup) {

			// グループ名を取得
			const std::string& groupName = itGroup->first;
			// グループの参照を取得
			Group& group = itGroup->second;

			// メニューを開く
			if (BeginMenu(groupName.c_str())) {

				// 各項目について
				for (std::map<std::string, Item>::iterator itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {

					// 項目名を取得
					const std::string& itemName = itItem->first;
					// 項目の参照を取得
					Item& item = itItem->second;

					// int32_t型の値を保持していれば
					if (std::holds_alternative<int32_t>(item.value)) {
						int32_t* ptr = std::get_if<int32_t>(&item.value);
						SliderInt(itemName.c_str(), ptr, 0, 100);
					}
					// float型の値を保持していれば
					else if (std::holds_alternative<float>(item.value)) {
						float* ptr = std::get_if<float>(&item.value);
						SliderFloat(itemName.c_str(), ptr, 0, 100);
					}
					// Vector3型の値を保持していれば
					else if (std::holds_alternative<Vector3>(item.value)) {
						Vector3* ptr = std::get_if<Vector3>(&item.value);
						SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10, 10);
					}
				}
				EndMenu();
			}
		}

		// メニューバーを閉じる
		EndMenuBar();
	}
	// ウィンドウを閉じる
	End();
}
void GlobalVariables::CreateGroup(const std::string& groupName) { 
	dates_[groupName]; 
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, int32_t value) {

	Group& group = dates_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {

	Group& group = dates_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3& value) {

	Group& group = dates_[groupName];

	Item newItem{};
	newItem.value = value;

	group.items[key] = newItem;
}