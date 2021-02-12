// Ethan Colucci

#pragma once
#include <Xinput.h>
#include <chrono>

// This XInput class was inspired by this tutorial
// https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/

// XInput Button values
static const WORD XINPUT_Buttons[] = {
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK
};


class XInput_Gamepad
{
public:
	XInput_Gamepad();						// Default constructor is empty
	XInput_Gamepad(
		int index, 
		float *ls_dz, 
		float *rs_dz, 
		float *lt_dz, 
		float *rt_dz);						// Index is required to distuigish gamepads

	// Thumbstick Functions
	float LeftStick_X();
	float LeftStick_Y();
	float RightStick_X();
	float RightStick_Y();

	// Trigger functions
	float LeftTrigger();
	bool LeftTriggerAsButton(float a_threshold);
	float RightTrigger();
	bool RightTriggerAsButton(float a_threshold);
	

	// Check input
	bool GetButtonDown(unsigned int a_iButton);	// Function takes unsigned int to avoid negative requests
	bool GetButtonPressed(unsigned int a_iButton);
	bool GetButtonReleased(unsigned int a_iButton);

	void setRumble(float leftMotor, float rightMotor);
	void setRumbleTime(float a_leftMotor, float a_rightMotor, double a_time);

	int getIndex();							// Return gamepad index
	bool connected();						// Return true if gamepad is connected

private:

	XINPUT_STATE m_State;					// Current gamepad state
	XINPUT_VIBRATION m_VState;				// Current vibration state.
	int m_iGamepadIndex;					// Gamepad index

	static const int ButtonCount = 14;		// Total buttons on the gamepad
	bool m_buttonLastState[ButtonCount];		// Previous button states
	std::chrono::steady_clock::time_point 
		m_VibrationStartTime;					// Timer for the vibration
	double m_VibrationTimer;

	float * m_LeftStick_DZ;
	float * m_RightStick_DZ;
	float * m_LeftTrigger_DZ;
	float * m_RightTrigger_DZ;
};