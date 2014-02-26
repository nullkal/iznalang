#ifndef SHOOTING_RENDER_TARGET_H
#define SHOOTING_RENDER_TARGET_H

#include <memory>
#include <functional>
#include <GL/glut.h>

#include "texture.h"

namespace stg {

class FBOTextureBase: public TextureBase
{
public:
	virtual ~FBOTextureBase();

	virtual void Render(std::function<void()> func) = 0;
	virtual void Render(
		std::function<void()> func,
		int x,
		int y,
		int width,
		int height) = 0;
};

typedef std::shared_ptr<FBOTextureBase> FBOTextureBase_ptr;


class FBOTexture;
class RawFBOTexture: public FBOTextureBase
{
private:
	GLuint m_texture, m_framebuffer, m_dsbuffer;
	int m_width, m_height;

	RawFBOTexture(const RawFBOTexture &);
	RawFBOTexture operator=(const RawFBOTexture &);

public:
	explicit RawFBOTexture();
	~RawFBOTexture();

	virtual GLuint GetHandle();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual double GetCoordLeft();
	virtual double GetCoordRight();
	virtual double GetCoordBottom();
	virtual double GetCoordTop();

	virtual void Render(std::function<void()> func);
	virtual void Render(
		std::function<void()> func,
		int x,
		int y,
		int width,
		int height);

	void Initialize(int width, int height);

	typedef FBOTexture partial_expression;
};

typedef std::shared_ptr<RawFBOTexture> RawFBOTexture_ptr;


class FBOTexture: public TextureBase
{
private:
	std::shared_ptr<RawFBOTexture> m_rt;
	int m_x, m_y, m_width, m_height;

public:
	explicit FBOTexture();
	explicit FBOTexture(std::shared_ptr<RawFBOTexture> rt);
	explicit FBOTexture(
		std::shared_ptr<RawFBOTexture> rt,
		int x,
		int y,
		int width,
		int height);
	explicit FBOTexture(
		std::shared_ptr<FBOTexture> tex,
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

	virtual void Render(std::function<void()> func);
	virtual void Render(
		std::function<void()> func,
		int x,
		int y,
		int width,
		int height);

	std::shared_ptr<RawFBOTexture> GetRawTexture();
	int GetX();
	int GetY();

	void SetRawTexture(std::shared_ptr<RawFBOTexture> rt);
	void SetX(int x);
	void SetY(int y);
	void SetWidth(int w);
	void SetHeight(int h);

	void Initialize(int width, int height);

	typedef FBOTexture partial_expression;
};

typedef std::shared_ptr<FBOTexture> FBOTexture_ptr;

}// stg

#endif//SHOOTING_RENDER_TARGET_H

