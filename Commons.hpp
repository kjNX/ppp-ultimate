#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <cmath>
#include <vector>
#include <fstream>
#include <thread>

#include "Vec.hpp"

namespace Commons
{
	inline constexpr const char* const& OUTPUT_FILENAME{"out.ppm"};

	inline const size_t& THREAD_COUNT{std::jthread::hardware_concurrency()};

	// display settings
	inline constexpr size_t RENDER_WIDTH{640u};
	inline constexpr size_t RENDER_HEIGHT{480u};
	inline constexpr size_t RENDER_PIXELS{RENDER_WIDTH * RENDER_HEIGHT};
	inline constexpr float ASPECT_RATIO{RENDER_WIDTH / static_cast<float>(RENDER_HEIGHT)};
	inline constexpr float FOV{M_PI / 2.};

	// detail settings
	inline constexpr float DRAW_DISTANCE{1000};
	inline constexpr size_t MAX_REFLECTIONS{5};

	inline constexpr Vec3f VEC3F_ZERO{0, 0, 0};

	inline constexpr Vec3f BG_COLOR{.2, .7, .8};
	inline constexpr Vec3f RENDER_ORIGIN{VEC3F_ZERO};

	inline constexpr const char* const& SEQUENTIAL_PATH{"sequential.ppm"};
	inline constexpr const char* const& PARALLEL_PATH{"parallel.ppm"};

	template<typename T> float intDiv(const T& a, const T& b)
	{ return static_cast<float>(a) / static_cast<float>(b); }

	void ppmWriteBuffer(const char* const& filename, const std::vector<Vec3f>& fb);
}

#endif //SETTINGS_HPP
