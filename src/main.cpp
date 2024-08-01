#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "Vector2D.h"
#include "Body.h"

double TIME_STEP = 43200; // Half a day in seconds
const double MIN_SPEED = 86400 * 1; // 1 day in seconds
const double MAX_SPEED = 86400 * 100; // 10 days in seconds

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
}

double calculateSpeedFromSlider(double sliderPosition, double sliderLength) {
    double normalizedPosition = sliderPosition / sliderLength;
    return MIN_SPEED * std::pow(MAX_SPEED / MIN_SPEED, normalizedPosition);
}

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Solar System Simulator");
    window.setFramerateLimit(60); // Set the frame rate limit

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

    // Create slider
    sf::RectangleShape sliderTrack(sf::Vector2f(200, 10));
    sliderTrack.setPosition(550, 730);
    sliderTrack.setFillColor(sf::Color::White);

    sf::RectangleShape sliderKnob(sf::Vector2f(20, 30));
    sliderKnob.setPosition(650, 715);
    sliderKnob.setFillColor(sf::Color::Green);

    bool isDragging = false;

    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (sliderKnob.getGlobalBounds().contains(mousePos)) {
                    isDragging = true;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                isDragging = false;
            }
            if (event.type == sf::Event::MouseMoved && isDragging) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                float newPosX = std::clamp(mousePos.x, sliderTrack.getPosition().x, sliderTrack.getPosition().x + sliderTrack.getSize().x - sliderKnob.getSize().x);
                sliderKnob.setPosition(newPosX, sliderKnob.getPosition().y);
                TIME_STEP = calculateSpeedFromSlider(newPosX - sliderTrack.getPosition().x, sliderTrack.getSize().x - sliderKnob.getSize().x);
            }
        }

        // Simulate
        simulate(bodies, 1);

        // Draw everything
        window.clear();
        drawBodies(window, bodies);

        // Draw slider
        window.draw(sliderTrack);
        window.draw(sliderKnob);

        window.display();
    }

    return 0;
}
