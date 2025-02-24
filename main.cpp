#include <cstdio>
#include <vector>
#include <fstream>
#include <bits/ranges_algo.h>

#include "Sphere.hpp"
#include "Vec.hpp"

constexpr const char* const& OUTPUT_FILENAME{"out.ppm"};

constexpr size_t RENDER_WIDTH{1024u};
constexpr size_t RENDER_HEIGHT{768u};
constexpr float ASPECT_RATIO{RENDER_WIDTH / static_cast<float>(RENDER_HEIGHT)};
constexpr float FOV{M_PI / 2.};

constexpr Vec3f VEC3F_ZERO{0, 0, 0};

constexpr Vec3f BG_COLOR{VEC3F_ZERO};
constexpr Vec3f RENDER_ORIGIN{VEC3F_ZERO};

template<typename T> float intDiv(const T& a, const T& b)
{ return static_cast<float>(a) / static_cast<float>(b); }

float pixelRay(const size_t& i, const size_t& total)
{ return 2.f * (static_cast<float>(i) + .5f)/static_cast<float>(total) - 1.f * std::tan(FOV/2.f); }

Vec3f castRay(const Vec3f& origin, const Vec3f& direction, const Sphere& sphere)
{
	float minDist{std::numeric_limits<float>::max()};
	if(!sphere.rayIntersects(origin, direction, minDist)) return BG_COLOR;
	return {1, 1, 1};
}

void ppmWriteBuffer(const char* const& filename, const std::vector<Vec3f>& fb)
{
	if(std::ofstream out{filename}; out.is_open())
	{
		out << "P6\n" << RENDER_WIDTH << ' ' << RENDER_HEIGHT << "\n255\n"; // header
		for(const Vec3f& i : fb) for(int j{0}; j < 3; ++j)
			out << static_cast<unsigned char>(255 * std::max(0.f, std::min(1.f, i[j])));
	}
}

void render(const Sphere& sphere)
{
	std::vector<Vec3f> fb(RENDER_WIDTH * RENDER_HEIGHT);

	for(size_t i{0u}; i < RENDER_HEIGHT; ++i) for(size_t j{0u}; j < RENDER_WIDTH; ++j)
	{
		// fb[i * RENDER_WIDTH + j] = {intDiv(i, RENDER_HEIGHT), intDiv(j, RENDER_WIDTH), 0};

		float x{pixelRay(j, RENDER_WIDTH) * ASPECT_RATIO};
		float y{-pixelRay(i, RENDER_HEIGHT)};
		Vec3f direction{Vec3f{x, y, -1}.normalize()};
		fb[i * RENDER_WIDTH + j] = castRay(RENDER_ORIGIN, direction, sphere);
	}
	ppmWriteBuffer(OUTPUT_FILENAME, fb);
}

int main()
{
	Sphere sp1{{-12, 0, -16}, 10};

	render(sp1);
	return 0;
}