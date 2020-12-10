#pragma once

#include <optional>
#include <vector>
#include <memory>
#include "Contact.h"
#include "OptionalUtilities.h"

class CircleF;
class AxisAlignedRectF;
class Point;
class Line;
class CompoundShape;

class Shape
{
public:
	Shape() = default;
	virtual ~Shape() = default;

	virtual std::unique_ptr<Shape> Clone() const = 0;

	enum class InternalityFilter
	{
		Both,
		Internal,
		External
	};

	// Return the first contact with the given shape, if any, when this shape is moved along the given vector
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainst(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given axis aligned rectangle's first contact with this shape, if any, when the other shape is moved along the given vector
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const AxisAlignedRectF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given circle's first contact with this shape, if any, when the circle is moved along the given vector
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const CircleF& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given lines' first contact with this shape, if any, when the line is moved along the given vector
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const Line& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	// Return the given point's first contact with this shape, if any, when the point is moved along the given vector
	[[nodiscard]]
	virtual std::optional<Contact> CastAgainstThis(const Point& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both) const = 0;

	[[nodiscard]]
	virtual AxisAlignedRectF GetAxisAlignedBoundingBox() const = 0;

	virtual Vector2F GetCentre() const;

	virtual void SetCentre(Vector2F position) = 0;
	void SetCentre(float x, float y)
	{
		SetCentre(Vector2F(x, y));
	}

	/// <summary>
	/// Translates this shape's position by the given movement or until it comes into contact with the given other shape
	/// </summary>
	/// <param name="other">The shape to move into contact with</param>
	/// <param name="movement">Movement vector</param>
	/// <param name="internalityFilter">Specifies whether to check for internal, external or both kinds of contact with the other shape</param>
	/// <returns>Optional contact representing the contact with the other shape if any occurred. Note that the contact distance will slightly exceed the actual distance moved as a small fudge
	/// factor is subtracted to account for accuracy limitations</returns>
	std::optional<Contact> MoveToContact(const Shape& other, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both);

	/// <summary>
	/// Translates this shape towards the given contact point, leaving a separation of Contact::MIN_SEPARATION_DISTANCE. If the shape is already within that distance, does nothing
	/// </summary>
	/// <param name="contact"></param>
	/// <returns>The movement made by the shape</returns>
	Vector2F MoveToContact(const Contact& contact);

	/// <summary>
	/// Translates this shape's position by the given movement or until it reaches the given contact point
	/// </summary>
	/// <param name="contact">Contact limiting the movement of the shape, or an empty optional for no limitation</param>
	/// <param name="movement">Movement vector</param>
	/// <returns>contact is returned</returns>
	std::optional<Contact> MoveToContact(const std::optional<Contact>& contact, const Vector2F& movement);

	/// <summary>
	/// Translates this shape's position by the given movement or until it comes into contact with one of the given other shapes
	/// </summary>
	/// <param name="other">The shapes to move into contact with</param>
	/// <param name="movement">Movement vector</param>
	/// <param name="internalityFilter">Specifies whether to check for internal, external or both kinds of contact with the other shapes</param>
	/// <returns>Optional contact representing the contact with the other shape if any occurred. Note that the contact distance will slightly exceed the actual distance moved as a small fudge
	/// factor is subtracted to account for accuracy limitations</returns>
	std::optional<Contact> MoveToContact(const std::vector<const Shape*>& others, const Vector2F& movement, const InternalityFilter internalityFilter = InternalityFilter::Both);

	virtual void Translate(Vector2F amount) = 0;
	void Translate(float x, float y)
	{
		Translate(Vector2F(x, y));
	}

	[[nodiscard]]
	static std::optional<Contact> ClosestContact(const std::optional<Contact>& first, const std::optional<Contact>& second)
	{
		std::optional<Contact> out;
		ClosestContact(first, second, out);
		return out;
	}

	static bool ClosestContact(const std::optional<Contact>& first, const std::optional<Contact>& second, std::optional<Contact>& out)
	{
		return optionalUtilities::MinValue(first, second, [](auto x, auto y) { return y.distance > x.distance; }, out);
	}

	// Returns the index in the given vector of the Contact with the lowest distance that matches the given filter, or an empty optional if the input contains no such contacts
	[[nodiscard]]
	static int FindClosestCollisionIndex(const std::vector<std::optional<Contact>>& contacts, const InternalityFilter& internalityFilter = InternalityFilter::Both)
	{
		int bestIndex = -1;
		for (unsigned int i = 0; i < contacts.size(); i++)
		{
			auto nullable = contacts[i];
			if (!nullable.has_value())
			{
				continue;
			}
			auto& contact = *nullable;
			if ((contact.stationarySide && internalityFilter == InternalityFilter::Internal) || (!contact.stationarySide && internalityFilter == InternalityFilter::External))
			{
				continue;
			}

			if (bestIndex == -1 || (contact.distance < contacts[bestIndex]->distance))
			{
				bestIndex = i;
			}
		}

		return bestIndex;
	}

	// Returns the Contact in the given vector with the lowest distance that matches the given filter, or an empty optional if the input contains no such contacts
	[[nodiscard]]
	static std::optional<Contact> FindClosestCollision(const std::vector<std::optional<Contact>>& contacts, const InternalityFilter& internalityFilter = InternalityFilter::Both)
	{
		auto index = FindClosestCollisionIndex(contacts, internalityFilter);
		// cppcheck-suppress containerOutOfBounds
		return index == -1 ? std::nullopt : contacts[index];
	}

protected:
	Shape(const Shape&) = default;
};