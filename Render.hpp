#ifndef RENDER_HPP
#define RENDER_HPP
#include <chrono>
#include <vector>

struct Sphere;
struct Light;

// void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights);
std::chrono::duration<float> sequentialRender(const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& bg_width, const int& bg_height);
std::chrono::duration<float> parallelRender(const std::vector<Sphere>& spheres, const std::vector<Light>& lights,
	const unsigned char* const& bg, const int& bg_width, const int& bg_height);

#endif //RENDER_HPP
