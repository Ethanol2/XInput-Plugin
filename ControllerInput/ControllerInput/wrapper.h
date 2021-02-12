// Ethan Colucci

#pragma once
#include "Lib_Settings.h"

#ifdef __cplusplus
extern "C"
{
#endif
	__declspec(dllexport) struct gamepadAxes
	{
	public:
		float l_ThumbStick_X;
		float l_ThumbStick_Y;

		float r_ThumbStick_X;
		float r_ThumbStick_Y;

		float l_Trigger;
		float r_Trigger;
	};

	// Sets gamepad IDs (0 to 3)
	LIB_API void InitGamepads();

	// Checks gamepad state and updates input
	LIB_API bool CheckGamepadConnected(int a_GamepadNum);

	// Returns true if the button is pressed
	LIB_API bool GetControllerKeyDown(int a_GamepadNum,int a_ButtonID);

	// Returns true if button was pressed after last update
	LIB_API bool GetControllerKeyPressed(int a_GamepadNum, int a_ButtonID);

	// Returns true if button was released after the last update
	LIB_API bool GetControllerKeyReleased(int a_GamepadNum, int a_ButtonID);

	// Returns true if button is pressed
	LIB_API bool GetCommandDown(int a_Player, char * a_Command);

	// Returns true if command was pressed after last update
	LIB_API bool GetCommandPressed(int a_Player, char * a_Command);

	// Returns true if command was released after last update
	LIB_API bool GetCommandReleased(int a_Player, char * a_Command);

	// Returns struct with axes' values
	LIB_API gamepadAxes GetControllerAxes(int a_GamepadNum);

	// Sets rumble (0 to 1)
	LIB_API void SetControllerRumble(int a_GamepadNum, float a_LeftMotor, float a_RightMotor);

	// Sets rumble with time (0 to 1 intensity, time in seconds)
	LIB_API void SetControllerRumbleTime(int gamepadNum, float leftMotor, float rightMotor, double time);

	// Create an input call with a special name
	LIB_API void CreateCommand(char * a_Name, unsigned int a_defaultButton);

	LIB_API void CreateTriggerCommand(char * a_Name, bool a_defaultAxis, float a_defaultThreshold);

	LIB_API void DeleteCommand(char * a_Name);

	LIB_API void CreateNewInputProfile(char * a_Name);

	LIB_API bool CreateNewCopiedInputProfile(char * a_CopyFrom, char * a_Name);

	LIB_API void DeleteInputProfile(char * a_Name);

	LIB_API bool RenameInputProfile(char * a_Name, char * a_NewName);

	LIB_API bool SetInputProfileBinding(char * a_CommandName, char * a_ProfileName, unsigned int a_NewBinding);

	LIB_API bool SetTriggerAsButton(char * a_CommandName, char * a_ProfileName, bool a_Trigger, float a_threshhold);

	LIB_API void SetInputProfileDeadzones(char * a_ProfileName, float a_ls, float a_rs, float a_lt, float a_rt);

	LIB_API bool SaveInputProfilesToDisk(char* a_FilePath);

	LIB_API bool LoadInputProfiles(char * a_FilePath);

#ifdef __cplusplus
}
#endif