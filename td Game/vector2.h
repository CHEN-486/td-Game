#ifndef _VECTOR_H_
#define _VECTOR_H_

#include<math.h>

class Vector2
{
public:
	double x = 0;
	double y = 0;

public:
	Vector2(double x,double y)
		:x(x),y(y)
	{

	}

	Vector2(const Vector2& vector)
	{
		x = vector.x;
		y = vector.y;
	}

	Vector2() = default;
	~Vector2() = default;

	Vector2 operator+(const Vector2& vector) const
	{
		return Vector2(vector.x + x, vector.y + y);
	}

	void operator+=(const Vector2& vector)
	{
		x += vector.x;
		y += vector.y;
	}

	Vector2 operator-(const Vector2& vector) const
	{
		return Vector2(x - vector.x, y - vector.y);
	}

	void operator-=(const Vector2& vector)
	{
		x -= vector.x;
		y -= vector.y;
	}

	void operator*=(const double& value)
	{
		x = x * value;
		y = y * value;
	}

	Vector2 operator*(const double& value) const
	{
		return Vector2(x * value, y * value);
	}

	double operator*(const Vector2& vector) const
	{
		return (x * vector.x + y * vector.y);
	}

	bool operator==(const Vector2& vector) const
	{
		return (x == vector.x && y == vector.y);
	}

	bool operator>(const Vector2& vector) const
	{
		return length() > vector.length();
	}

	bool operator<(const Vector2& vector) const
	{
		return length() < vector.length();
	}

	double length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2 normalize() const
	{
		double len = length();
		if (len == 0)
			return Vector2(0, 0);
		
		return Vector2(x / len, y / len);
	}

	bool approx_zero()
	{
		return length() < 0.0001;
	}

};

#endif // !_VECTOR_H_

