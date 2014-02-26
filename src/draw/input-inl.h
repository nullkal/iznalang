#ifndef SHOOTING_INPUT_INL_H
#define SHOOTING_INPUT_INL_H

namespace stg {
namespace input {

inline Key::Key(int key):
	m_key(key)
{}

inline Key Key::Up()
{
	return Key(static_cast<int>(SpecialKeys::UP));
}

inline Key Key::Down()
{
	return Key(static_cast<int>(SpecialKeys::DOWN));
}

inline Key Key::Left()
{
	return Key(static_cast<int>(SpecialKeys::LEFT));
}

inline Key Key::Right()
{
	return Key(static_cast<int>(SpecialKeys::RIGHT));
}

inline Key Key::Esc()
{
	return Key(static_cast<int>(SpecialKeys::ESC));
}

}// std::input
}// std

#endif//SHOOTING_INPUT_INL_H

