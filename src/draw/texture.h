#ifndef SHOOTING_TEXTURE_H
#define SHOOTING_TEXTURE_H

#include <memory>
#include <vector>
#include <GL/glut.h>

namespace stg {

class TextureBase
{
public:
	explicit TextureBase();
	virtual ~TextureBase();

	virtual GLuint GetHandle() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual double GetCoordLeft() = 0;
	virtual double GetCoordRight() = 0;
	virtual double GetCoordBottom() = 0;
	virtual double GetCoordTop() = 0;
};

typedef std::shared_ptr<TextureBase> TextureBase_ptr;


class Texture;
class RawTexture: public TextureBase
{
private:
	GLuint m_texture;
	int m_width, m_height;

	RawTexture(const RawTexture &);
	RawTexture operator=(const RawTexture &);

public:
	explicit RawTexture();
	~RawTexture();

	virtual GLuint GetHandle();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual double GetCoordLeft();
	virtual double GetCoordRight();
	virtual double GetCoordBottom();
	virtual double GetCoordTop();

	void Initialize(
		int width,
		int height,
		GLenum format,
		GLenum type,
		const void *data);

	typedef Texture partial_expression;
};

typedef std::shared_ptr<RawTexture> RawTexture_ptr;


class Texture: public TextureBase
{
private:
	std::shared_ptr<RawTexture> m_rt;
	int m_x, m_y, m_width, m_height;

public:
	explicit Texture();
	explicit Texture(std::shared_ptr<RawTexture> rt);
	explicit Texture(
		std::shared_ptr<RawTexture> rt,
		int x,
		int y,
		int width,
		int height);
	explicit Texture(
		std::shared_ptr<Texture> tex,
		int x,
		int y,
		int width,
		int height);

	virtual GLuint GetHandle();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual double GetCoordLeft();
	virtual double GetCoordRight();
	virtual double GetCoordBottom();
	virtual double GetCoordTop();

	std::shared_ptr<RawTexture> GetRawTexture();
	int GetX();
	int GetY();

	void SetRawTexture(std::shared_ptr<RawTexture> rt);
	void SetX(int x);
	void SetY(int y);
	void SetWidth(int w);
	void SetHeight(int h);

	typedef Texture partial_expression;
};

typedef std::shared_ptr<Texture> Texture_ptr;


unsigned int NextPow2(unsigned int num);

Texture_ptr LoadPNG(const char *filename);

template <typename T>
std::vector<std::shared_ptr<typename T::partial_expression>> SplitTexture(
	std::shared_ptr<T> tex,
	int num_x,
	int num_y);

template <typename T>
std::vector<std::shared_ptr<typename T::partial_expression>> SplitTexture(
	std::shared_ptr<T> tex,
	int num_x,
	int num_y,
	int partial_width,
	int partial_height);

}// stg

#include "texture-inl.h"

#endif//SHOOTING_TEXTURE_H
