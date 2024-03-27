#include <iostream>
#include <SFML/Graphics.hpp>
#include "./world.hpp"
#include "./circle.hpp"

int main() {
    World world;
    bool showInterCircleVectorsAll = false;
    bool showInterCircleVectorsTouching = false;
    bool showCircleBorders = false;

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
                        showInterCircleVectorsAll = !showInterCircleVectorsAll;
                    }
                    if (event.key.code == sf::Keyboard::T) {
                        showInterCircleVectorsTouching = !showInterCircleVectorsTouching;
                    }
                    if (event.key.code == sf::Keyboard::Y) {
                        showCircleBorders = !showCircleBorders;
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
            if (showCircleBorders) {
                window.draw(circle.getBorder());
            }
            window.draw(circle.getShape());
            if (showInterCircleVectorsAll) {
                for (Circle& other : world.circles) {
                    sf::VertexArray line(sf::Lines, 2);
                    line[0].position = sf::Vector2f(circle.x, circle.y);
                    line[1].position = sf::Vector2f(other.x, other.y);
                    line[0].color = sf::Color::Red;
                    line[1].color = sf::Color::Red;
                    window.draw(line);
                }
            }
            if (showInterCircleVectorsTouching) {
                for (Circle& other : world.circles) {
                    if (world.areTouching(circle, other)) {
                        sf::VertexArray line(sf::Lines, 2);
                        line[0].position = sf::Vector2f(circle.x, circle.y);
                        line[1].position = sf::Vector2f(other.x, other.y);
                        line[0].color = sf::Color::Blue;
                        line[1].color = sf::Color::Blue;
                        window.draw(line);
                    }
                }
            }
        }
        window.display();
    }
}