#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP
#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Snake.hpp"
#include <random>
#include <future>
#include "Client.hpp"


class GameLoop {
public:
	GameLoop(bool isMultiplayer) : m_isMultiplayer{ isMultiplayer }, m_isGameOver{ false }, m_score{ 0 } {
		m_font.loadFromFile("ARCADECLASSIC.TTF");
		m_food.setFillColor(sf::Color::White);
		std::mt19937 gen32;
		gen32.seed(time(nullptr));
		uint16_t x = 0;
		uint16_t y = 0;
		do {
			x = ((gen32() - DEFAULT_SNAKE_SIZE_X) % WIDTH);
			y = ((gen32() - DEFAULT_SNAKE_SIZE_Y) % HEIGHT);
		} while (x == DEFAULT_SNAKE_X && y == DEFAULT_SNAKE_Y);
		m_food.setPosition(x, y);
		m_food.setSize(sf::Vector2f(DEFAULT_SNAKE_SIZE_X, DEFAULT_SNAKE_SIZE_Y));
		if (isMultiplayer) {
			m_client = std::make_shared<Client>();
			if (m_client->operator bool()) {
				isMultiplayer = false; // We failed to get online
			}
			else {
				std::async([](std::weak_ptr<Client> weakClient) {
					if (auto client = weakClient.lock()) {
						client->updateScore(0);
					}
				}, m_client);
			}
		}

	}
	~GameLoop() = default;

	void render(sf::RenderWindow& window);
	void processEvent(sf::RenderWindow& window, sf::Event& event);
	void moveSnake();

	bool isGameOver() const;

private:
	bool m_isMultiplayer;
	uint64_t m_score;
	uint64_t m_rank = UINT64_MAX;
	uint64_t m_particapantCount = 1;
	bool m_isGameOver;
	Snake m_snake;
	sf::Font m_font;
	sf::RectangleShape m_food;
	std::shared_ptr<Client> m_client = nullptr;
	

};

#endif