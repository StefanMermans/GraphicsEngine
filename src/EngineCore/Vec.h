#ifndef VEC_H
#define VEC_H


class Vec3f {
public:
	Vec3f();
	Vec3f(const Vec3f &other);
	Vec3f(const float &x, const float &y, const float &z);
	virtual ~Vec3f() = default;

	Vec3f& operator= (const Vec3f &other);

	float x;
	float y;
	float z;
};

class Vec4f {
public:
	Vec4f();
	Vec4f(const Vec4f &other);
	Vec4f(const float &x, const float &y, const float &z, const float &w);
	virtual ~Vec4f() = default;

	Vec4f& operator= (const Vec3f &other);

	float x;
	float y;
	float z;
	float w;
};


#endif // !VEC_H
