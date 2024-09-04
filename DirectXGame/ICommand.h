#pragma once
#include "player.h"

/// <summary>
/// ICommandクラスを宣言
/// </summary>
class ICommand
{
public://メンバ関数

	///仮想デストラクタ
	virtual ~ICommand();

	//純粋仮想関数
	virtual void Exec(Player& player) = 0;

};

/// <summary>
/// ICommandクラスを継承したMoveRightCommandクラスを作成する
/// </summary>
class MoveRightCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

/// <summary>
/// ICommandクラスを継承したMoveLeftCommandクラスを作成する
/// </summary>
class MoveLeftCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

/// <summary>
/// ICommandクラスを継承したMoveUpCommandクラスを作成する
/// </summary>
class MoveUpCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

/// <summary>
/// ICommandクラスを継承したMoveDownCommandクラスを作成する
/// </summary>
class MoveDownCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};
