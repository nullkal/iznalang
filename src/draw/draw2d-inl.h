#ifndef SHOOTING_DRAW2D_INL_H
#define SHOOTING_DRAW2D_INL_H

#include <cmath>

namespace stg {

inline Drawer2D::Drawer2D():
	m_tex(NULL),
	m_position_x(0),
	m_position_y(0),
	m_rotation(0.0),
	m_origin_x(0),
	m_origin_y(0),
	m_flip_x(false),
	m_flip_y(false),
	m_scale_x(1.0),
	m_scale_y(1.0),
	m_color_r(255),
	m_color_g(255),
	m_color_b(255),
	m_color_a(255),
	m_blendmode(BlendMode::NORMAL),
	m_circle_gauge_enabled(false),
	m_start_deg(0.0),
	m_end_deg(0.0)
{}

inline Drawer2D::Drawer2D(TextureBase_ptr tex):
	m_tex(tex),
	m_position_x(0),
	m_position_y(0),
	m_rotation(0.0),
	m_origin_x(0),
	m_origin_y(0),
	m_flip_x(false),
	m_flip_y(false),
	m_scale_x(1.0),
	m_scale_y(1.0),
	m_color_r(255),
	m_color_g(255),
	m_color_b(255),
	m_color_a(255),
	m_blendmode(BlendMode::NORMAL),
	m_circle_gauge_enabled(false),
	m_start_deg(0.0),
	m_end_deg(0.0)
{}

inline Drawer2D::Drawer2D(TextureBase_ptr tex, int pos_x, int pos_y):
	m_tex(tex),
	m_position_x(pos_x),
	m_position_y(pos_y),
	m_rotation(0.0),
	m_origin_x(0),
	m_origin_y(0),
	m_flip_x(false),
	m_flip_y(false),
	m_scale_x(1.0),
	m_scale_y(1.0),
	m_color_r(255),
	m_color_g(255),
	m_color_b(255),
	m_color_a(255),
	m_blendmode(BlendMode::NORMAL),
	m_circle_gauge_enabled(false),
	m_start_deg(0.0),
	m_end_deg(0.0)
{}

inline Drawer2D Drawer2D::SetTexture(TextureBase_ptr tex) const
{
	Drawer2D new_stat(*this);
	new_stat.m_tex = tex;

	return new_stat;
}

inline Drawer2D Drawer2D::SetPosition(int x, int y) const
{
	Drawer2D new_stat(*this);
	new_stat.m_position_x = x;
	new_stat.m_position_y = y;

	return new_stat;
}

inline Drawer2D Drawer2D::SetOrigin(int x, int y) const
{
	Drawer2D new_stat(*this);
	new_stat.m_origin_x = x;
	new_stat.m_origin_y = y;

	return new_stat;
}

inline Drawer2D Drawer2D::Rotate(double deg) const
{
	Drawer2D new_stat(*this);
	new_stat.m_rotation = deg;

	return new_stat;
}

inline Drawer2D Drawer2D::SetFlipX(bool flip) const
{
	Drawer2D new_stat(*this);
	new_stat.m_flip_x = flip;

	return new_stat;
}

inline Drawer2D Drawer2D::SetFlipY(bool flip) const
{
	Drawer2D new_stat(*this);
	new_stat.m_flip_y = flip;

	return new_stat;
}

inline Drawer2D Drawer2D::SetScale(double scale) const
{
	Drawer2D new_stat(*this);
	new_stat.m_scale_x = scale;
	new_stat.m_scale_y = scale;
	
	return new_stat;
}

inline Drawer2D Drawer2D::SetScale(double x, double y) const
{
	Drawer2D new_stat(*this);
	new_stat.m_scale_x = x;
	new_stat.m_scale_y = y;
	
	return new_stat;
}

inline Drawer2D Drawer2D::SetColor(
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a) const
{
	Drawer2D new_stat(*this);
	new_stat.m_color_r = r;
	new_stat.m_color_g = g;
	new_stat.m_color_b = b;
	new_stat.m_color_a = a;

	return new_stat;
}

inline Drawer2D Drawer2D::SetBlendMode(BlendMode mode) const
{
	Drawer2D new_stat(*this);
	new_stat.m_blendmode = mode;

	return new_stat;
}

inline Drawer2D Drawer2D::CircleGauge(bool enabled, double start_deg, double end_deg) const
{
	Drawer2D new_stat(*this);
	new_stat.m_circle_gauge_enabled = enabled;
	new_stat.m_start_deg = start_deg;
	new_stat.m_end_deg   = end_deg;

	return new_stat;
}

}

#endif//SHOOTING_INPUT_INL_H

