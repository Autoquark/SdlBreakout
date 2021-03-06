#pragma once

#include <cmath>
#include <string>
#include <algorithm>
#include <assert.h>

using namespace std::string_literals;

template<class ElementType>
struct Vector2
{
public:
	static Vector2 Between(const Vector2& first, const Vector2& second)
	{
		return second - first;
	}

	static float DistanceBetween(const Vector2& first, const Vector2& second)
	{
		return (second - first).Magnitude();
	}

	static Vector2 LinearInterpolate(Vector2 first, Vector2 second, float proportion)
	{
		proportion = std::clamp(proportion, 0.0f, 1.0f);
		return first + proportion * (second - first);
	}

	static Vector2 Up()
	{
		return Vector2(0, -1);
	}

	static Vector2 Down()
	{
		return Vector2(0, 1);
	}

	Vector2() = default;

	Vector2(ElementType x, ElementType y) : x(x), y(y)
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

	Vector2& operator *= (ElementType rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	friend Vector2 operator*(Vector2 lhs, ElementType rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	friend Vector2 operator*(ElementType lhs, Vector2 rhs)
	{
		rhs *= lhs;
		return rhs;
	}

	Vector2& operator /= (ElementType rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	friend Vector2 operator/(Vector2 lhs, ElementType rhs)
	{
		lhs /= rhs;
		return lhs;
	}

	friend Vector2 operator/(ElementType lhs, Vector2 rhs)
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

	template<class T>
	operator Vector2<T>() const
	{
		return Vector2<T>((T)x, (T)y);
	}

	std::string ToString() const
	{
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}

	float ClockwiseAngleFromUp() const
	{
		return Up().ClockwiseAngleToDegrees(*this);
	}

	float SignedAngleFromUp() const
	{
		return Up().SignedAngleToDegrees(*this);
	}

	float ClockwiseAngleToDegrees(Vector2 other) const
	{
		auto signedValue = SignedAngleToDegrees(other);
		if (signedValue < 0)
		{
			signedValue += 360;
		}
		return signedValue;
	}

	// Returns the smallest rotation in degrees which makes this vector parallel to the given vector. A positive value indicates a clockwise rotation, and a negative value a counterclockwise one
	float SignedAngleToDegrees(Vector2 other) const
	{
		return (float) ((180 / M_PI) * std::atan2(x * other.y - y * other.x, x * other.x + y * other.y));
	}

	float DotProduct(Vector2 other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 ElementMultiply(Vector2 other)
	{
		return Vector2(x * other.x, y * other.y);
	}

	float Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	void Normalise()
	{
		auto magnitude = Magnitude();
		assert(magnitude != 0);
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
		auto normalised = normal.Normalised();
		*this -= 2 * (this->DotProduct(normalised)) * normalised;
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

		x = (ElementType)(std::cos(-inRadians) * x - std::sin(-inRadians) * y);
		y = (ElementType)(std::sin(-inRadians) * temp + std::cos(-inRadians) * y);
	}

	Vector2 Rotated(float degreesClockwise) const
	{
		auto clone = Vector2(*this);
		clone.Rotate(degreesClockwise);
		return clone;
	}

	void SetMagnitude(float magnitude)
	{
		// Attempting to normalise a (0,0) vector triggers an assert but it's ok to SetMagnitude(0) on one
		if (magnitude != 0)
		{
			Normalise();
		}
		*this *= magnitude;
	}

	Vector2 WithMagnitude(float magnitude) const
	{
		auto clone = Vector2(*this);
		clone.SetMagnitude(magnitude);
		return clone;
	}

	void SetDirection(const Vector2& direction)
	{
		auto magnitude = Magnitude();
		*this = direction;
		SetMagnitude(magnitude);
	}

	Vector2 WithDirection(const Vector2& direction)
	{
		auto clone = Vector2(*this);
		clone.SetMagnitude(direction);
		return clone;
	}

	ElementType x;
	ElementType y;
};

typedef Vector2<float> Vector2F;