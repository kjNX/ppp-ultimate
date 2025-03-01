#include <limits>
#include <vector>
#include <cmath>
#include <chrono>

#include "Commons.hpp"
#include "Light.hpp"
#include "Sphere.hpp"

using clk = std::chrono::steady_clock;

float pixelRay(const size_t& i, const size_t& total)
{ return 2.f * (static_cast<float>(i) + .5f)/static_cast<float>(total) - 1.f * std::tan(Commons::FOV/2.f); }

Vec3f reflect(const Vec3f& i, const Vec3f& N) { return i - N * 2.f * (i * N); }

Vec3f refract(const Vec3f& i, const Vec3f& N, const float& refractive_index)
{
	float cosi{-std::max(-1.f, std::min(1.f, i * N))};
	float etai{1}, etat{refractive_index};
	Vec3f n{N};
	if(cosi < 0)
	{
		cosi = -cosi;
		std::swap(etai, etat);
		n = -N;
	}
	float eta{etai / etat};
	float k{1 - eta * eta * (1 - cosi * cosi)};
	return k < 0 ? Vec3f{0, 0, 0} : i * eta + n * (eta * cosi - std::sqrtf(k));
}

size_t sceneIntersect(const Vec3f& origin, const Vec3f& direction, const std::vector<Sphere>& spheres,
	Vec3f& hit, Vec3f& N)
{
	float min_dist{std::numeric_limits<float>::max()};
	size_t temp{SIZE_MAX};
	for(size_t i{0u}; i < spheres.size(); ++i)
	{
		if(float dist_i{}; spheres[i].rayIntersects(origin, direction, dist_i) && dist_i < min_dist)
		{
			min_dist = dist_i;
			temp = i;
		}
	}
	hit = origin + direction * min_dist;
	N = (hit - spheres[temp].center).normalize();
	return min_dist < Commons::DRAW_DISTANCE ? temp : SIZE_MAX;
}

Vec3f castRay(const Vec3f& origin, const Vec3f& direction,
	const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& bg_width, const int& bg_height,
	const size_t& depth = 0)
{
	Vec3f hit{}, N{};
	const auto& origin_func{[&hit, &N](const Vec3f& dir)
		{ return hit - N * (dir * N < 0 ? 1e-3f : -1e-3f); }};

	size_t idx{sceneIntersect(origin, direction, spheres, hit, N)};
	if(depth > Commons::MAX_REFLECTIONS || idx >= spheres.size()) //return Commons::BG_COLOR;
	{
		int base_idx{static_cast<int>(bg_width * (direction.x / 2.f + .5) + bg_height * (direction.y / 2.f + .5))};
		return Vec3f{bg[base_idx] / 255.f, bg[base_idx + 1] / 255.f, bg[base_idx + 2] / 255.f};
	}

	Vec3f reflect_direction{reflect(direction, N).normalize()};
	Vec3f refract_direction{refract(direction, N, spheres[idx].material.refractive_index).normalize()};

	Vec3f reflect_origin{origin_func(reflect_direction)};
	Vec3f refract_origin{origin_func(refract_direction)};

	Vec3f reflect_color{castRay(reflect_origin, reflect_direction, spheres, lights, bg, bg_width, bg_height, depth + 1)};
	Vec3f refract_color{castRay(refract_origin, refract_direction, spheres, lights, bg, bg_width, bg_height, depth + 1)};

	const Material& mat{spheres[idx].material};
	float diffuse_light{0.f}, specular_light{0.f};
	for(const auto& [position, intensity] : lights)
	{
		Vec3f light_direction{(position - hit).normalize()};
		float light_distance{(position - hit).norm()};

		Vec3f shadow_origin{origin_func(light_direction)};
		Vec3f shadow_hit{}, shadow_N{};
		size_t shadow_idx{sceneIntersect(shadow_origin, light_direction, spheres, shadow_hit, shadow_N)};
		if(shadow_idx < spheres.size() && (shadow_hit - shadow_origin).norm() < light_distance && shadow_idx != idx) continue;
		diffuse_light += intensity * std::max(0.f, light_direction * N);
		specular_light += std::powf(std::max(0.f, reflect(light_direction, N) * direction),
			mat.specular_exp) * intensity;
	}
	return mat.diffuse * diffuse_light * mat.albedo.x +
		Vec3f{1, 1, 1} * specular_light * mat.albedo.y +
			reflect_color * mat.albedo.z +
				refract_color * mat.albedo.w;
}

void render(std::vector<Vec3f>& fb,
	const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& bg_width, const int& bg_height,
	const size_t& begin = 0, const size_t& end = Commons::RENDER_PIXELS)
{
	/*
	for(size_t i{0u}; i < Commons::RENDER_HEIGHT; ++i)
	{
		float y{-pixelRay(i, Commons::RENDER_HEIGHT)};
		for(size_t j{0u}; j < Commons::RENDER_WIDTH; ++j)
		{
			// fb[i * RENDER_WIDTH + j] = {intDiv(i, RENDER_HEIGHT), intDiv(j, RENDER_WIDTH), 0};
			float x{pixelRay(j, Commons::RENDER_WIDTH) * Commons::ASPECT_RATIO};
			Vec3f direction{Vec3f{x, y, -1}.normalize()};
			fb[i * Commons::RENDER_WIDTH + j] = castRay(Commons::RENDER_ORIGIN, direction, spheres, lights);
		}
	}
	*/

	for(size_t i{begin}; i < end; ++i)
	{
		float x{static_cast<float>(i % Commons::RENDER_WIDTH) + .5f - Commons::RENDER_WIDTH / 2.f};
		float y{-static_cast<float>(i) / Commons::RENDER_WIDTH - .5f + Commons::RENDER_HEIGHT / 2.f};
		float z{-static_cast<float>(Commons::RENDER_HEIGHT) / (2 * std::tanf(Commons::FOV / 2.f))};
		fb[i] = castRay(Commons::RENDER_ORIGIN, Vec3f{x, y, z}.normalize(),
			spheres, lights, bg, bg_width, bg_height);
	}
}

std::chrono::duration<float> sequentialRender(const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& width, const int& height)
{
	std::vector<Vec3f> fb(Commons::RENDER_PIXELS);
	auto startTime{clk::now()};
	render(fb, spheres, lights, bg, width, height);
	auto endTime{clk::now()};
	Commons::ppmWriteBuffer(Commons::SEQUENTIAL_PATH, fb);
	return endTime - startTime;
}

std::chrono::duration<float> parallelRender(const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& width, const int& height)
{
	std::vector<Vec3f> fb(Commons::RENDER_PIXELS);
	const size_t& partition_size{Commons::RENDER_PIXELS / Commons::THREAD_COUNT};
	std::unique_ptr<std::jthread> threads[Commons::THREAD_COUNT - 1u]{};

	size_t i{0u}, j{1u};
	auto startTime{clk::now()};
	for(; i < Commons::THREAD_COUNT; i = j, ++j)
		threads[i] = std::make_unique<std::jthread>(&render,
			std::ref(fb), spheres, lights, bg, width, height, partition_size * i, partition_size * j);

	render(fb, spheres, lights, bg, width, height, partition_size * i, partition_size * j);
	for(const std::unique_ptr<std::jthread>& k : threads) k -> join();
	auto endTime{clk::now()};
	Commons::ppmWriteBuffer(Commons::PARALLEL_PATH, fb);
	return endTime - startTime;
}