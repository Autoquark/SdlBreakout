#include <string>

using namespace std::string_literals;

#pragma once
struct Vector2
{
public:
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

	std::string ToString()
	{
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}

	float DotProduct(Vector2 other)
	{
		return x * other.x + y * other.y;
	}

	void Rotate(float degreesClockwise)
	{
		auto temp = x;
		x = std::cos(-degreesClockwise) * x - std::sin(-degreesClockwise) * y;
		y = std::sin(-degreesClockwise) * temp - std::cos(-degreesClockwise) * y;
	}

	float x;
	float y;
};