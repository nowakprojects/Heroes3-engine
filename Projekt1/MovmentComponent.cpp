#include "pch.h"
#include "MovmentComponent.h"
#include "Direction.h"

MovmentComponent::MovmentComponent(sf::Sprite& sprite)
	: sprite(sprite), nodeIndex(0),moving(false),speed(120.f), positionChanged(false),path(nullptr)
{
	this->pathEnded = false;
	this->velocity = sf::Vector2f(0, 0);
	this->tilePos = sf::Vector2i(-1, -1);
}


MovmentComponent::~MovmentComponent()
{
}

const sf::Vector2f & MovmentComponent::getVelocity() const
{
	return this->velocity;
}

const bool MovmentComponent::getState(const short unsigned state) const
{
	switch (state)
	{
	case IDLE:
		return(velocity.x == 0 && velocity.y == 0);
	case MOVING_DOWN:
		return (velocity.x == 0 && velocity.y > 0);
	case MOVING_DOWN_LEFT:
		return (velocity.x < 0 && velocity.y > 0);
	case MOVING_LEFT:
		return (velocity.x < 0 && velocity.y == 0);
	case MOVING_UP_LEFT:
		return (velocity.x < 0 && velocity.y < 0);
	case MOVING_UP:
		return (velocity.x == 0 && velocity.y < 0);
	case MOVING_UP_RIGHT:
		return (velocity.x > 0 && velocity.y < 0);
	case MOVING_RIGHT:
		return (velocity.x > 0 && velocity.y == 0);
	case MOVING_DOWN_RIGHT:
		return (velocity.x > 0 && velocity.y > 0);
	}
	return false;
}

void MovmentComponent::fixSpritePos()
{
	sf::Vector2f fixedPos = this->sprite.getPosition();
	fixedPos /= TILEWIDTH;
	fixedPos.x = std::round(fixedPos.x);
	fixedPos.y = std::round(fixedPos.y);
	fixedPos *= TILEWIDTH;
	this->sprite.setPosition(fixedPos);
}

bool MovmentComponent::isHeroChangedPos()
{
	if (this->positionChanged)
	{
		this->positionChanged = false;
		// I assume that hero's position will be changed after return true
		return true;
	}
	return false;
}

bool MovmentComponent::isPathEnded()
{
	if (this->pathEnded)
	{
		this->pathEnded = false;
		return true;
	}
	return false;
}

bool MovmentComponent::setNewPath(std::shared_ptr<Path> path)
{
	if (!moving)
	{
		this->path = path;
		this->pathEnded = false;
		return true;
	}
	else
		return false;
}

sf::Vector2i MovmentComponent::getActualHeroTilePos()
{
	if (path)
	{
		return this->path->nodes[nodeIndex].coord;
	}
	if (!moving)
	{
		return this->tilePos;
	}
	return sf::Vector2i(-1,-1);
}

void MovmentComponent::DoMove(float x, float y)
{
	this->positionChanged = true;
	this->velocity = sf::Vector2f(x, y);

}

void MovmentComponent::startMoving()
{
	if (path)
	{
		this->moving = true;
		this->nodeIndex = (int)path->nodes.size()-1;
		this->tilePos = this->path->nodes[nodeIndex].coord;
		this->offset = sf::Vector2f(TILEWIDTH, TILEWIDTH);
		this->pathEnded = false;
	}
	
}

void MovmentComponent::stopMoving()
{
	if (path)
	{
		this->moving = false;
		this->tilePos = this->path->nodes[nodeIndex].coord;
	}
}

void MovmentComponent::update(const float & dt)
{
	this->sprite.move(this->velocity *dt * this->speed);
	this->offset += this->velocity * dt * this->speed;
	// sprite moved one tile distance
	if (abs(this->offset.x) >= TILEWIDTH || abs(this->offset.y) >= TILEWIDTH)
	{
		this->velocity = sf::Vector2f(0, 0);
		this->offset = sf::Vector2f(0, 0);
		if (moving)
		{
			if (0 < nodeIndex)
			{
				
				PathNode cur = this->path->nodes[nodeIndex];
				PathNode next = this->path->nodes[nodeIndex - 1];
				if (nodeIndex == 1 && next.accessible == PathNode::BLOCKVIS)
				{	
					this->stopMoving();
					this->DoMove(
						(next.coord.x - cur.coord.x)* 0.01,
						(next.coord.y - cur.coord.y)* 0.01);
					this->offset = sf::Vector2f(TILEWIDTH, TILEWIDTH);
					//this->path->nodes.pop_back();
				}
				else
				{
					this->fixSpritePos();
					this->DoMove(next.coord.x - cur.coord.x, next.coord.y - cur.coord.y);
					nodeIndex--;
					this->path->nodes.pop_back();
				}
				
			}
			else
			{
				this->stopMoving();
				this->pathEnded = true;
				this->path = nullptr;
			}
		}
		else
		{
			//path interupted or visiting blockVistited object
			this->pathEnded = true;
			this->path = nullptr;
		}
	}
}
