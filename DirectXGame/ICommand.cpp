#include "ICommand.h"

///仮想デストラクタの定義
ICommand::~ICommand(){}

///MoveRightCommandクラスのメンバ関数Execを定義する
void MoveRightCommand::Exec(Player &player) {

	//playerクラスからメンバ関数MoveRightを呼び出す
	player.MoveRight();
}

//MoveLeftCommandクラスのメンバ関数Execを定義する
void MoveLeftCommand::Exec(Player& player) {
	
	//Playerクラスからメンバ関数MoveLeftを呼び出す
	player.MoveLeft();
}

///MoveUpCommandクラスのメンバ関数Execを定義する
void MoveUpCommand::Exec(Player& player) {

	//Playerクラスからメンバ関数MoveUpを呼び出す
	player.MoveUp();
}

///MoveDownCommandクラスのメンバ関数Execを定義する
void MoveDownCommand::Exec(Player& player) {

	//Playerクラスからメンバ関数MoveDownを呼び出す
	player.MoveDown();
}