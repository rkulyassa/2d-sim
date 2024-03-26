#include <iostream>
#include <SFML/Graphics.hpp>
#include "./world.hpp"
#include "./circle.hpp"

int main() {
    World world;

    sf::RenderWindow window(sf::VideoMode(800, 800), "2d Physics Simulation");
    window.setFramerateLimit(30);

    while (window.isOpen()) {
        world.update();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) {
                    Circle circle;
                    world.circles.push_back(circle);
                }
                if (event.key.code == sf::Keyboard::R) {
                    world.circles.clear();
                }
            }
        }
        window.clear(sf::Color::Black);
        for (Circle& circle : world.circles) {
            window.draw(circle.getCircleShape());
        }
        window.display();
    }
}