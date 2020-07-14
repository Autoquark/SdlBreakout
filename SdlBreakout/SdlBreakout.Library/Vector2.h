#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include <string>
#include <algorithm>

using namespace std::string_literals;

struct Vector2
{
public:

	static float DistanceBetween(const Vector2& first, const Vector2& second)
	{
		return (second - first).Magnitude();
	}

	static Vector2 LinearInterpolate(Vector2 first, Vector2 second, float proportion)
	{
		proportion = std::clamp(proportion, 0.0f, 1.0f);
		return first + proportion * (second - first);
	}

	Vector2() = default;

	Vector2(float x, float y) : x(x), y(y)
	{
	}

	friend bool operator==(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	friend bool operator!=(const Vector2& lhs, const Vector2& rhs)
	{
		return !(lhs == rhs);
	}

	Vector2& operator *= (float rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	friend Vector2 operator*(Vector2 lhs, float rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	friend Vector2 operator*(float lhs, Vector2 rhs)
	{
		rhs *= lhs;
		return rhs;
	}

	Vector2& operator /= (float rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	friend Vector2 operator/(Vector2 lhs, float rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	friend Vector2 operator/(float lhs, Vector2 rhs)
	{
		rhs /= lhs;
		return rhs;
	}

	Vector2& operator+=(const Vector2& rhs) // compound assignment (does not need to be a member,
	{                           // but often is, to modify the private members)
	  /* addition of rhs to *this takes place here */
		this->x += rhs.x;
		this->y += rhs.y;
		return *this; // return the result by reference
	}

	Vector2& operator-=(const Vector2& rhs) // compound assignment (does not need to be a member,
	{                           // but often is, to modify the private members)
	  /* addition of rhs to *this takes place here */
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	friend Vector2 operator-(Vector2 operand)
	{
		operand.x *= -1;
		operand.y *= -1;
		return operand;
	}

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	// I copied these comments and have no idea what they mean
	friend Vector2 operator+(Vector2 lhs,        // passing lhs by value helps optimize chained a+b+c
		const Vector2& rhs) // otherwise, both parameters may be const references
	{
		lhs += rhs; // reuse compound assignment
		return lhs; // return the result by value (uses move constructor)
	}

	friend Vector2 operator-(Vector2 lhs,
		const Vector2& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	std::string ToString() const
	{
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}

	float DotProduct(Vector2 other) const
	{
		return x * other.x + y * other.y;
	}

	float Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	void Normalise()
	{
		auto magnitude = Magnitude();
		x /= magnitude;
		y /= magnitude;
	}

	Vector2 Normalised() const
	{
		auto clone = Vector2(*this);
		clone.Normalise();
		return clone;
	}

	void Reflect(Vector2 normal)
	{
		*this -= 2 * (this->DotProduct(normal)) * normal;
	}

	Vector2 Reflected(Vector2 normal) const
	{
		auto clone = Vector2(*this);
		clone.Reflect(normal);
		return clone;
	}

	void Rotate(float degreesClockwise)
	{
		auto temp = x;
		auto inRadians = degreesClockwise * (M_PI / 180);

		x = (float)(std::cos(-inRadians) * x - std::sin(-inRadians) * y);
		y = (float)(std::sin(-inRadians) * temp - std::cos(-inRadians) * y);
	}

	Vector2 Rotated(float degreesClockwise) const
	{
		auto clone = Vector2(*this);
		clone.Rotate(degreesClockwise);
		return clone;
	}

	float x;
	float y;
};