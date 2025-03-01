#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "Vec.hpp"

#include <cmath>

#include "Material.hpp"

struct Sphere
{
	Vec3f center;
	float radius;
	Material material;

	bool rayIntersects(const Vec3f& origin, const Vec3f& direction, float& t0) const
	{
		Vec3f L{center - origin};
		float tca{L * direction};
		float d2{L * L - tca * tca};
		// float d{std::sqrtf(d2)};
		if(d2 > radius * radius) return false;
		float thc{std::sqrtf(radius * radius - d2 * d2)};
		t0 = tca - thc;
		float t1{tca + thc};
		if(t0 < 0) t0 = t1;
		if(t0 < 0) return false;
		return true;
	}
};

#endif //SPHERE_HPP
