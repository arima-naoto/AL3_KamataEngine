#include "InputHandler.h"
#include "Input.h"

void InputHandler::Assign_MoveRight_Command2_PressKeyD() {
	ICommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}

void InputHandler::Assign_MoveLeft_Command2_PressKeyA() {
	ICommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}

void InputHandler::Assign_MoveUp_Command2_PressKeyW() {
	ICommand* command = new MoveUpCommand();
	this->pressKeyW_ = command;
}

void InputHandler::Assign_MoveDown_Command2_PressKeyS() {
	ICommand* command = new MoveDownCommand();
	this->pressKeyS_ = command;
}

void InputHandler::Assign_RotateRight_Command2_PressKeyE() { 
	ICommand* command = new RotateRightCommand(); 
	this->pressKeyQ_ = command;
}

void InputHandler::Assign_RotateLeft_Command2_PressKeyQ() {
	ICommand* command = new RotateLeftCommand(); 
	this->pressKeyE_ = command;
}

ICommand* InputHandler::InputVirtical() {

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_D)) {
		return pressKeyD_;
	}

	if (input->PushKey(DIK_A)) {
		return pressKeyA_;
	}

	return nullptr;
}

ICommand* InputHandler::InputHorizontal() {

	Input* input = Input::GetInstance();
	
	if (input->PushKey(DIK_W)) {
		return pressKeyW_;
	}

	if (input->PushKey(DIK_S)) {
		return pressKeyS_;
	}

	return nullptr;
}

ICommand* InputHandler::InputRotateVirtical() {

	Input* input = Input::GetInstance();
	
	if (input->PushKey(DIK_E)) { 
		return pressKeyE_; 
	}
	
	if (input->PushKey(DIK_Q)) { 
		return pressKeyQ_; 
	}
	
	return nullptr;
}
