#include "Commons.hpp"

void Commons::ppmWriteBuffer(const char* const& filename, const std::vector<Vec3f>& fb)
{
	if(std::ofstream out{filename}; out.is_open())
	{
		out << "P6\n" << RENDER_WIDTH << ' ' << RENDER_HEIGHT << "\n255\n"; // header
		for(const Vec3f& i : fb) for(int j{0}; j < 3; ++j)
			out << static_cast<unsigned char>(255 * std::max(0.f, std::min(1.f, i[j])));
	}
}
