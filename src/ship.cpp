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
	//m_reset();
	setIsColliding(false);
	setType(GameObjectType::SHIP);
	setState(SteeringState::IDLE);
	m_maxSpeed = 1.0f;
	m_currentDirection = 0.0;
	m_turnSpeed = 2.0f;
	m_steerForce = 0.1f;
	
	//TheTextureManager::Instance()->setColour("ship", 255, 255, 0);

	/*
	SoundManager::Instance()->load("../Assets/audio/yay.ogg",
		"yay", sound_type::SOUND_SFX);
	*/
}


ship::~ship()
{
}

void ship::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("ship", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), m_currentDirection, 255, true);
}

void ship::m_checkState()
{
	switch (getState())
	{
	case SteeringState::IDLE:
		move();
		break;
	case SteeringState::SEEK:
		m_seek();
		//move();
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
	/*m_checkState();
	
	m_checkBounds();*/

	//std::cout << "current Direction:" << m_currentDirection << std::endl;
}

void ship::clean()
{
}

void ship::turnRight()
{
	m_currentDirection += m_turnSpeed;
	if (m_currentDirection >= 360) 
	{
		m_currentDirection = 0;
	}

}

void ship::turnLeft()
{
	m_currentDirection -= m_turnSpeed;
	if (m_currentDirection < 0)
	{
		m_currentDirection = 359.0;
	}
}

void ship::move()
{
	

	

	float currentDirectionInRadians = m_currentDirection * Util::Deg2Rad;
	float velocityX = cos(currentDirectionInRadians) * m_maxSpeed;
	float velocityY = sin(currentDirectionInRadians) * m_maxSpeed;


	glm::vec2 newVelocity = glm::vec2(velocityX, velocityY) + getAcceleration();
	setVelocity(newVelocity);
	setVelocity(Util::limitMagnitude(getVelocity(), m_maxSpeed));


	glm::vec2 newPosition = getPosition() + getVelocity();
	setPosition(newPosition);

}

void ship::m_checkBounds()
{
	//float distance = Util::distance(getPosition(), TheGame::Instance()->getTargetPosition());
	////std::cout << "distance: " << distance << std::endl;
	//if(distance <= 5.0f)
	//{
	//	setState(State::IDLE);
	//	
	//}

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
	setIsColliding(false);
	int halfWidth = getWidth() * 0.5;
	int xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	int yComponent = -getHeight();
	setPosition(glm::vec2(xComponent, yComponent));
}

void ship::m_seek()
{
	m_target = TheGame::Instance()->getTargetPosition();
	glm::vec2 desired = Util::normalize(m_target - getPosition()) * m_maxSpeed;
	glm::vec2 steer = (desired - getVelocity());

	steer = Util::limitMagnitude(steer, m_steerForce);
	setAcceleration(steer);


	glm::vec2 targetDirection = m_target - getPosition();
	
	if (Util::squaredMagnitude(targetDirection) > 0.0f) 
	{
		targetDirection = Util::normalize(targetDirection);
	}



	////glm::vec2 offset = m_target;
	double desiredDirection;
	if (targetDirection.x * targetDirection.x + targetDirection.y * targetDirection.y > 0) 
	{
		desiredDirection = atan2(-targetDirection.x, -targetDirection.y);
		
		desiredDirection *= Util::Rad2Deg; // convert to degrees
		
		desiredDirection +=90.0f;

		
		std::cout << "Desired Direction : " << desiredDirection << std::endl;

		/*std::cout << "=============================" << std::endl;
		std::cout << "Current Direction : " << m_currentDirection << std::endl;
		std::cout << "Desired Direction : " << desiredDirection << std::endl;
		std::cout << "Desired Direction - current: " << desiredDirection - m_currentDirection << std::endl;
		std::cout << "Current Velocity-X : " << getVelocity().x << std::endl;
		std::cout << "Current Velocity-Y : " << getVelocity().y << std::endl;
		std::cout << "=============================" << std::endl;*/

		if (desiredDirection - m_currentDirection > 0)
		{
			turnRight();
		}
		else if (desiredDirection - m_currentDirection < 0)
		{
			turnLeft();
		}

	//	move();
	}
	//else {
	//	desiredDirection = 0.0;
	//	m_currentDirection = desiredDirection;
	//}
}
