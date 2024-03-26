#include <iostream>
#include <SFML/Graphics.hpp>
#include "./world.hpp"
#include "./circle.hpp"

int main() {
    World world;
    bool showInterCircleVectors = false;

    sf::RenderWindow window(sf::VideoMode(world.windowX, world.windowY), "2d Physics Simulation");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        world.update(sf::Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::KeyPressed: 
                    if (event.key.code == sf::Keyboard::W) {
                        Circle circle;
                        world.circles.push_back(circle);
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        showInterCircleVectors = !showInterCircleVectors;
                    }
                    if (event.key.code == sf::Keyboard::R) {
                        world.circles.clear();
                    }
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (Circle& circle : world.circles) {
                            if (circle.getShape().getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                                circle.isDragging = true;
                                break;
                            }
                        }
                    }
                    break;
                
                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (Circle& circle : world.circles) {
                            circle.isDragging = false;
                        }
                    }
                    break;
                
                default:
                    break;
            }
        }
        window.clear(sf::Color::Black);
        for (Circle& circle : world.circles) {
            window.draw(circle.getShape());
            if (showInterCircleVectors) {
                for (Circle& other : world.circles) {
                    sf::VertexArray line(sf::Lines, 2);
                    line[0].position = sf::Vector2f(circle.x, circle.y);
                    line[1].position = sf::Vector2f(other.x, other.y);
                    line[0].color = sf::Color::Red;
                    line[1].color = sf::Color::Red;
                    window.draw(line);
                }
            }
        }
        window.display();
    }
}