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
	void Assign_MoveRight_Command2_PressKeyRightArrow();

	// "Left"キーに左移動コマンドを割り当てる関数
	void Assign_MoveLeft_Command2_PressKeyLeftArrow();

	// "Up"キーに上移動コマンドを割り当てる関数
	void Assign_MoveUp_Command2_PressKeyUpArrow();

	// "Down"キーに下移動コマンドを割り当てる関数
	void Assign_MoveDown_Command2_PressKeyDownArrow();

	// "D"キーに下移動コマンドを割り当てる関数
	void Assign_RotateRight_Command2_PressKeyD();
	
	// "A"キーに下移動コマンドを割り当てる関数
	void Assign_RotateLeft_Command2_PressKeyA();

private:

	// "Right"キーが押されたときのコマンド
	ICommand* pressKeyRight_;

	// "Left"キーが押されたときのコマンド
	ICommand* pressKeyLeft_;

	// "Up"キーが押されたときのコマンド
	ICommand* pressKeyUp_;

	// "Down"キーが押されたときのコマンド
	ICommand* pressKeyDown_;

	// "D"キーが押されたときのコマンド
	ICommand* pressKeyD_;
	
	// "A"キーが押されたときのコマンド
	ICommand* pressKeyA_;

};

