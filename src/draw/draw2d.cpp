#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>

#include "draw2d.h"

namespace stg {

namespace {

GLuint g_vertexBuffer;

}

Drawer2D Drawer2D::Apply() const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glTranslated(m_position_x, m_position_y, 0);
	glRotated(m_rotation, 0, 0, -1);
	glScaled(m_scale_x * (m_flip_x ? -1 : 1), m_scale_y * (m_flip_y ? -1 : 1), 1.0);
	glTranslated(-m_origin_x, -m_origin_y, 0);
	glScaled(m_tex->GetWidth(), m_tex->GetHeight(), 1.0);

	if (m_circle_gauge_enabled)
	{
		double start_deg = m_start_deg;
		double end_deg   = m_end_deg;

		end_deg = fmod((end_deg - start_deg) + 360.0, 720.0) - 360.0;
		start_deg -= floor(start_deg / 360.0) * 360.0;
		end_deg += start_deg;
		if (start_deg > end_deg)
		{
			std::swap(start_deg, end_deg);
		}

		double fill_r = sqrt(pow(m_tex->GetWidth() / 2, 2) * pow(m_tex->GetHeight() / 2, 2)) * (2 / sqrt(3));

		glEnable(GL_STENCIL_TEST);

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 1, ~0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glDepthMask(0);
		glColorMask(0, 0, 0, 0);

		double center_x = m_tex->GetWidth()  / 2.0;
		double center_y = m_tex->GetHeight() / 2.0;

		glBegin(GL_TRIANGLE_FAN);
			glVertex2d(center_x, center_y);

			glVertex2d(
				center_x + fill_r * cos(start_deg * M_PI / 180.0),
				center_y + fill_r * sin(start_deg * M_PI / 180.0));
			for (double ideg = start_deg + 60; ideg < end_deg; ideg += 60)
			{
				glVertex2d(
					center_x + fill_r * cos(ideg * M_PI / 180.0),
					center_y + fill_r * sin(ideg * M_PI / 180.0));
			}
			glVertex2d(
				center_x + fill_r * cos(end_deg * M_PI / 180.0),
				center_y + fill_r * sin(end_deg * M_PI / 180.0));
		glEnd();

		glStencilFunc(GL_EQUAL, 1, ~0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDepthMask(1);
		glColorMask(1, 1, 1, 1);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, m_tex->GetHandle());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	switch (m_blendmode)
	{
	case BlendMode::NORMAL:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case BlendMode::ADD:
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	case BlendMode::MULTIPLY:
		glBlendFunc(GL_ZERO, GL_SRC_COLOR);
		break;
	case BlendMode::SCREEN:
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
		break;
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
	glVertexPointer(2, GL_INT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_tex->GetTexCoordBuffer());
	glTexCoordPointer(2, GL_DOUBLE, 0, 0);

	glColor4ub(m_color_r, m_color_g, m_color_b, m_color_a);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (m_circle_gauge_enabled)
	{
		glDisable(GL_STENCIL_TEST);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	return *this;
}

void Draw2D(std::function<void()> draw_func)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, 0, 1);

	draw_func();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void InitDraw2D()
{
	glGenBuffers(1, &g_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);

	GLint vertices[4][2] = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLint), vertices, GL_STATIC_DRAW);
}

}// stg
