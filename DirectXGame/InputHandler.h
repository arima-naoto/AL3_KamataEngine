#pragma once
#include "ICommand.h"

/// <summary>
/// InputHandleクラスの宣言
/// </summary>
class InputHandler
{
public:
	

	// "D"キーに右移動コマンドを割り当てる関数
	void Assign_MoveRight_Command2_PressKeyD();

	// "A"キーに左移動コマンドを割り当てる関数
	void Assign_MoveLeft_Command2_PressKeyA();

	// "W"キーに上移動コマンドを割り当てる関数
	void Assign_MoveUp_Command2_PressKeyW();

	// "S"キーに下移動コマンドを割り当てる関数
	void Assign_MoveDown_Command2_PressKeyS();

	// "E"キーに下移動コマンドを割り当てる関数
	void Assign_RotateRight_Command2_PressKeyE();
	
	// "Q"キーに下移動コマンドを割り当てる関数
	void Assign_RotateLeft_Command2_PressKeyQ();

	//左右移動
	ICommand* InputVirtical();

	//上下移動
	ICommand* InputHorizontal();

	//左右移動
	ICommand* InputRotateVirtical();

private:

	// "D"キーが押されたときのコマンド
	ICommand* pressKeyD_;

	// "A"キーが押されたときのコマンド
	ICommand* pressKeyA_;

	// "W"キーが押されたときのコマンド
	ICommand* pressKeyW_;

	// "S"キーが押されたときのコマンド
	ICommand* pressKeyS_;

	// "E"キーが押されたときのコマンド
	ICommand* pressKeyE_;
	
	// "Q"キーが押されたときのコマンド
	ICommand* pressKeyQ_;

};

