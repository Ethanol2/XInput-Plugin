// Ethan Colucci

#include <Windows.h>
#include <iostream>

#include "XInput_Gamepad.h"

// Link the Xinput library - Important!
#pragma comment(lib, "Xinput.lib")


XInput_Gamepad::XInput_Gamepad()
{
	return;
}

XInput_Gamepad::XInput_Gamepad(int index, float *ls_dz, float *rs_dz, float *lt_dz, float *rt_dz)
{
	setRumble(0.f, 0.f);

	// Set gamepad index
	m_iGamepadIndex = index;

	m_LeftStick_DZ = ls_dz;
	m_RightStick_DZ = rs_dz;
	m_LeftTrigger_DZ = lt_dz;
	m_RightTrigger_DZ = rt_dz;

	m_VibrationTimer = 0;

	return;
}

float XInput_Gamepad::LeftStick_X()
{
	// Obtain X axis of left stick
	float l_X = m_State.Gamepad.sThumbLX;
	l_X /= 32768.0f;
	if (abs(l_X) < *m_LeftStick_DZ) { return 0; }

	//Devides the result to return a value from -1 to 1.
	return l_X;
}

float XInput_Gamepad::LeftStick_Y()
{
	// Obtain Y axis of left stick
	float l_Y = m_State.Gamepad.sThumbLY;
	l_Y /= 32768.0f;
	if (abs(l_Y) < *m_LeftStick_DZ) { return 0; }

	//Devides the result to return a value from -1 to 1.
	return l_Y;
}

float XInput_Gamepad::RightStick_X()
{
	// Obtain X axis of left stick
	float l_X = m_State.Gamepad.sThumbRX;
	l_X /= 32768.0f;
	if (abs(l_X) < *m_RightStick_DZ) { return 0; }

	//Devides the result to return a value from -1 to 1.
	return l_X;
}

float XInput_Gamepad::RightStick_Y()
{
	// Obtain Y axis of left stick
	float l_Y = m_State.Gamepad.sThumbRY;
	l_Y /= 32768.0f;
if (abs(l_Y) < *m_RightStick_DZ) { return 0; }

//Devides the result to return a value from -1 to 1.
return l_Y;
}

float XInput_Gamepad::LeftTrigger()
{
	// Obtain left trigger info
	float l_Trigger = m_State.Gamepad.bLeftTrigger;
	l_Trigger /= 255.0f;
	if (l_Trigger < *m_LeftTrigger_DZ) { return 0; }

	return l_Trigger;
}

bool XInput_Gamepad::LeftTriggerAsButton(float a_threshold)
{
	float l_Trigger = m_State.Gamepad.bLeftTrigger;
	l_Trigger /= 255.f;
	if (l_Trigger >= a_threshold) { return true; }
	return false;
}

float XInput_Gamepad::RightTrigger()
{
	// Obtain right trigger info
	float l_Trigger = m_State.Gamepad.bRightTrigger;
	l_Trigger /= 255.0f;
	if (l_Trigger < *m_RightTrigger_DZ) { return 0; }

	return l_Trigger;
}

bool XInput_Gamepad::RightTriggerAsButton(float a_threshold)
{
	float l_Trigger = m_State.Gamepad.bRightTrigger;
	l_Trigger /= 255.f;
	if (l_Trigger >= a_threshold) { return true; }
	return false;
}

bool XInput_Gamepad::GetButtonDown(unsigned int a_iButton)
{
	return (m_State.Gamepad.wButtons & XINPUT_Buttons[a_iButton]) != 0;
}

bool XInput_Gamepad::GetButtonPressed(unsigned int a_iButton)
{
	return (m_State.Gamepad.wButtons & XINPUT_Buttons[a_iButton]) && !m_buttonLastState[a_iButton] ? true : false;
}

bool XInput_Gamepad::GetButtonReleased(unsigned int a_iButton)
{
	return !(m_State.Gamepad.wButtons & XINPUT_Buttons[a_iButton]) && m_buttonLastState[a_iButton] ? true : false;
}

void XInput_Gamepad::setRumble(float leftMotor, float rightMotor)
{
	//Clear the vibration state
	ZeroMemory(&m_VState, sizeof(XINPUT_VIBRATION));

	//Set the vibration intensities. 
	//leftMotor and rightMotor variables are from 0 to 1 and XINPUT uses from 0 to 65535.
	m_VState.wLeftMotorSpeed = leftMotor * 65535;
	m_VState.wRightMotorSpeed = rightMotor * 65535;

	//Update the vibration with XInput
	XInputSetState(m_iGamepadIndex, &m_VState);

	return;
}

void XInput_Gamepad::setRumbleTime(float a_leftMotor, float a_rightMotor, double a_time)
{
	//Clear the vibration state
	ZeroMemory(&m_VState, sizeof(XINPUT_VIBRATION));

	//Set the vibration intensities. 
	//leftMotor and rightMotor variables are from 0 to 1 and XINPUT uses from 0 to 65535.
	m_VState.wLeftMotorSpeed = a_leftMotor * 65535;
	m_VState.wRightMotorSpeed = a_rightMotor * 65535;

	//Update the vibration with XInput
	XInputSetState(m_iGamepadIndex, &m_VState);

	m_VibrationStartTime = std::chrono::high_resolution_clock::now();
	m_VibrationTimer = a_time;
}

int XInput_Gamepad::getIndex()
{
	return m_iGamepadIndex;
}

bool XInput_Gamepad::connected()
{
	if (m_VibrationTimer)
	{
		if (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_VibrationStartTime).count() >= m_VibrationTimer)
		{
			//Clear the vibration state
			ZeroMemory(&m_VState, sizeof(XINPUT_VIBRATION));

			//Set the vibration intensities. 
			//leftMotor and rightMotor variables are from 0 to 1 and XINPUT uses from 0 to 65535.
			m_VState.wLeftMotorSpeed = 0;
			m_VState.wRightMotorSpeed = 0;

			//Update the vibration with XInput
			XInputSetState(m_iGamepadIndex, &m_VState);

			m_VibrationTimer = 0;
		}
	}

	for (int k = 0; k < ButtonCount; k++)
	{
		m_buttonLastState[k] = (m_State.Gamepad.wButtons & XINPUT_Buttons[k]) != 0;
	}

	// Resets State
	ZeroMemory(&m_State, sizeof(XINPUT_STATE));

	// Get the state of the gamepad
	DWORD Result = XInputGetState(m_iGamepadIndex, &m_State);

	if (Result == ERROR_SUCCESS)
		return true;
	else
		return false;
}
