#include "InputHandler.h"
#include "Input.h"

void InputHandler::Assign_MoveRight_Command2_PressKeyRightArrow() {
	ICommand* command = new MoveRightCommand();
	this->pressKeyRight_ = command;
}

void InputHandler::Assign_MoveLeft_Command2_PressKeyLeftArrow() {
	ICommand* command = new MoveLeftCommand();
	this->pressKeyLeft_ = command;
}

void InputHandler::Assign_MoveUp_Command2_PressKeyUpArrow() {
	ICommand* command = new MoveUpCommand();
	this->pressKeyUp_ = command;
}

void InputHandler::Assign_MoveDown_Command2_PressKeyDownArrow() {
	ICommand* command = new MoveDownCommand();
	this->pressKeyDown_ = command;
}

void InputHandler::Assign_RotateRight_Command2_PressKeyD() { 
	ICommand* command = new RotateRightCommand(); 
	this->pressKeyD_ = command;
}

void InputHandler::Assign_RotateLeft_Command2_PressKeyA() {
	ICommand* command = new RotateLeftCommand(); 
	this->pressKeyA_ = command;
}

ICommand* InputHandler::HandleInput() {

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_RIGHT)) { return pressKeyRight_; }
	
	if (input->PushKey(DIK_LEFT)) { return pressKeyLeft_; }
	
	if (input->PushKey(DIK_UP)) { return pressKeyUp_; }
	
	if (input->PushKey(DIK_DOWN)) { return pressKeyDown_; }
	
	if (input->PushKey(DIK_D)) { return pressKeyD_; }
	
	if (input->PushKey(DIK_A)) { return pressKeyA_; }
	
	return nullptr;
}
