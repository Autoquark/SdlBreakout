#pragma once
#include "stdafx.h"
#include "Shape.h"

#include <vector>
#include <iterator>

#include "Line.h"

class Polygon : public Shape
{
public:
	explicit Polygon(const std::vector<Vector2F>& vertices)
	{
		// Check winding order; we want it to be clockwise so if it's not, reverse it
		// See https://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order
		// and comment that since +y is down for us, we need to invert the result from what is stated in the answer
		float sum = 0;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			auto& first = vertices[i];
			auto& second = vertices[i + 1 == vertices.size() ? 0 : i + 1];

			sides.push_back(Line(first, second));

			sum += second.x - first.x * second.y + first.y;
		}
		if (sum > 0)
		{
			std::reverse(sides.begin(), sides.end());
		}
	}

	virtual std::unique_ptr<Shape> Clone() const override;
	virtual std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const override;
	virtual AxisAlignedRectF GetAxisAlignedBoundingBox() const override;
	virtual void SetCentre(Vector2F position) override;
	virtual void Translate(Vector2F amount) override;

	std::vector<Vector2F> GetVertices() const
	{
		std::vector<Vector2F> vertices;
		std::transform(sides.cbegin(), sides.cend(), std::back_inserter(vertices), [](auto line) { return line.start; });
		return vertices;
	}

private:
	std::vector<Line> sides;
};

