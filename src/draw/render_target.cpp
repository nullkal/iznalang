#include <GL/glew.h>
#include <GL/glut.h>

#include "render_target.h"

namespace stg {

FBOTextureBase::~FBOTextureBase()
{}


RawFBOTexture::RawFBOTexture():
	m_texture(0),
	m_framebuffer(0),
	m_dsbuffer(0),
	m_width(0),
	m_height(0)
{}

RawFBOTexture::~RawFBOTexture()
{
	if (m_framebuffer)
	{
		glDeleteFramebuffersEXT(1, &m_framebuffer);
	}

	if (m_dsbuffer)
	{
		glDeleteRenderbuffersEXT(1, &m_dsbuffer);
	}

	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}
}

GLuint RawFBOTexture::GetHandle()
{
	return m_texture;
}

int RawFBOTexture::GetWidth()
{
	return m_width;
}

int RawFBOTexture::GetHeight()
{
	return m_height;
}

double RawFBOTexture::GetCoordLeft()
{
	return 0.0;
}

double RawFBOTexture::GetCoordRight()
{
	return 1.0;
}

double RawFBOTexture::GetCoordBottom()
{
	return 0.0;
}

double RawFBOTexture::GetCoordTop()
{
	return 1.0;
}

void RawFBOTexture::Render(std::function<void()> func)
{
	Render(func, 0, 0, m_width, m_height);
}

void RawFBOTexture::Render(
	std::function<void()> func,
	int x,
	int y,
	int width,
	int height)
{
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(x, y, width, height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_framebuffer);

	func();
	glFlush();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RawFBOTexture::Initialize(int width, int height)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffersEXT(1, &m_dsbuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_dsbuffer);
	glRenderbufferStorageEXT(
		GL_RENDERBUFFER_EXT,
		GL_DEPTH_STENCIL_EXT,
		width,
		height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	glGenFramebuffersEXT(1, &m_framebuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_framebuffer);
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT,
		GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D,
		m_texture,
		0);
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT,
		m_dsbuffer);
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT,
		GL_STENCIL_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT,
		m_dsbuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	m_width  = width;
	m_height = height;
}


FBOTexture::FBOTexture():
	m_rt(NULL),
	m_x(0),
	m_y(0),
	m_width(0),
	m_height(0)
{}

FBOTexture::FBOTexture(std::shared_ptr<RawFBOTexture> rt):
	m_rt(rt),
	m_x(0),
	m_y(0),
	m_width(rt? rt->GetWidth() : 0),
	m_height(rt? rt->GetHeight() : 0)
{}

FBOTexture::FBOTexture(
	std::shared_ptr<RawFBOTexture> rt,
	int x,
	int y,
	int width,
	int height):
	m_rt(rt),
	m_x(x),
	m_y(y),
	m_width(width),
	m_height(height)
{}

FBOTexture::FBOTexture(
	std::shared_ptr<FBOTexture> tex,
	int x,
	int y,
	int width,
	int height):
	m_rt(tex->GetRawTexture()),
	m_x(tex->GetX() + x),
	m_y(tex->GetY() + y),
	m_width(width),
	m_height(height)
{
	int x2 = m_x + m_width;
	int y2 = m_y + m_height;

	if (m_x < tex->GetX())
	{
		m_x = tex->GetX();
	}
	else if (tex->GetX() + tex->GetWidth() < m_x)
	{
		m_x = tex->GetX() + tex->GetWidth();
	}

	if (x2 < tex->GetX())
	{
		x2 = tex->GetX();
	}
	else if (tex->GetX() + tex->GetWidth() < x2)
	{
		x2 = tex->GetX() + tex->GetWidth();
	}

	if (m_y < tex->GetY())
	{
		m_y = tex->GetY();
	}
	else if (tex->GetY() + tex->GetHeight() < m_y)
	{
		m_y = tex->GetY() + tex->GetHeight();
	}

	if (y2 < tex->GetY())
	{
		y2 = tex->GetY();
	}
	else if (tex->GetY() + tex->GetHeight() < y2)
	{
		y2 = tex->GetY() + tex->GetHeight();
	}

	m_width  = x2 - m_x;
	m_height = y2 - m_y;
}

GLuint FBOTexture::GetHandle()
{
	return m_rt->GetHandle();
}

int FBOTexture::GetWidth()
{
	return m_width;
}

int FBOTexture::GetHeight()
{
	return m_height;
}

double FBOTexture::GetCoordLeft()
{
	return static_cast<double>(m_x) / m_rt->GetWidth();
}

double FBOTexture::GetCoordRight()
{
	return static_cast<double>(m_x + m_width) / m_rt->GetWidth();
}

double FBOTexture::GetCoordBottom()
{
	return static_cast<double>(m_y) / m_rt->GetHeight();
}

double FBOTexture::GetCoordTop()
{
	return static_cast<double>(m_y + m_height) / m_rt->GetHeight();
}

void FBOTexture::Render(std::function<void()> func)
{
	m_rt->Render(func, m_x, m_y, m_width, m_height);
}

void FBOTexture::Render(
	std::function<void()> func,
	int x,
	int y,
	int width,
	int height)
{
	x += m_x;
	y += m_y;
	int x2 = x + width;
	int y2 = y + height;

	if (x < m_x)
	{
		x = m_x;
	}
	else if (m_x + m_width < x)
	{
		x = m_x + m_width;
	}

	if (x2 < m_x)
	{
		x2 = m_x;
	}
	else if (m_x + m_width < x2)
	{
		x2 = m_x + m_width;
	}

	if (y < m_y)
	{
		y = m_y;
	}
	else if (m_y + m_height < y)
	{
		y = m_y + m_height;
	}

	if (y2 < m_y)
	{
		y2 = m_y;
	} else if (m_y + m_height < y2)
	{
		y2 = m_y + m_height;
	}

	m_rt->Render(func, x, y, x2 - x, y2 - y);
}

std::shared_ptr<RawFBOTexture> FBOTexture::GetRawTexture()
{
	return m_rt;
}

int FBOTexture::GetX()
{
	return m_x;
}

int FBOTexture::GetY()
{
	return m_y;
}

void FBOTexture::SetRawTexture(std::shared_ptr<RawFBOTexture> rt)
{
	m_rt = rt;
}

void FBOTexture::SetX(int x)
{
	m_x = x;
}

void FBOTexture::SetY(int y)
{
	m_y = y;
}

void FBOTexture::SetWidth(int width)
{
	m_width = width;
}

void FBOTexture::SetHeight(int height)
{
	m_height = height;
}

void FBOTexture::Initialize(int width, int height)
{
	m_rt = std::make_shared<RawFBOTexture>();
	m_x = 0;
	m_y = 0;
	m_width  = width;
	m_height = height;

	m_rt->Initialize(NextPow2(width), NextPow2(height));
}

}// stg

