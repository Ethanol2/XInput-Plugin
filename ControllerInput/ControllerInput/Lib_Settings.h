// Drew Terbrack - 100548194
// Ethan Colucci - 100583455
// Justin Kan - 100485922
#pragma once

#define CONTROLLER_INPUT

#ifdef CONTROLLER_INPUT
#define LIB_API __declspec(dllexport)
#elif CONTROLLER_INPUT
#define LIB_API __declspec(dllimport)
#else
#define LIB_API
#endif*/
