#ifndef SHOOTING_DRAW2D_H
#define SHOOTING_DRAW2D_H

#include <functional>

#include "texture.h"

namespace stg {

enum class BlendMode { NORMAL, ADD, MULTIPLY, SCREEN };

class Drawer2D
{
private:
	TextureBase_ptr m_tex;

	int m_position_x, m_position_y;

	double m_rotation;
	int m_origin_x, m_origin_y;

	bool m_flip_x, m_flip_y;
	double m_scale_x, m_scale_y;

	unsigned char m_color_r, m_color_g, m_color_b, m_color_a;
	BlendMode m_blendmode;

	bool m_circle_gauge_enabled;
	double m_start_deg, m_end_deg;

public:
	explicit Drawer2D();
	explicit Drawer2D(TextureBase_ptr tex);
	explicit Drawer2D(TextureBase_ptr tex, int pos_x, int pos_y);

	Drawer2D Apply() const;

	Drawer2D SetTexture(TextureBase_ptr tex) const;

	Drawer2D SetPosition(int x, int y) const;

	Drawer2D SetOrigin(int x, int y) const;
	Drawer2D Rotate(double deg) const;

	Drawer2D SetFlipX(bool flip = true) const;
	Drawer2D SetFlipY(bool flip = true) const;

	Drawer2D SetScale(double scale) const;
	Drawer2D SetScale(double x, double y) const;

	Drawer2D SetColor(
		unsigned char r,
		unsigned char g,
		unsigned char b,
		unsigned char a = 255) const;
	Drawer2D SetBlendMode(BlendMode mode) const;
	
	Drawer2D CircleGauge(bool enable, double start_deg = 0.0, double end_deg = 0.0) const;
};

void Draw2D(std::function<void()> draw_func);

}// stg

#include "draw2d-inl.h"

#endif//SHOOTING_DRAW2D_H

