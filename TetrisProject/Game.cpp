// Game.cpp - SFML 3.0.0 호환 버전

#include "Game.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <optional>

Game::Game()
    : scoreText(font), levelText(font)
{
    if (!font.openFromFile("assets/NotoSansKR-Regular.ttf")) {
        std::cerr << "폰트 로드 실패\n";
    }

    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(320, 30));

    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(sf::Vector2f(320, 60));
}

void Game::run() {
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(800, 600), 32),  
        "SFML 테트리스",
        sf::Style::Default,
        sf::State::Windowed
    );
    window.setFramerateLimit(60);

    auto lastFall = std::chrono::steady_clock::now();

    // 메뉴 텍스트
    sf::Text title(font, L"테트리스", 48);
    title.setPosition(sf::Vector2f(230, 150));
    title.setFillColor(sf::Color::White);

    sf::Text instructions(font, L"S - 시작\nESC - 종료", 24);
    instructions.setPosition(sf::Vector2f(250, 250));
    instructions.setFillColor(sf::Color::White);

    sf::Text gameOverText(font, L"게임 오버!\n스페이스바로 재시작", 32);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(sf::Vector2f(100, 200));

    while (window.isOpen()) {
        switch (state) {
        case GameState::Menu:
            window.clear();
            window.draw(title);
            window.draw(instructions);
            window.display();

            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (event->is<sf::Event::KeyPressed>()) {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::S) {
                        resetGame();
                        state = GameState::Playing;
                    }
                    else if (key == sf::Keyboard::Key::Escape)
                        window.close();
                }
            }
            break;

        case GameState::GameOver:
            window.clear();
            window.draw(gameOverText);
            window.display();

            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (event->is<sf::Event::KeyPressed>()) {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Space)
                        state = GameState::Menu;
                }
            }
            break;

        case GameState::Playing: {
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>())
                    window.close();
                else if (event->is<sf::Event::KeyReleased>()) {
                    auto key = event->getIf<sf::Event::KeyReleased>()->code;
                    if (key == sf::Keyboard::Key::P) pKeyHeld = false;
                }
                else if (event->is<sf::Event::KeyPressed>()) {
                    auto key = event->getIf<sf::Event::KeyPressed>()->code;
                    if (key == sf::Keyboard::Key::Escape) window.close();
                    if (key == sf::Keyboard::Key::P && !pKeyHeld) {
                        isPaused = !isPaused;
                        pKeyHeld = true;
                    }
                    if (isPaused) break;

                    Tetromino temp = current;
                    if (key == sf::Keyboard::Key::Left) { temp.x--; if (!board.checkCollision(temp)) current.x--; }
                    else if (key == sf::Keyboard::Key::Right) { temp.x++; if (!board.checkCollision(temp)) current.x++; }
                    else if (key == sf::Keyboard::Key::Up) {
                        temp.rotate();
                        for (int dx = -1; dx <= 1; ++dx) {
                            Tetromino moved = temp; moved.x += dx;
                            if (!board.checkCollision(moved)) { current = moved; break; }
                        }
                    }
                    else if (key == sf::Keyboard::Key::Down) {
                        while (!board.checkCollision(temp)) {
                            current = temp;
                            temp.y++;
                        }
                        board.placeTetromino(current);
                        handleFall();
                    }
                    else if ((key == sf::Keyboard::Key::C || key == sf::Keyboard::Key::LShift) && !holdUsedThisTurn) {
                        if (!hasHold) {
                            hold = current;
                            current = next;
                            next = Tetromino();
                            hasHold = true;
                        }
                        else {
                            std::swap(current, hold);
                        }
                        current.x = 3; current.y = 0;
                        holdUsedThisTurn = true;
                    }
                }
            }

            if (isPaused) {
                window.clear();
                drawPause(window);
                window.display();
                break;
            }

            Tetromino ghost = current;
            while (true) {
                Tetromino nextGhost = ghost;
                nextGhost.y++;
                if (board.checkCollision(nextGhost)) break;
                ghost.y++;
            }
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFall).count() > speed) {
            fall:
                Tetromino moved = current;
                moved.y++;
                if (!board.checkCollision(moved)) {
                    current.y++;
                }
                else {
                    board.placeTetromino(current);
                    int lines = board.clearFullLines();
                    switch (lines) {
                    case 1: score += 100; break;
                    case 2: score += 300; bonusMessage = L"더블!"; showBonus = true; bonusTimer = now; break;
                    case 3: score += 500; bonusMessage = L"트리플!"; showBonus = true; bonusTimer = now; break;
                    case 4: score += 800; bonusMessage = L"TETRIS!"; showBonus = true; bonusTimer = now; break;
                    }

                    level = score / 1000 + 1;
                    speed = std::max(100, 500 - (level - 1) * 40);
                    scoreText.setString(sf::String(L"점수 : ") + std::to_wstring(score));
                    levelText.setString(sf::String(L"레벨 : ") + std::to_wstring(level));

                    if (board.checkCollision(next)) {
                        state = GameState::GameOver;
                        break;
                    }

                    current = next;
                    next = Tetromino();
                    holdUsedThisTurn = false;
                }
                lastFall = now;
            }

            window.clear();
            board.draw(window, current, ghost, score, level);
            window.draw(scoreText);
            window.draw(levelText);
            drawNext(window);
            drawHold(window);
            drawBonus(window);
            window.display();
            break;
        }
        }
    }
}
void Game::handleFall() {
    Tetromino moved = current;
    moved.y++;

    if (!board.checkCollision(moved)) {
        current.y++;
    }
    else {
        board.placeTetromino(current);
        auto now = std::chrono::steady_clock::now();
        int lines = board.clearFullLines();

        switch (lines) {
        case 1: score += 100; break;
        case 2: score += 300; bonusMessage = L"더블!"; showBonus = true; bonusTimer = now; break;
        case 3: score += 500; bonusMessage = L"트리플!"; showBonus = true; bonusTimer = now; break;
        case 4: score += 800; bonusMessage = L"TETRIS!"; showBonus = true; bonusTimer = now; break;
        }

        level = score / 1000 + 1;
        speed = std::max(100, 500 - (level - 1) * 40);
        scoreText.setString(sf::String(L"점수 : ") + std::to_wstring(score));
        levelText.setString(sf::String(L"레벨 : ") + std::to_wstring(level));

        if (board.checkCollision(next)) {
            state = GameState::GameOver;
            return;
        }

        current = next;
        next = Tetromino();
        holdUsedThisTurn = false;
    }
}
void Game::drawNext(sf::RenderWindow& window) {
    const float blockSize = 30.f;
    sf::RectangleShape block({ blockSize, blockSize });
    block.setOutlineThickness(1.f);
    block.setOutlineColor(sf::Color::Black);
    block.setFillColor(getColorByType(next.type));

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (next.shape[i][j] == '#') {
                block.setPosition({ 600.f + j * blockSize, 100.f + i * blockSize });
                window.draw(block);
            }
        }
    }

    sf::Text label{ font, L"다음", 24 };
    label.setFillColor(sf::Color::White);
    label.setPosition({ 600.f, 60.f });
    window.draw(label);
}
void Game::resetGame() {
    board = Board();
    current = Tetromino();
    next = Tetromino();
    score = 0;
    level = 1;
    speed = 500;
    hasHold = false;
    holdUsedThisTurn = false;
    isPaused = false;
    isGameOver = false;

    scoreText.setString(L"점수 : 0");
    levelText.setString(L"레벨 : 1");
    scoreText.setPosition({ 320.f, 30.f });
    levelText.setPosition({ 320.f, 60.f });
}
void Game::drawHold(sf::RenderWindow& window) {
    if (!hasHold) return;

    const float blockSize = 30.f;
    sf::RectangleShape block({ blockSize, blockSize });
    block.setOutlineThickness(1.f);
    block.setOutlineColor(sf::Color::Black);
    block.setFillColor(getColorByType(hold.type));

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (hold.shape[i][j] == '#') {
                block.setPosition({ 600.f + j * blockSize, 250.f + i * blockSize });
                window.draw(block);
            }
        }
    }

    sf::Text label{ font, L"보류", 24 };
    label.setFillColor(sf::Color::White);
    label.setPosition({ 600.f, 220.f });
    window.draw(label);
}
void Game::drawBonus(sf::RenderWindow& window) {
    if (!showBonus) return;

    auto now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - bonusTimer).count() < 1000) {
        sf::Text bonus{ font, bonusMessage, 28 };
        bonus.setFillColor(sf::Color::Cyan);
        bonus.setPosition({ 320.f, 100.f });
        window.draw(bonus);
    }
    else {
        showBonus = false;
    }
}
void Game::drawPause(sf::RenderWindow& window) {
    sf::Text pause{ font, L"일시정지\nP 키로 계속", 32 };
    pause.setFillColor(sf::Color::Yellow);
    pause.setPosition({ 200.f, 250.f });
    window.draw(pause);
}

