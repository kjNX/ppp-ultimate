#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include "Vec.hpp"

class Model {
private:
	std::vector<Vec3f> m_vertices;
	std::vector<Vec3i> m_faces;

public:
	Model(const char* const& filename);

	size_t n_verts() const;
	size_t n_faces() const;

	bool ray_triangle_intersect(const int& fi, const Vec3f& origin, const Vec3f& dir, float& tnear);

	Vec3f& point(const size_t& i);
	const Vec3f& point(const size_t& i) const;
	int vert(const size_t& fi, const size_t& li) const;
	void get_bbox(Vec3f& min, Vec3f& max);
};


#endif //MODEL_HPP
