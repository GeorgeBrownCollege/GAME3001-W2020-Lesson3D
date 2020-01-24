#include "ship.h"
#include "Game.h"
#include "Util.h"
#include "GLM/gtx/rotate_vector.hpp"


ship::ship()
{
	TheTextureManager::Instance()->load("../Assets/textures/ship3.png",
		"ship", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(glm::vec2(400.0f, 300.0f));
	setVelocity(glm::vec2(0.0f, 0.0f));
	setAcceleration(glm::vec2(0.0f, 0.0f));
	setIsColliding(false);
	setType(GameObjectType::SHIP);
	setSteeringState(SteeringState::IDLE);
	m_maxSpeed = 5.0f;
	m_currentHeading = 0.0; // current facing angle
	m_turnRate = 5.0f; // 5 degrees per frame
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right

}


ship::~ship()
{
}

void ship::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("ship", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), m_currentHeading, 255, true);
}

void ship::m_checkSteeringState()
{
	switch (getState())
	{
	case SteeringState::IDLE:
		//move();
		break;
	case SteeringState::SEEK:
		m_seek();
		move();
		m_checkBounds();
		m_checkArrival();
		break;
	case SteeringState::ARRIVE:
		break;
	case SteeringState::AVOID:
		break;
	case SteeringState::FLEE:
		break;
	}
}

void ship::update()
{
	m_checkSteeringState();
}

void ship::clean()
{
}

void ship::turnRight()
{
	m_currentHeading += m_turnRate;

	if (m_currentHeading > 360)
	{
		m_currentHeading -= 360.0f;
	}

	//std::cout << "currentHeading: " << m_currentHeading << std::endl;
	m_changeDirection();
	m_computeTargetDirection();
}

void ship::turnLeft()
{
	m_currentHeading -= m_turnRate;

	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}

	//std::cout << "currentHeading: " << m_currentHeading << std::endl;
	m_changeDirection();
	m_computeTargetDirection();
}

void ship::move()
{
	float angleToTarget = Util::signedAngle(getCurrentDirection(), m_targetDirection);
	//std::cout << "target Direction: " << angleToTarget << std::endl;

	//TODO: if angleToTarget > 0.0f then turn right

	//TODO: if angleToTarget < 0.0f then turn left

	//TODO: set velocity to the current Direction


	glm::vec2 newPosition = getPosition() + getVelocity() * m_maxSpeed;
	setPosition(newPosition);
}

glm::vec2 ship::getTargetPosition()
{
	return m_targetPosition;
}

glm::vec2 ship::getCurrentDirection()
{
	return m_currentDirection;
}

float ship::getMaxSpeed()
{
	return m_maxSpeed;
}

void ship::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void ship::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void ship::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void ship::m_checkBounds()
{

	if (getPosition().x > 800)
	{
		setPosition(glm::vec2(0.0f, getPosition().y));
	}

	if (getPosition().x < 0)
	{
		setPosition(glm::vec2(800.0f, getPosition().y));
	}

	if (getPosition().y > 600)
	{
		setPosition(glm::vec2(getPosition().x, 0.0f));
	}

	if (getPosition().y < 0)
	{
		setPosition(glm::vec2(getPosition().x, 600.0f));
	}

}

void ship::m_reset()
{

}

void ship::m_changeDirection()
{
	float x = cos(m_currentHeading * Util::Deg2Rad);
	float y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}


void ship::m_computeTargetDirection()
{
	glm::vec2 steeringVelocity = getTargetPosition() - getPosition();
	m_targetDirection = Util::normalize(steeringVelocity);
}

void ship::m_seek()
{
	m_computeTargetDirection();
	setVelocity(m_targetDirection); // TODO: remove after move is updated 
}

void ship::m_checkArrival()
{
	if (Util::distance(getPosition(),getTargetPosition()) <= 3.0f)
	{
		setSteeringState(IDLE);
	}
}
