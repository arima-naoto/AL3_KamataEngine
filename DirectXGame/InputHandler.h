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

	// "Right"キーに右移動コマンドを割り当てる関数
	void AssignMoveRightCommand2PressKeyRight();

	// "Left"キーに左移動コマンドを割り当てる関数
	void AssignMoveLeftCommand2PressKeyLeft();

	// "Up"キーに上移動コマンドを割り当てる関数
	void AssignMoveUpCommand2PressKeyUp();

	// "Down"キーに下移動コマンドを割り当てる関数
	void AssignMoveDownCommand2PressKeyDown();

private:

	// "Right"キーが押されたときのコマンド
	ICommand* pressKeyRight_;

	// "Left"キーが押されたときのコマンド
	ICommand* pressKeyLeft_;

	// "Up"キーが押されたときのコマンド
	ICommand* pressKeyUp_;

	// "Down"キーが押されたときのコマンド
	ICommand* pressKeyDown_;
};

