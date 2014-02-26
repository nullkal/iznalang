#ifndef SHOOTING_INPUT_H
#define SHOOTING_INPUT_H

namespace stg {
namespace input {

enum class SpecialKeys: int { ESC = 27, UP  = 256, DOWN, LEFT, RIGHT, KEY_COUNT };

class Key
{
private:
	int m_key;

public:
	explicit Key(int key);

	static Key Up();
	static Key Down();
	static Key Left();
	static Key Right();
	static Key Esc();

	bool IsPushed();
	bool IsHolded();
	bool IsReleased();
};

void Init();
void Update();

}// std::input
}// stg

#include "input-inl.h"

#endif//SHOOTING_INPUT_H

