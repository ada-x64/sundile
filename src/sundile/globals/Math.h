//-- Numerical typenames
//-- Wrapper classes for glm, imgui, float*
#include <imgui.h>
#include <glm/glm.hpp>
//-- Wrapper for Vec2's
 struct Vec2 {
	float x, y;
	Vec2() :x(0), y(0) {};
	Vec2(const Vec2& other) : x(other.x), y(other.y) {};
	Vec2(const ImVec2& other) : x(other.x), y(other.y) {};
	Vec2(const glm::vec2& other) : x(other.x), y(other.y) {};
	Vec2(const float* other) : x(other[0]), y(other[1]) {};
	Vec2(float _x, float _y) :x(_x), y(_y) {};
	Vec2(float all) :x(all), y(all) {};
	operator float* () { float f[] = { x, y }; return std::move(f); }
	operator int* () { int f[] = { static_cast<int>(x), static_cast<int>(y) }; return std::move(f); }
	operator glm::vec2() { return (glm::vec2(x, y)); }
	operator ImVec2() { return ImVec2(x, y); }
	bool operator == (const Vec2 other) { return (this->x == other.x && this->y == other.y); }
	bool operator != (const Vec2 other) { return !(*this == other); }
	Vec2 operator = (const Vec2 other) { this->x = other.x; this->y = other.y; return *this; }
	Vec2 operator += (const Vec2 other) { this->x += other.x; this->y += other.y; return *this; }
	Vec2 operator + (const Vec2 other) { return Vec2(x + other.x, y + other.y); }
	Vec2 operator -= (const Vec2 other) { x -= other.x; y -= other.y; return *this; }
	Vec2 operator - (const Vec2 other) { return Vec2(x - other.x, y - other.y); }
};

 struct Vec3 {
	float x, y, z;
	Vec3() :x(0), y(0), z(0) {};
	Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {};
	Vec3(const glm::vec3& other) : x(other.x), y(other.y), z(other.z) {};
	Vec3(const float* other) : x(other[0]), y(other[1]), z(other[2]) {};
	Vec3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};
	Vec3(float all) :x(all), y(all), z(all) {};
	operator float* () { float f[] = { x, y, z }; return std::move(f); }
	operator int* () { int f[] = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(z) }; return std::move(f); }
	operator glm::vec3() { return (glm::vec3(x, y, z)); }
	bool operator == (const Vec3 other) { return (x == other.x && y == other.y && z == other.z); }
	bool operator != (const Vec3 other) { return !(*this == other); }
	Vec3 operator = (const Vec3 other) { x = other.x; y = other.y; z = other.z; return *this; }
	Vec3 operator += (const Vec3 other) { x += other.x; y += other.y; z += other.z; return *this; }
	Vec3 operator + (const Vec3 other) { return Vec3(x + other.x, y + other.y, z + other.z); }
	Vec3 operator -= (const Vec3 other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	Vec3 operator - (const Vec3 other) { return Vec3(x - other.x, y - other.y, z - other.z); }
};

 struct Vec4 {
	float x, y, z, w;
	Vec4() :x(0), y(0), z(0), w(0) {};
	Vec4(const Vec4& other) : x(other.x), y(other.y), z(other.z) {};
	Vec4(const ImVec4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {};
	Vec4(const glm::vec4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {};
	Vec4(const float* other) : x(other[0]), y(other[1]), z(other[2]), w(other[3]) {};
	Vec4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {};
	Vec4(float all) :x(all), y(all), z(all), w(all) {};
	operator float* () { float f[] = { x, y, z, w }; return std::move(f); }
	operator int* () { int f[] = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(w) }; return std::move(f); }
	operator glm::vec4() { return (glm::vec4(x, y, z, w)); }
	bool operator == (const Vec4 other) { return (x == other.x && y == other.y && z == other.z && w == other.w); }
	bool operator != (const Vec4 other) { return !(*this == other); }
	Vec4 operator = (const Vec4 other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
	Vec4 operator += (const Vec4 other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
	Vec4 operator + (const Vec4 other) { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
	Vec4 operator -= (const Vec4 other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
	Vec4 operator - (const Vec4 other) { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
};