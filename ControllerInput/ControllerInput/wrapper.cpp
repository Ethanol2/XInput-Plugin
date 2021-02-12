// Ethan Colucci

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <chrono>
#include "wrapper.h"
#include "XInput_Gamepad.h"

using namespace std;

struct Custom_Command
{
	char * m_Name;
	unsigned int m_Input = 0;
	bool m_Type = false;
	bool m_Axis = 0;
	float m_AxisThreshold = 1;

	Custom_Command operator=(unsigned int &a_Value)
	{
		this->m_Input = a_Value;
		return *this;
	}
	Custom_Command operator=(char * a_Value)
	{
		this->m_Name = a_Value;
		return *this;
	}
};

struct Input_Profile
{
	Input_Profile()
	{
		m_Name = "N/A";
	}
	Input_Profile(char * a_Name)
	{
		m_Name = a_Name;
	}
	char * m_Name;
	std::map<char *, Custom_Command> m_CustomInput;

	float m_LeftStick_DZ = 0.1f;
	float m_RightStick_DZ = 0.1f;
	float m_LeftTrigger_DZ = 0.1f;
	float m_RightTrigger_DZ = 0.1f;
};

struct Player
{
	Player(int pNum, Input_Profile * a_Profile) : PLAYER_NUMBER(pNum)
	{
		m_profile = a_Profile;
	}
	void InitGamePad()
	{
		delete m_control;
		m_control = new XInput_Gamepad(
			PLAYER_NUMBER,
			&m_profile->m_LeftStick_DZ,
			&m_profile->m_RightStick_DZ,
			&m_profile->m_LeftTrigger_DZ,
			&m_profile->m_RightTrigger_DZ
		);
	}
	bool getCommandDown(char * a_Name)
	{
		return m_control->GetButtonDown(m_profile->m_CustomInput[a_Name].m_Input);
	}

	XInput_Gamepad * m_control;
	Input_Profile * m_profile;
	const int PLAYER_NUMBER;
};

Input_Profile g_DefaultProfile("Default");
std::map<char *, Input_Profile> g_Profiles;

Player g_Players[4] = {
	Player(0, &g_DefaultProfile),
	Player(1, &g_DefaultProfile),
	Player(2, &g_DefaultProfile),
	Player(3, &g_DefaultProfile)
};


void InitGamepads()
{

	g_Players[0].InitGamePad();
	g_Players[1].InitGamePad();
	g_Players[2].InitGamePad();
	g_Players[3].InitGamePad();

	return;
}

bool CheckGamepadConnected(int gamepadNum)
{
	return g_Players[gamepadNum].m_control->connected();
}

bool GetControllerKeyDown(int a_GamepadNum, int a_ButtonID)
{
	return  g_Players[a_GamepadNum].m_control->GetButtonDown(a_ButtonID);
}

bool GetControllerKeyPressed(int a_GamepadNum, int a_ButtonID)
{
	return g_Players[a_GamepadNum].m_control->GetButtonPressed(a_ButtonID);
}

bool GetControllerKeyReleased(int a_GamepadNum, int a_ButtonID)
{
	return g_Players[a_GamepadNum].m_control->GetButtonReleased(a_ButtonID);
}

bool GetCommandDown(int a_Player, char * a_Command)
{
	try { g_DefaultProfile.m_CustomInput.at(a_Command); }
	catch (std::out_of_range)
	{
		return false;
	}

	if (!g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Type)
	{
		return g_Players->m_control->GetButtonDown(g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Input);
	}

	if (!g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Axis)
	{
		return g_Players[a_Player].m_control->LeftTriggerAsButton(
			g_Players->m_profile->m_CustomInput[a_Command].m_AxisThreshold
		);
	}

	return g_Players[a_Player].m_control->RightTriggerAsButton(
		g_Players->m_profile->m_CustomInput[a_Command].m_AxisThreshold
	);

}

bool GetCommandPressed(int a_Player, char * a_Command)
{
	try { g_DefaultProfile.m_CustomInput.at(a_Command); }
	catch (std::out_of_range)
	{
		return false;
	}

	if (!g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Type)
	{
		return g_Players->m_control->GetButtonPressed(g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Input);
	}

	return false;
}

bool GetCommandReleased(int a_Player, char * a_Command)
{

	try { g_DefaultProfile.m_CustomInput.at(a_Command); }
	catch (std::out_of_range)
	{
		return false;
	}

	if (!g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Type)
	{
		return g_Players->m_control->GetButtonReleased(g_Players[a_Player].m_profile->m_CustomInput[a_Command].m_Input);
	}

	return false;
}

gamepadAxes GetControllerAxes(int gamepadNum)
{
	gamepadAxes l_temp;
	l_temp.l_ThumbStick_X = g_Players[gamepadNum].m_control->LeftStick_X();
	l_temp.l_ThumbStick_Y = g_Players[gamepadNum].m_control->LeftStick_Y();

	l_temp.r_ThumbStick_X = g_Players[gamepadNum].m_control->RightStick_X();
	l_temp.r_ThumbStick_Y = g_Players[gamepadNum].m_control->RightStick_Y();

	l_temp.l_Trigger = g_Players[gamepadNum].m_control->LeftTrigger();
	l_temp.r_Trigger = g_Players[gamepadNum].m_control->RightTrigger();

	return l_temp;
}

void SetControllerRumble(int gamepadNum, float leftMotor, float rightMotor)
{
	g_Players[gamepadNum].m_control->setRumble(leftMotor, rightMotor);
}

void SetControllerRumbleTime(int gamepadNum, float leftMotor, float rightMotor, double time)
{
	g_Players[gamepadNum].m_control->setRumbleTime(leftMotor, rightMotor, time);
}

void CreateCommand(char * a_Name, unsigned int a_defaultButton)
{
	g_DefaultProfile.m_CustomInput[a_Name] = a_defaultButton;
	g_DefaultProfile.m_CustomInput[a_Name] = a_Name;

	for (auto const &k : g_Profiles)
	{
		g_Profiles[k.first].m_CustomInput[a_Name] = a_defaultButton;
		g_Profiles[k.first].m_CustomInput[a_Name] = a_Name;
	}
}

void CreateTriggerCommand(char * a_Name, bool a_defaultAxis, float a_defaultThreshold)
{
	g_DefaultProfile.m_CustomInput[a_Name] = a_Name;

	g_DefaultProfile.m_CustomInput[a_Name].m_Type = true;
	g_DefaultProfile.m_CustomInput[a_Name].m_Axis = a_defaultAxis;
	g_DefaultProfile.m_CustomInput[a_Name].m_AxisThreshold = a_defaultThreshold;

	for (auto const &k : g_Profiles)
	{
		g_Profiles[k.first].m_CustomInput[a_Name] = g_DefaultProfile.m_CustomInput[a_Name];
	}
}

void DeleteCommand(char * a_Name)
{
	g_DefaultProfile.m_CustomInput.erase(a_Name);

	for (auto const &k : g_Profiles)
	{
		g_Profiles[k.first].m_CustomInput.erase(a_Name);
	}

	return;
}

void CreateNewInputProfile(char * a_Name)
{
	g_Profiles[a_Name] = g_DefaultProfile;
	g_Profiles[a_Name].m_Name = a_Name;
	return;
}

bool CreateNewCopiedInputProfile(char * a_CopyFrom, char * a_Name)
{
	try { g_Profiles.at(a_CopyFrom); }
	catch (std::out_of_range)
	{
		g_Profiles[a_Name] = g_DefaultProfile;
		g_Profiles[a_Name].m_Name = a_Name;
		return false;
	}
	g_Profiles[a_Name] = g_Profiles[a_CopyFrom];
	g_Profiles[a_Name].m_Name = a_Name;
	return true;
}

void DeleteInputProfile(char * a_Name)
{
	g_Profiles.erase(a_Name);
	return;
}

bool RenameInputProfile(char * a_Name, char * a_NewName)
{
	try { g_Profiles.at(a_Name); }
	catch (std::out_of_range)
	{
		return false;
	}
	g_Profiles[a_NewName] = g_Profiles[a_Name];
	g_Profiles[a_NewName].m_Name = a_NewName;
	g_Profiles.erase(a_Name);
	return true;
}

bool SetInputProfileBinding(char * a_CommandName, char * a_ProfileName, unsigned int a_NewBinding)
{
	try {
		g_Profiles.at(a_ProfileName);
		g_DefaultProfile.m_CustomInput.at(a_CommandName);
	}
	catch (std::out_of_range)
	{
		return false;
	}
	g_Profiles[a_ProfileName].m_CustomInput[a_CommandName].m_Type = false;
	g_Profiles[a_ProfileName].m_CustomInput[a_CommandName] = a_NewBinding;
	return true;
}

bool SetTriggerAsButton(char * a_CommandName, char * a_ProfileName, bool a_Trigger, float a_threshhold)
{
	try {
		g_Profiles.at(a_ProfileName);
		g_DefaultProfile.m_CustomInput.at(a_CommandName);
	}
	catch (std::out_of_range)
	{
		return false;
	}

	g_Profiles[a_ProfileName].m_CustomInput[a_CommandName].m_Type = true;
	g_Profiles[a_ProfileName].m_CustomInput[a_CommandName].m_Axis = a_Trigger;
	g_Profiles[a_ProfileName].m_CustomInput[a_CommandName].m_AxisThreshold = a_threshhold;

	return true;
}

void SetInputProfileDeadzones(char * a_ProfileName, float a_ls, float a_rs, float a_lt, float a_rt)
{
	try { g_Profiles.at(a_ProfileName); }
	catch (std::out_of_range)
	{
		return;
	}

	g_Profiles[a_ProfileName].m_LeftStick_DZ = a_ls;
	g_Profiles[a_ProfileName].m_RightStick_DZ = a_rs;
	g_Profiles[a_ProfileName].m_LeftTrigger_DZ = a_lt;
	g_Profiles[a_ProfileName].m_RightStick_DZ = a_rt;

	return;
}

bool SaveInputProfilesToDisk(char* a_FilePath)
{
	std::ofstream l_Save;
	l_Save.open(a_FilePath, std::ofstream::out);
	if (!l_Save.is_open())
	{
		return false;
	}

	l_Save << g_DefaultProfile.m_CustomInput.size() << std::endl;

	for (const auto &k : g_DefaultProfile.m_CustomInput)
	{
		l_Save << g_DefaultProfile.m_CustomInput[k.first].m_Name << " "
			<< g_DefaultProfile.m_CustomInput[k.first].m_Input << " "
			<< g_DefaultProfile.m_CustomInput[k.first].m_Type << " "
			<< g_DefaultProfile.m_CustomInput[k.first].m_Axis << " "
			<< g_DefaultProfile.m_CustomInput[k.first].m_AxisThreshold << std::endl;
	}

	l_Save << g_Profiles.size() << std::endl;

	for (const auto &k : g_Profiles)
	{
		l_Save << g_Profiles[k.first].m_Name << " "
			<< g_Profiles[k.first].m_LeftStick_DZ << " "
			<< g_Profiles[k.first].m_LeftTrigger_DZ << " "
			<< g_Profiles[k.first].m_RightStick_DZ << " "
			<< g_Profiles[k.first].m_RightTrigger_DZ;

		for (const auto &l : g_Profiles[k.first].m_CustomInput)
		{
			l_Save << " " << g_Profiles[k.first].m_CustomInput[l.first].m_Input << " "
				<< g_Profiles[k.first].m_CustomInput[l.first].m_Type << " "
				<< g_Profiles[k.first].m_CustomInput[l.first].m_Axis << " "
				<< g_Profiles[k.first].m_CustomInput[l.first].m_AxisThreshold;
		}
		l_Save << std::endl;
	}

	l_Save.close();
}

bool LoadInputProfiles(char * a_FilePath)
{
	//std::ifstream l_Load;
	//l_Load.open(a_FilePath, std::ifstream::in);
	//if (!l_Load.is_open())
	{
		return false;
	}
	//
	//std::string l_line;
	//char l_identifier;
	//char * l_Name;
	//
	//int l_NumOfCommands = 0;
	//unsigned int l_Input = 0;
	//bool l_Type = false;
	//bool l_Axis = false;
	//float l_AxisThreshold = 0.1f;
	//
	//std::getline(l_Load, l_line);
	//sscanf_s(l_line.c_str(), "%d", &l_NumOfCommands);
	//
	//for (int k = 0; k < l_NumOfCommands; k++)
	//{
	//	std::getline(l_Load, l_line);
	//	sscanf_s(l_line.c_str(), "%s %d %i %i %f", &l_Name, &l_Input, &l_Type, &l_Axis, &l_AxisThreshold);
	//
	//	g_DefaultProfile.m_CustomInput[l_Name].m_Input = l_Input;
	//	g_DefaultProfile.m_CustomInput[l_Name].m_Type = l_Type;
	//	g_DefaultProfile.m_CustomInput[l_Name].m_Axis = l_Axis;
	//	g_DefaultProfile.m_CustomInput[l_Name].m_AxisThreshold = l_AxisThreshold;
	//}
	//
	//int l_NumOfProfiles = 0;
	//
	//std::getline(l_Load, l_line);
	//sscanf_s(l_line.c_str(), "%d", &l_NumOfProfiles);
	//
	//for (int k = 0; k < l_NumOfProfiles; k++)
	//{
	//	std::getline(l_Load, l_line);
	//	std::istringstream l_Word(l_line);
	//	l_Word >> l_Name;
	//	for (int l = 0; l_line[l] != '\n'; l++)
	{

	}
	//}
}