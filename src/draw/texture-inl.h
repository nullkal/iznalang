#ifndef SHOOTING_TEXTURE_INL_H
#define SHOOTING_TEXTURE_INL_H

namespace stg {

template <typename T>
std::vector<std::shared_ptr<typename T::partial_expression>> SplitTexture(
	std::shared_ptr<T> tex,
	int num_x,
	int num_y)
{
	using PartialTexture = typename T::partial_expression;

	int partial_width = tex->GetWidth() / num_x;
	int partial_height = tex->GetHeight() / num_y;

	std::vector<std::shared_ptr<PartialTexture>> v;
	v.reserve(num_x * num_y);
	for (int iy = 0; iy < num_y; ++iy)
	{
		for (int ix = 0; ix < num_x; ++ix)
		{
			v.emplace_back(
				std::make_shared<PartialTexture>(
				tex,
				partial_width  * ix,
				partial_height * iy,
				partial_width,
				partial_height));
		}
	}

	return v;
}

template <typename T>
std::vector<std::shared_ptr<typename T::partial_expression>> SplitTexture(
	std::shared_ptr<T> tex,
	int num_x,
	int num_y,
	int partial_width,
	int partial_height)
{
	using PartialTexture = typename T::partial_expression;

	std::vector<std::shared_ptr<PartialTexture>> v;
	v.reserve(num_x * num_y);
	for (int iy = 0; iy < num_y; ++iy)
	{
		for (int ix = 0; ix < num_x; ++ix)
		{
			v.emplace_back(
				std::make_shared<PartialTexture>(
				tex,
				partial_width  * ix,
				tex->GetHeight() - partial_height * (iy + 1),
				partial_width,
				partial_height));
		}
	}

	return v;
}

}// stg

#endif//SHOOTING_TEXTURE_INL_H
