#include "Game.h"

Game* Game::s_pInstance = 0;

glm::vec2 Game::getTargetPosition()
{
	return m_pTarget->getPosition();
}

SDL_Renderer * Game::getRenderer()
{
	return m_pRenderer;
}

glm::vec2 Game::getMousePosition()
{
	return m_mousePosition;
}

Game::Game()
{
}

Game::~Game()
{
}

void Game::createGameObjects()
{
	/*m_pPlayer = new Player();
	m_pIsland = new Island();
	m_pOcean = new Ocean();

	for (size_t i = 0; i < m_cloudNum; i++)
	{
		m_pClouds.push_back(new Cloud());
	}
	*/
	m_pShip = new ship();

	m_pTarget = new Target();
}

bool Game::init(const char* title, int xpos, int ypos, int height, int width, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
	{
		std::cout << "SDL Init success" << std::endl;

		// if succeeded create our window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, height, width, flags);

		// if window creation successful create our renderer
		if (m_pWindow != 0)
		{
			std::cout << "window creation success" << std::endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0) // render init success
			{
				std::cout << "renderer creation success" << std::endl;
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "renderer init failure" << std::endl;
				return false; // render int fail
			}

			//TheTextureManager::Instance()->load("../../Assets/textures/animate-alpha.png", "animate", m_pRenderer);
			createGameObjects();
		}
		else 
		{
			std::cout << "window init failure" << std::endl;
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init failure" << std::endl;
		return false; //SDL could not intialize
	}

	std::cout << "init success" << std::endl;
	m_bRunning = true; // everything initialized successfully - start the main loop

	return true;
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

	/*m_pOcean->draw();
	m_pIsland->draw();
	m_pPlayer->draw();

	for (Cloud* cloud : m_pClouds) {
		cloud->draw();
	}
	*/

	m_pTarget->draw();
	m_pShip->draw();

	

	SDL_RenderPresent(m_pRenderer); // draw to the screen
}

void Game::update()
{
	/*m_pPlayer->update();
	m_pIsland->update();
	m_pOcean->update();
	
	Collision::squaredRadiusCheck(m_pPlayer, m_pIsland);

	for (Cloud* cloud : m_pClouds) {
		cloud->update();
		Collision::squaredRadiusCheck(m_pPlayer, cloud);
	}
	*/
	m_pShip->update();

	m_pTarget->update();

}

void Game::clean()
{
	std::cout << "cleaning game" << std::endl;

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

void Game::handleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_MOUSEMOTION:
			m_mousePosition.x = event.motion.x;
			m_mousePosition.y = event.motion.y;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					m_bRunning = false;
					break;
				case SDLK_w:
					m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, -1.0f));
					break;
				case SDLK_s:
					m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 1.0f));
					break;
				
				case SDLK_a:
					m_pTarget->setVelocity(glm::vec2(-1.0f, m_pTarget->getVelocity().y));
					break;
				case SDLK_d:
					m_pTarget->setVelocity(glm::vec2(1.0f, m_pTarget->getVelocity().y));
					break;
				case SDLK_0:
					m_pShip->setState(SteeringState::IDLE);
					break;
				case SDLK_1:
					m_pShip->setState(SteeringState::SEEK);
					break;
				case SDLK_2:
					m_pShip->setState(SteeringState::ARRIVE);
					break;
				case SDLK_3:
					m_pShip->setState(SteeringState::AVOID);
					break;
				case SDLK_4:
					m_pShip->setState(SteeringState::FLEE);
					break;
				case SDLK_RIGHT:
					m_pShip->turnRight();
					break;
				case SDLK_LEFT:
					m_pShip->turnLeft();
					break;

			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
				case SDLK_w:
					if (m_pTarget->getVelocity().y < 0.0f) {
						m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 0.0f));
					}
					break;
				
				case SDLK_s:
					if (m_pTarget->getVelocity().y > 0.0f) {
						m_pTarget->setVelocity(glm::vec2(m_pTarget->getVelocity().x, 0.0f));
					}
					break;
				
				case SDLK_a:
					if (m_pTarget->getVelocity().x < 0.0f) {
						m_pTarget->setVelocity(glm::vec2(0.0f, m_pTarget->getVelocity().y));
					}
					break;
				case SDLK_d:
					if (m_pTarget->getVelocity().x > 0.0f) {
						m_pTarget->setVelocity(glm::vec2(0.0f, m_pTarget->getVelocity().y));
					}
					break;
				
			}
			
		default:
			break;
		}
	}
}