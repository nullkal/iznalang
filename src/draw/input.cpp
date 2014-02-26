#include <GL/glut.h>
#include <cstring>

#include "input.h"

namespace stg {
namespace input {

namespace {

bool state[2][static_cast<int>(SpecialKeys::KEY_COUNT)] = {};

void KeyDown(unsigned char key, int, int)
{
	state[0][key] = true;
}

void KeyUp(unsigned char key, int, int)
{
	state[0][key] = false;
}

void SpecialDown(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		state[0][static_cast<int>(SpecialKeys::UP)] = true;
		break;
	case GLUT_KEY_DOWN:
		state[0][static_cast<int>(SpecialKeys::DOWN)] = true;
		break;
	case GLUT_KEY_LEFT:
		state[0][static_cast<int>(SpecialKeys::LEFT)] = true;
		break;
	case GLUT_KEY_RIGHT:
		state[0][static_cast<int>(SpecialKeys::RIGHT)] = true;
		break;
	}
}

void SpecialUp(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		state[0][static_cast<int>(SpecialKeys::UP)] = false;
		break;
	case GLUT_KEY_DOWN:
		state[0][static_cast<int>(SpecialKeys::DOWN)] = false;
		break;
	case GLUT_KEY_LEFT:
		state[0][static_cast<int>(SpecialKeys::LEFT)] = false;
		break;
	case GLUT_KEY_RIGHT:
		state[0][static_cast<int>(SpecialKeys::RIGHT)] = false;
		break;
	}
}

}// (anonymous)


bool Key::IsPushed()
{
	return state[0][m_key] && !state[1][m_key];
}

bool Key::IsHolded()
{
	return state[0][m_key];
}

bool Key::IsReleased()
{
	return !state[0][m_key] && state[1][m_key];
}


void Init()
{
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

	glutSpecialFunc(SpecialDown);
	glutSpecialUpFunc(SpecialUp);
}

void Update()
{
	memcpy(
		state[1],
		state[0],
		sizeof(bool) * static_cast<int>(SpecialKeys::KEY_COUNT));
}

}// std::input
}// std

