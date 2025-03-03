#include "Model.hpp"

#include <fstream>
#include <sstream>

Model::Model(const char* const& filename) : m_vertices{}, m_faces{}
{
	if(std::ifstream in{filename}; in.is_open())
	{
		std::string line{};
		while(!in.eof())
		{
			std::getline(in, line);
			std::istringstream stream(line.c_str());
			char trash;
			if(!line.compare(0, 2, "v "))
			{
				stream >> trash;
				Vec3f vertex;
				for(int i{0}; i < 3; ++i) stream >> vertex[i];
				m_vertices.push_back(vertex);
			}
			else if(!line.compare(0, 2, "f "))
			{
				Vec3i face;
				int idx{};
				int count{0};
				while(stream >> idx)
				{
					--idx;
					face[count++] = idx;
				}
				if(count == 3) m_faces.push_back(face);
			}
		}
		printf("%lu vertices\n%lu faces\n", n_verts(), n_faces());
		Vec3f min{}, max{};
	}
	else printf("Failed to open %s.\n", filename);
}

size_t Model::n_verts() const { return m_vertices.size(); }
size_t Model::n_faces() const { return m_faces.size(); }

bool Model::ray_triangle_intersect(const int& fi, const Vec3f& origin, const Vec3f& dir, float& tnear)
{
	Vec3f edge1{point(vert(fi, 1)) - point(vert(fi, 0))};
	Vec3f edge2{point(vert(fi, 2)) - point(vert(fi, 0))};
	Vec3f pvec{cross(dir, edge2)};
	float det{edge1 * pvec};
	if(det < 1e-5) return false;

	Vec3f tvec{origin - point(vert(fi, 0))};
	float u{tvec * pvec};
}

Vec3f& Model::point(const size_t& i) { return m_vertices[i]; }
const Vec3f& Model::point(const size_t& i) const { return m_vertices[i]; }

int Model::vert(const size_t& fi, const size_t& li) const { return m_faces[fi][li]; }
