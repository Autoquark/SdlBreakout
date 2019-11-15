#pragma once
struct Vector2
{
public:
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

	Vector2& operator+=(const Vector2& rhs) // compound assignment (does not need to be a member,
	{                           // but often is, to modify the private members)
	  /* addition of rhs to *this takes place here */
		this->x += rhs.x;
		this->y += rhs.y;
		return *this; // return the result by reference
	}

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	// I copied these comments and have no idea what they mean
	friend Vector2 operator+(Vector2 lhs,        // passing lhs by value helps optimize chained a+b+c
		const Vector2& rhs) // otherwise, both parameters may be const references
	{
		lhs += rhs; // reuse compound assignment
		return lhs; // return the result by value (uses move constructor)
	}

	float x;
	float y;
};