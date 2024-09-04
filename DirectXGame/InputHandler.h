#pragma once
#include "ICommand.h"

/// <summary>
/// InputHandleクラスの宣言
/// </summary>
class InputHandler
{
public:
	// 入力を処理するための関数
	ICommand* HandleInput();

	// "D"キーに右移動コマンドを割り当てる関数
	void AssignMoveRightCommand2PressKeyD();

	// "A"キーに左移動コマンドを割り当てる関数
	void AssignMoveLeftCommand2PressKeyA();

	// "W"キーに上移動コマンドを割り当てる関数
	void AssignMoveUpCommand2PressKeyW();

	// "S"キーに下移動コマンドを割り当てる関数
	void AssignMoveDownCommand2PressKeyS();

private:

	// "D"キーが押されたときのコマンド
	ICommand* pressKeyD_;

	// "A"キーが押されたときのコマンド
	ICommand* pressKeyA_;

	// "W"キーが押されたときのコマンド
	ICommand* pressKeyW_;

	// "S"キーが押されたときのコマンド
	ICommand* pressKeyS_;
};

