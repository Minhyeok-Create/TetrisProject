#include "Game.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <algorithm>
#include <iostream>


Game::Game()
	: scoreText(font), levelText(font) 
{
	if (!font.openFromFile("assets/NotoSansKR-Regular.ttf")) {
		std::cerr << "폰트 로드 오류" << std::endl;
	}


	scoreText.setString(sf::String(L"점수 : 0"));
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(sf::Vector2f(320, 30));

	levelText.setString(sf::String(L"레벨 : 1"));
	levelText.setCharacterSize(24);
	levelText.setFillColor(sf::Color::White);
	levelText.setPosition(sf::Vector2f(320, 60));
}




void Game::run() {
	current = Tetromino();
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }, 32), "SFML 3.0.0 테트리스");
	window.setFramerateLimit(60);

	sf::Text gameOverText(font);
	gameOverText.setString(L"게임 오버!\n스페이스바로 재시작");
	gameOverText.setCharacterSize(32);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(sf::Vector2f(100, 200));



	auto lastFall = std::chrono::steady_clock::now();

	while (window.isOpen()) {
		Tetromino ghost = current;
		while (true) {
			Tetromino next = ghost;
			next.y++;
			if (board.checkCollision(next)) break;
			ghost.y++;
		}
		while (const std::optional<sf::Event> event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();



			if (event->is<sf::Event::KeyPressed>()) {
				auto key = event->getIf<sf::Event::KeyPressed>()->code;

				Tetromino temp = current;

				if (key == sf::Keyboard::Key::Left) {
					temp.x--;
					if (!board.checkCollision(temp)) current.x--;
				}
				else if (key == sf::Keyboard::Key::Right) {
					temp.x++;
					if (!board.checkCollision(temp)) current.x++;
				}
				else if (key == sf::Keyboard::Key::Up) {
					Tetromino rotated = current;
					rotated.rotate();
					for (int dx = -1; dx <= 1; ++dx) {
						Tetromino tryMove = rotated;
						tryMove.x += dx;
						if (!board.checkCollision(tryMove)) {
							current = tryMove;
							break;
						}
					}
				}
				else if (key == sf::Keyboard::Key::Down) {
					Tetromino dropped = current;
					while (true) {
						Tetromino next = dropped;
						next.y++;
						if (board.checkCollision(next)) break;
						dropped.y++;
					}
					current = dropped;
					board.placeTetromino(current);
				}
				else if (key == sf::Keyboard::Key::Escape) {
					window.close();
				}

				if (isGameOver && key == sf::Keyboard::Key::Space) {
					board = Board();
					current = Tetromino();
					score = 0;
					level = 1;
					speed = 500;
					isGameOver = false;

					scoreText.setString(L"점수 : 0");
					levelText.setString(L"레벨 : 1");
				}
			}
		}
		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFall);
		if (duration.count() > speed) {
			Tetromino next = current;
			next.y++;

			if (!board.checkCollision(next)) {
				current.y++;
			}
			else {
				board.placeTetromino(current);
				int lines = board.clearFullLines();
				switch (lines) {
				case 1: score += 100; break;
				case 2: score += 300; break;
				case 3: score += 500; break;
				case 4: score += 800; break;
				default: break;
				}
				level = score / 1000 + 1;
				scoreText.setString(sf::String(L"점수 : ") + std::to_wstring(score));
				levelText.setString(sf::String(L"레벨 : ") + std::to_wstring(level));

				int fallSpeed = 500 - (level - 1) * 40;
				speed = std::max(100, fallSpeed);

				current = Tetromino();

				if (board.checkCollision(current)) {
					isGameOver = true;
				}
				if (isGameOver) {
					window.draw(gameOverText);
					window.display();
					continue;
				}
			}
			lastFall = std::chrono::steady_clock::now();
		}
		window.clear(sf::Color::Black);
		board.draw(window, current, ghost, score, level);
		window.draw(scoreText);
		window.draw(levelText);
		window.display();

	}

}