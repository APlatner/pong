#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cstdio>
#include <stdexcept>
#include <string>

class Pong {
public:
	void Startup() {
		// load font
		if (!font.loadFromFile(
		        "res/NotoSansMono-Regular-Nerd-Font-Complete.ttf")) {
			std::runtime_error("Failed to load font from file!");
			return;
		}

		// setup text
		menu.setFont(font);
		menu.setString("'N' to start a new game, and 'ESC' to quit");
		menu.setCharacterSize(24);
		menu.setFillColor(sf::Color::White);
		menu.setStyle(sf::Text::Bold);
		menu.setPosition(sf::Vector2f(75.0, 250.0));

		winner.setFont(font);
		winner.setCharacterSize(24);
		winner.setFillColor(sf::Color::White);
		winner.setStyle(sf::Text::Bold);
		winner.setPosition(sf::Vector2f(75.0, 200.0));

		title.setFont(font);
		title.setString("Pong");
		title.setCharacterSize(24);
		title.setFillColor(sf::Color::White);
		title.setStyle(sf::Text::Bold);
		title.setPosition(sf::Vector2f(375.0, 0.0));

		player1.setFont(font);
		player1.setString("Player 1 | 0");
		player1.setCharacterSize(24);
		player1.setFillColor(sf::Color::White);
		player1.setStyle(sf::Text::Bold);
		player1.setPosition(sf::Vector2f(0.0, 0.0));

		player2.setFont(font);
		player2.setString("Player 2 | 0");
		player2.setCharacterSize(24);
		player2.setFillColor(sf::Color::White);
		player2.setStyle(sf::Text::Bold);
		player2.setPosition(sf::Vector2f(600.0, 0.0));

		// setup game objects
		board.setOutlineThickness(2.0);
		board.setOutlineColor(sf::Color::White);
		board.setFillColor(sf::Color::Black);
		board.setSize(sf::Vector2f(796.0, 568.0));
		board.setPosition(sf::Vector2f(2.0, 30.0));

		ball.setFillColor(sf::Color::White);
		ball.setRadius(5.0);
		ball.setPosition(sf::Vector2f(400, 300));

		paddle1.setSize(sf::Vector2f(5.0, 50.0));
		paddle1.setPosition(sf::Vector2f(8.0, 568.0 / 2));

		paddle2.setSize(sf::Vector2f(5.0, 50.0));
		paddle2.setPosition(sf::Vector2f(792.0 - 5.0, 568.0 / 2));
	}

	void MenuLoop() {
		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					case sf::Keyboard::N:
						Gameloop();
						break;
					default:
						break;
					}
				}
			}

			RenderMenu();
		}
	}

	void Gameloop() {
		int maxScore = 10;
		int player1Score = 0;
		int player2Score = 0;
		float paddle1Speed = 0.0f;
		float paddle2Speed = 0.0f;
		float ballSpeed = 3.0f;
		sf::Vector2f ballDirection(1.0f, 1.0f);

		while (window.isOpen()) {
			time = clock.restart();
			physicsTime += time;

			// Physics
			if (physicsTime.asMicroseconds() >=
			    (1000000.0f / 60.0f)) {
				physicsTime = physicsTime.Zero;
				ball.setPosition(ball.getPosition() +
				                 ballDirection * ballSpeed);
				if (paddle1.getPosition().y <
				        board.getPosition().y +
				            board.getOutlineThickness() &&
				    paddle1Speed < 0) {
					paddle1Speed = 0.0f;
				} else if (paddle1.getPosition().y +
				                   paddle1.getSize().y >
				               board.getPosition().y +
				                   board.getSize().y -
				                   board
				                       .getOutlineThickness() &&
				           paddle1Speed > 0) {
					paddle1Speed = 0.0f;
				}

				if (paddle2.getPosition().y <
				        board.getPosition().y +
				            board.getOutlineThickness() &&
				    paddle2Speed < 0) {
					paddle2Speed = 0.0f;
				} else if (paddle2.getPosition().y +
				                   paddle2.getSize().y >
				               board.getPosition().y +
				                   board.getSize().y -
				                   board
				                       .getOutlineThickness() &&
				           paddle2Speed > 0) {
					paddle2Speed = 0.0f;
				}

				paddle1.setPosition(paddle1.getPosition().x,
				    paddle1.getPosition().y + paddle1Speed);
				paddle2.setPosition(paddle2.getPosition().x,
				    paddle2.getPosition().y + paddle2Speed);
			}

			if (ball.getPosition().y < board.getPosition().y) {
				ballDirection.y = 1.0f;
			} else if (ball.getPosition().y + ball.getRadius() >
			           board.getPosition().y + board.getSize().y) {
				ballDirection.y = -1.0f;
			}

			if (ball.getPosition().x < board.getPosition().x) {
				ball.setPosition(sf::Vector2f(400, 300));
				player2.setString(
				    "Player 2 | " +
				    std::to_string(++player2Score));
			} else if (ball.getPosition().x >
			           board.getPosition().x + board.getSize().x) {
				player1.setString(
				    "Player 1 | " +
				    std::to_string(++player1Score));
				ball.setPosition(sf::Vector2f(400, 300));
			}

			if ((ball.getPosition().x + ball.getRadius() >=
			        paddle2.getPosition().x) &&
			    (ball.getPosition().y > paddle2.getPosition().y &&
			        ball.getPosition().y <
			            paddle2.getPosition().y +
			                paddle2.getSize().y)) {
				ballDirection.x = -1.0f;
			} else if ((ball.getPosition().x + ball.getRadius() <
			               paddle1.getPosition().x +
			                   paddle1.getSize().x) &&
			           (ball.getPosition().y >
			                   paddle1.getPosition().y &&
			               ball.getPosition().y <
			                   paddle1.getPosition().y +
			                       paddle1.getSize().y)) {
				ballDirection.x = 1.0f;
			}

			// Events and user input
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
					case sf::Keyboard::W:
						paddle1Speed = -3.0f;
						break;
					case sf::Keyboard::S:
						paddle1Speed = 3.0f;
						break;
					case sf::Keyboard::Up:
						paddle2Speed = -3.0f;
						break;
					case sf::Keyboard::Down:
						paddle2Speed = 3.0f;
						break;
					default:
						break;
					}
				}
				if (event.type == sf::Event::KeyReleased) {
					switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
					case sf::Keyboard::W:
					case sf::Keyboard::S:
						paddle1Speed = 0.0f;
						break;
					case sf::Keyboard::Up:
					case sf::Keyboard::Down:
						paddle2Speed = 0.0f;
						break;
					default:
						break;
					}
				}
			}

			// Render objects
			RenderGame();

			if (player1Score == maxScore) {
				winner.setString("Player 1 won!");
				return;
			} else if (player2Score == maxScore) {
				winner.setString("Player 2 won!");
				return;
			}
		}
	}

	void RenderMenu() {
		window.clear(sf::Color::Black);

		window.draw(title);
		window.draw(player1);
		window.draw(player2);
		window.draw(board);
		window.draw(ball);
		window.draw(paddle1);
		window.draw(paddle2);
		window.draw(menu);
		window.draw(winner);
		window.display();
	}

	void RenderGame() {
		window.clear(sf::Color::Black);

		window.draw(title);
		window.draw(player1);
		window.draw(player2);
		window.draw(board);
		window.draw(ball);
		window.draw(paddle1);
		window.draw(paddle2);

		window.display();
	}

private:
	sf::RenderWindow window =
	    sf::RenderWindow(sf::VideoMode(800, 600), "Pong");
	sf::Font font;

	sf::Text menu;
	sf::Text winner;
	sf::Text title;
	sf::Text player1;
	sf::Text player2;

	sf::RectangleShape board;
	sf::CircleShape ball;
	sf::RectangleShape paddle1;
	sf::RectangleShape paddle2;

	sf::Clock clock;
	sf::Time time;
	sf::Time physicsTime;
};

int main() {
	Pong pong;
	pong.Startup();
	pong.MenuLoop();

	return 0;
}
