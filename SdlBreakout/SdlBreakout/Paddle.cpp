#include "stdafx.h"
#include "Paddle.h"

#include <iostream>
#include <algorithm>

#include "Ball.h"
#include "Debug.h"
#include "Game.h"
#include "Textures.h"
#include "CompoundShape.h"
#include "CircleF.h"
#include "Polygon.h"
#include "RenderUtils.h"
#include "Util.h"

const float Paddle::MAX_VIRTUAL_CURVE = 20;
const float Paddle::BUMP_UP_TIME = 0.1f;
const float Paddle::BUMP_DOWN_TIME = 0.3f;
//const float Paddle::BUMP_COOLDOWN = 1.5;
const float Paddle::BUMP_DISTANCE = 32;

Paddle::Paddle()
{
	// Rectangle with rounded ends
	auto compound = std::make_unique<CompoundShape>(Util::MakeVector<std::unique_ptr<Shape>>(
		std::make_unique<CircleF>(0.0f, 16.0f, 16.0f),
		std::make_unique<AxisAlignedRectF>(0.0f, 0.0f, 96.0f, 32.0f),
		std::make_unique<CircleF>(96.0f, 16.0f, 16.0f)
		));
	//centreSegment = static_cast<AxisAlignedRectF*>(compound->shapes[1].get());
	collisionBounds = std::move(compound);
	sprite = Textures::GetTexture("paddle");

	/*std::vector<Vector2F> vertices = {
		//Vector2F(29, 0), Vector2F(98, 0), Vector2F(127, 31), Vector2F(0, 31)
		//Vector2F(58, 0), Vector2F(69, 0), Vector2F(127, 29), Vector2F(127, 31), Vector2F(0, 31), Vector2F(0, 29)
		Vector2F(0, 31), Vector2F(18, 13), Vector2F(36, 4), Vector2F(54, 0), Vector2F(73, 0), Vector2F(91, 4), Vector2F(109, 13), Vector2F(127, 31)
	};
	collisionBounds = std::make_unique<Polygon>(vertices);
	sprite = Textures::GetTexture("paddleApproxCurve");*/

	moveSpeed = 650;
}

void Paddle::Update(float timeElapsed)
{
	auto& input = Game::GetInput();
	auto movement = Vector2F(std::clamp(input.GetMouseMovement().x, -moveSpeed * timeElapsed, moveSpeed * timeElapsed), 0);
	if (input.KeyIsDown(SDL_SCANCODE_LEFT))
	{
		movement = Vector2F(-moveSpeed * timeElapsed, 0);
	}
	else if (input.KeyIsDown(SDL_SCANCODE_RIGHT))
	{
		movement = Vector2F(moveSpeed * timeElapsed, 0);
	}

	auto time = Game::GetInstance().GetTime();
	if (input.MouseButtonDown(Input::MouseButton::Right) && time >= lastBumpTime + BUMP_UP_TIME + BUMP_DOWN_TIME)
	{
		bumpStartY = collisionBounds->GetCentre().y;
		lastBumpTime = time;
	}
	
	if (time < lastBumpTime + BUMP_UP_TIME)
	{
		movement.y = -(BUMP_DISTANCE * timeElapsed / BUMP_UP_TIME);
	}
	else if (time < lastBumpTime + BUMP_UP_TIME + BUMP_DOWN_TIME)
	{
		movement.y = (BUMP_DISTANCE * timeElapsed) / BUMP_DOWN_TIME;
	}
	// Reset position to account for any rounding error
	else if(lastBumpTime >= 0)
	{
		collisionBounds->SetCentre(collisionBounds->GetCentre().x, bumpStartY);
	}

	if (movement.Magnitude() > 0)
	{
		auto castVector = movement.WithMagnitude(movement.Magnitude() + Contact::MIN_SEPARATION_DISTANCE);

		auto level = Game::GetLevel();

		// Push balls as far as possible without passing the level bounds
		// Also keep track of the smallest contact distance to any of the (moved) balls
		std::optional<Contact> closestContact;
		for (auto ball : level->GetBalls())
		{
			auto ballContact = collisionBounds->CastAgainst(*ball->collisionBounds, castVector, Shape::InternalityFilter::External);
			if (ballContact.has_value())
			{
				auto ballMovement = movement.WithMagnitude(castVector.Magnitude() - ballContact->distance);
				ball->collisionBounds->MoveToContact(Game::levelArea, ballMovement);
				closestContact = Shape::ClosestContact(closestContact,
					collisionBounds->CastAgainst(*ball->collisionBounds, castVector, Shape::InternalityFilter::External));
			}
		}

		auto bestContact = Shape::ClosestContact(closestContact, collisionBounds->CastAgainst(Game::levelArea, castVector, Shape::InternalityFilter::Internal));

		auto actualMovement = movement.WithMagnitude(bestContact.has_value() ? bestContact->distance : movement.Magnitude());
		// Cast over our actual movement for powerups
		for (auto drop : level->GetGameObjects<PowerupDrop>())
		{
			if (collisionBounds->CastAgainst(*drop->collisionBounds, actualMovement, Shape::InternalityFilter::External))
			{
				drop->Collect();
			}
		}

		// Move as far as possible without penetrating the ball or level bounds
		collisionBounds->MoveToContact(bestContact, movement);

		speedLastUpdate = actualMovement / timeElapsed;
	}
	GameObject::Update(timeElapsed);

	if (powerup != nullptr)
	{
		RenderUtils::RenderTexture(Game::GetInstance().renderer, collisionBounds->GetCentre(), Vector2F(0.5, 0.5), *powerup->GetIcon());

		if (input.KeyPressed(SDL_Scancode::SDL_SCANCODE_SPACE))
		{
			powerup->Activate();
			powerup = nullptr;
		}
	}
}