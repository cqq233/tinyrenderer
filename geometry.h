#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>
#include "tgaimage.h"

template<class t>
struct Vec2
{
	union 
	{
		struct { t x, y; };
		struct { t u, v; };
		t raw[2];
	};

	Vec2() : x(0), y(0) {}
	Vec2(t _x, t _y) : x(_x), y(_y) {}

	inline Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.v); }
	inline Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.v); }
	inline Vec2<t> operator *(float f)			const { return Vec2<t>(x * f, y * f); }
	template<class t>
	friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template<class t>
struct Vec3
{
	union
	{
		struct { t x, y, z; };
		struct { t u, v, w; };
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};

	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}

	inline Vec3<t> operator ^(const Vec3<t>& v) const { return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	inline Vec3<t> operator +(const Vec3<t>& V) const { return Vec3<t>(x + V.x, y + V.y, z + V.z); }
	inline Vec3<t> operator -(const Vec3<t>& V) const { return Vec3<t>(x - V.x, y - V.y, z - V.z); }
	inline Vec3<t> operator *(float f)			const { return Vec3<t>(x * f, y * f, z * f); }
	inline t       operator *(const Vec3<t>& v) const { return x * v.x + y * v.y + z * v.z; }
	float norm() const { return std::sqrt(x * x + y * y + z * z); }
	Vec3<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
	template<class t>
	friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>	Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>	Vec3i;

template<class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v)
{
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template<class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v)
{
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

void triangle_cross(Vec2i a, Vec2i b, Vec2i c, TGAImage& framebuffer, TGAColor color);

void triangle_bc(Vec3i a, Vec3i b, Vec3i c, TGAImage& framebuffer, TGAColor color);

void triangle(Vec3f a, Vec3f b, Vec3f c, float* zbuffer, TGAImage& framebuffer, TGAColor color);

void triangle_z(Vec3f* wps, Vec3i* sps, float* zbuffer, TGAImage& framebuffer, TGAColor color);


#endif // !_GEOMETRY_H_