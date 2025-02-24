#ifndef VECTOR_HPP
#define VECTOR_HPP

template <size_t COMP, typename T> struct Vec
{
	T data[COMP]{};

	T& operator[](const size_t& i) { return data[i]; }
	const T& operator[](const size_t& i) const { return data[i]; }
};

typedef Vec<2, float> Vec2f;
typedef Vec<3, float> Vec3f;
typedef Vec<4, float> Vec4f;

template<typename T> struct Vec<3, T>
{
	T x, y, z;

	constexpr Vec() : x{}, y{}, z{} {}
	constexpr Vec(const T& a, const T& b, const T& c) : x{a}, y{b}, z{c} {}

 	float norm() { return x * x + y * y + z * z; }
	Vec& normalize(T l = 1)
	{
		*this = *this * (l / norm());
		return *this;
	}

	T& operator[](const size_t& i) { return i <= 0 ? x : i == 1 ? y : z; }
	const T& operator[](const size_t& i) const { return i <= 0 ? x : i == 1 ? y : z; }
};

template<size_t COMP, typename T> T operator*(const Vec<COMP, T>& a, const Vec<COMP, T>& b)
{
	T temp{};
	for(size_t i{0u}; i < COMP; ++i) temp += a[i] * b[i];
	return temp;
}

template<size_t COMP, typename T> Vec<COMP, T> operator*(Vec<COMP, T> a, const T& b)
{
	for(size_t i{0u}; i < COMP; ++i) a[i] *= b;
	return a;
}

template<size_t COMP, typename T> Vec<COMP, T> operator-(const Vec<COMP, T>& a) { return a * -1.f; }

template<size_t COMP, typename T> Vec<COMP, T> operator+(Vec<COMP, T> a, const Vec<COMP, T>& b)
{
	for(size_t i{0u}; i < COMP; ++i) a[i] += b[i];
	return a;
}

template<size_t COMP, typename T> Vec<COMP, T> operator-(const Vec<COMP, T>& a, const Vec<COMP, T>& b)
{ return a + -b; }

#endif //VECTOR_HPP
