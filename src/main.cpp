#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include "Body.h"

double TIME_STEP = 43200; // Half a day in seconds
const double SPEED_FACTOR = 10;

void simulate(std::vector<Body>& bodies, int steps) {
    for (int step = 0; step < steps; ++step) {
        std::vector<Vector2D> forces(bodies.size());

        // Calculate forces
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = 0; j < bodies.size(); ++j) {
                if (i != j) {
                    forces[i] = forces[i] + bodies[i].gravitationalForce(bodies[j]);
                }
            }
        }

        // Update velocities and positions
        for (size_t i = 0; i < bodies.size(); ++i) {
            Vector2D acceleration = forces[i] * (1.0 / bodies[i].mass);
            bodies[i].updateVelocity(acceleration, TIME_STEP);
            bodies[i].updatePosition(TIME_STEP);
        }
    }
}

void drawBodies(sf::RenderWindow& window, const std::vector<Body>& bodies) {
    window.clear();
    for (const auto& body : bodies) {
        sf::CircleShape shape(body.mass > 1e26 ? 10 : 5); // Sun is bigger
        shape.setPosition(
            (window.getSize().x / 2) + body.position.x / 1.5e9, 
            (window.getSize().y / 2) + body.position.y / 1.5e9
        );

        if (body.mass > 1e26) {
            shape.setFillColor(sf::Color::Yellow); // Sun
        } else {
            shape.setFillColor(sf::Color::Blue); // Earth
        }

        window.draw(shape);
    }
    window.display();
}

bool isButtonClicked(const sf::RectangleShape& button, sf::Vector2f mousePos) {
    return button.getGlobalBounds().contains(mousePos);
}

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Solar System Simulator");

    // Initialize bodies
    Body sun(Vector2D(0, 0), Vector2D(0, 0), 1.989e30);
    Body earth(Vector2D(1.496e11, 0), Vector2D(0, 29783), 5.972e24);

    std::vector<Body> bodies = { sun, earth };

    // Load font
    sf::Font font;
    if (!font.loadFromFile("../fonts/Roboto-Bold.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    // Create buttons
    sf::RectangleShape speedUpButton(sf::Vector2f(120, 50));
    speedUpButton.setPosition(650, 700);
    speedUpButton.setFillColor(sf::Color::Green);

    sf::Text speedUpText("Speed Up", font, 20);
    speedUpText.setPosition(660, 715);

    sf::RectangleShape slowDownButton(sf::Vector2f(120, 50));
    slowDownButton.setPosition(500, 700);
    slowDownButton.setFillColor(sf::Color::Red);

    sf::Text slowDownText("Slow Down", font, 20);
    slowDownText.setPosition(510, 715);

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (isButtonClicked(speedUpButton, mousePos)) {
                    TIME_STEP *= SPEED_FACTOR;
                } else if (isButtonClicked(slowDownButton, mousePos)) {
                    TIME_STEP /= SPEED_FACTOR;
                }
            }
        }

        // Simulate and draw
        simulate(bodies, 1);
        drawBodies(window, bodies);

        // Draw buttons
        window.draw(speedUpButton);
        window.draw(speedUpText);
        window.draw(slowDownButton);
        window.draw(slowDownText);

        window.display();
    }

    return 0;
}
