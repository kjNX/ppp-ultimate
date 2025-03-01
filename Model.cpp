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
			std::istringstream dustbin(line.c_str());
			char trash;
			if(!line.compare(0, 2, "v "))
			{
				dustbin >> trash;
				Vec3f v;
				// for()
			}
		}
		printf("%lu vertices\n%lu faces\n", n_verts(), n_faces());
		Vec3f min{}, max{};
		get_bbox(min, max);
	}
	else printf("Failed to open %s.\n", filename);
}

size_t Model::n_verts() const { return m_vertices.size(); }
size_t Model::n_faces() const { return m_faces.size(); }

Vec3f& Model::point(const size_t& i) { return m_vertices[i]; }
const Vec3f& Model::point(const size_t& i) const { return m_vertices[i]; }

int Model::vert(const size_t& fi, const size_t& li) const
{
	return m_faces[fi][li];
}
