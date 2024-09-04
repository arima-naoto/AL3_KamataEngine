#pragma once
#include "player.h"

class ICommand
{
public:

	virtual ~ICommand();

	virtual void Exec(Player& player) = 0;

};

class MoveRightCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

class MoveLeftCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

class MoveUpCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

class MoveDownCommand : public ICommand {

public://メンバ関数

	//Playerオブジェクトを操作するためのメンバ関数
	void Exec(Player& player) override;

};

class RotateRightCommand : public ICommand {

public:

	void Exec(Player& player) override;

};

class RotateLeftCommand : public ICommand {

public:
	void Exec(Player& player) override;
};