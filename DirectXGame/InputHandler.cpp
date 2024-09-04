#include "InputHandler.h"
#include "Input.h"

void InputHandler::AssignMoveRightCommand2PressKeyRight() {

	ICommand* command = new MoveRightCommand();

	this->pressKeyRight_ = command;

}

void InputHandler::AssignMoveLeftCommand2PressKeyLeft() {

	ICommand* command = new MoveLeftCommand();

	this->pressKeyLeft_ = command;

}

void InputHandler::AssignMoveUpCommand2PressKeyUp() {

	ICommand* command = new MoveUpCommand();

	this->pressKeyUp_ = command;

}

void InputHandler::AssignMoveDownCommand2PressKeyDown() {

	ICommand* command = new MoveDownCommand();

	this->pressKeyDown_ = command;

}

ICommand* InputHandler::HandleInput() {

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_RIGHT)) {
		return pressKeyRight_;
	}
	if (input->PushKey(DIK_LEFT)) {
		return pressKeyLeft_;
	}
	if (input->PushKey(DIK_UP)) {
		return pressKeyUp_;
	}
	if (input->PushKey(DIK_DOWN)) {
		return pressKeyDown_;
	}

	return nullptr;
}
