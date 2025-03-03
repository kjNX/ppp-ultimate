#include <vector>
#include <chrono>

#include "Vec.hpp"
#include "Light.hpp"
#include "Sphere.hpp"
#include "Render.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main()
{
	int background_width, background_height;
	unsigned char* const background{stbi_load("bg.jpg",
		&background_width, &background_height, nullptr, 3)};
	std::vector<Material> materials{
		{ 1,
			{
				.6,
				.3,
				.1,
				0
			}, {
				.4,
				.4,
				.3
			}, 50
		},{ 1.5,
			{
				0,
				.5,
				.1,
				.8
			}, {
				.6,
				.7,
				.8
			}, 125
		}, { 1,
			{
				.9,
				.1,
				0,
				0
			}, {
				.3,
				.1,
				.1
			}, 10
		}, { 1,
			{
				0,
				10,
				.8,
				0
			}, {
				1,
				1,
				1
			}, 1425
		}
	};
	// Sphere sp1{{-12, 0, -16}, 10};
	/*
	std::vector<Sphere> spheres{{
			{
				-3,
				0,
				-8,
			}, 2, materials[0]
		}, {
			{
				-1,
				-1.5,
				-6,
			}, 2, materials[1]
		}, {
			{
				1.5,
				-.5,
				-9
			}, 3, materials[2]
		}, {
			{
				7,
				5,
				-9
			}, 4, materials[3]
		}
	};
	*/
	const size_t& sphere_rows{8u};
	const size_t& sphere_cols{8u};
	const size_t& sphere_count{sphere_rows * sphere_cols};
	std::vector<Sphere> spheres(sphere_count);

	for(size_t i{0u}; i < sphere_rows; ++i) for(size_t j{0u}; j < sphere_cols; ++j)
	{
		size_t curr{j + i * sphere_cols};
		spheres[curr] = {
			{
				-10.f + 3.25f * static_cast<float>(j),
				-4,
				-6.f + -3.25f * static_cast<float>(i)
			}, 2, materials[static_cast<int>(rand() % materials.size())]
		};
	}

	std::vector<Light> lights{
		{
			{
				-20,
				20,
				10
			}, 1.5
		}, {
			{
				30,
				50,
				-12.5
			}, 1.8
		}, {
			{
				30,
				20,
				15
			}, 1.7
		}
	};

	std::chrono::duration sequentialTime{sequentialRender(spheres, lights, background, background_width, background_height)};
	printf("Sequential time: %f\n", sequentialTime.count());

	std::chrono::duration parallelTime{parallelRender(spheres, lights, background, background_width, background_height)};
	printf("Parallel time: %f\n", parallelTime.count());

	std::chrono::duration<float> speedup{sequentialTime / parallelTime};
	printf("Speedup: %f\n", speedup.count());

	stbi_image_free(background);

	return 0;
}