#include <png.h>

#include <vector>
#include <cstdio>
#include <cstring>
#include <csetjmp>
#include <limits>
#include <GL/glut.h>

#include "texture.h"

namespace stg {

TextureBase::TextureBase()
{}

TextureBase::~TextureBase()
{}

RawTexture::RawTexture():
	m_texture(0),
	m_width(0),
	m_height(0)
{}

RawTexture::~RawTexture()
{
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}
}

GLuint RawTexture::GetHandle()
{
	return m_texture;
}

int RawTexture::GetWidth()
{
	return m_width;
}

int RawTexture::GetHeight()
{
	return m_height;
}

double RawTexture::GetCoordLeft()
{
	return 0.0;
}

double RawTexture::GetCoordRight()
{
	return 1.0;
}

double RawTexture::GetCoordBottom()
{
	return 0.0;
}

double RawTexture::GetCoordTop()
{
	return 1.0;
}

void RawTexture::Initialize(
	int width,
	int height,
	GLenum format,
	GLenum type,
	const void *data)
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
		format,
		type,
		data);
	glBindTexture(GL_TEXTURE_2D, 0);

	m_width  = width;
	m_height = height;
}


Texture::Texture():
	m_rt(NULL),
	m_x(0),
	m_y(0),
	m_width(0),
	m_height(0)
{}

Texture::Texture(std::shared_ptr<RawTexture> rt):
	m_rt(rt),
	m_x(0),
	m_y(0),
	m_width(rt? rt->GetWidth() : 0),
	m_height(rt? rt->GetHeight() : 0)
{}

Texture::Texture(
	std::shared_ptr<RawTexture> rt,
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

Texture::Texture(
	std::shared_ptr<Texture> tex,
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

GLuint Texture::GetHandle()
{
	return m_rt->GetHandle();
}

int Texture::GetWidth()
{
	return m_width;
}

int Texture::GetHeight()
{
	return m_height;
}

double Texture::GetCoordLeft()
{
	return static_cast<double>(m_x) / m_rt->GetWidth();
}

double Texture::GetCoordRight()
{
	return static_cast<double>(m_x + m_width) / m_rt->GetWidth();
}

double Texture::GetCoordBottom()
{
	return static_cast<double>(m_y) / m_rt->GetHeight();
}

double Texture::GetCoordTop()
{
	return static_cast<double>(m_y + m_height) / m_rt->GetHeight();
}

std::shared_ptr<RawTexture> Texture::GetRawTexture()
{
	return m_rt;
}

int Texture::GetX()
{
	return m_x;
}

int Texture::GetY()
{
	return m_y;
}

void Texture::SetRawTexture(std::shared_ptr<RawTexture> rt)
{
	m_rt = rt;
}

void Texture::SetX(int x)
{
	m_x = x;
}

void Texture::SetY(int y)
{
	m_y = y;
}

void Texture::SetWidth(int width)
{
	m_width = width;
}

void Texture::SetHeight(int height)
{
	m_height = height;
}


unsigned int NextPow2(unsigned int num)
{
	num = num - 1;
	for (int i = 1; i < std::numeric_limits<unsigned int>::digits; i <<= 1)
	{
		num |= num >> i;
	}
	return num + 1;
}

std::shared_ptr<Texture> LoadPNG(const char *filename)
{
	std::shared_ptr<Texture> retval;

	FILE *fp;
	png_structp png_ptr  = NULL;
	png_infop   info_ptr = NULL;

	{
		std::vector<unsigned char> data;
		std::shared_ptr<RawTexture> rt;

		const int SIG_SIZE = 8;

		fp = fopen(filename, "rb");
		if (!fp)
		{
			retval = nullptr;
			goto RELEASE;
		}

		png_byte sig[SIG_SIZE];
		fread(sig, sizeof(png_byte), 8, fp);
		if (png_sig_cmp(sig, 0, 8))
		{
			retval = nullptr;
			goto RELEASE;
		}

		png_ptr = png_create_read_struct(
				PNG_LIBPNG_VER_STRING,
				NULL,
				NULL,
				NULL);
		if (!png_ptr)
		{
			retval = nullptr;
			goto RELEASE;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			retval = nullptr;
			goto RELEASE;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			retval = nullptr;
			goto RELEASE;
		}

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, SIG_SIZE);

		png_read_png(
			png_ptr,
			info_ptr,
			PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,
			NULL);

		unsigned int  width    = png_get_image_width(png_ptr, info_ptr);
		unsigned int  height   = png_get_image_height(png_ptr, info_ptr);
		unsigned int  rowbytes = png_get_rowbytes(png_ptr, info_ptr);
		unsigned char depth    = png_get_bit_depth(png_ptr, info_ptr) / 8;
		unsigned char channels;
		switch (png_get_color_type(png_ptr, info_ptr))
		{
			case PNG_COLOR_TYPE_GRAY:
				channels = 1;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				channels = 2;
				break;
			case PNG_COLOR_TYPE_RGB:
				channels = 3;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				channels = 4;
				break;
			default:
				fprintf(stderr, "Huh!?\n");
				exit(1);
		}

		unsigned int tex_width    = NextPow2(width);
		unsigned int tex_height   = NextPow2(height);
		unsigned int tex_rowbytes = tex_width * depth * channels;
		data.resize(tex_rowbytes * tex_height);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
		for (unsigned int i = 0; i < height; ++i)
		{
			unsigned char *tex_row = &data[tex_rowbytes * (height - 1 - i)];

			memcpy(tex_row, row_pointers[i], rowbytes);
			memset(tex_row + rowbytes, 0, tex_rowbytes - rowbytes);
		}

		GLenum tex_format;
		GLenum tex_type = (depth == 1)? GL_UNSIGNED_BYTE : GL_UNSIGNED_SHORT;
		switch (channels)
		{
			case 1:
				tex_format = GL_LUMINANCE;
				break;
			case 2:
				tex_format = GL_LUMINANCE_ALPHA;
				break;
			case 3:
				tex_format = GL_RGB;
				break;
			case 4:
				tex_format = GL_RGBA;
				break;
		}

		rt = std::make_shared<RawTexture>();
		rt->Initialize(
			tex_width,
			tex_height,
			tex_format,
			tex_type,
			&data[0]);

		retval = std::make_shared<Texture>(
			rt,
			0,
			0,
			width,
			height);
	}
RELEASE:
	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	}

	if (fp)
	{
		fclose(fp);
	}

	return retval;
}

}// stg
