#include <iostream>
#include <SFML/Graphics.hpp>
#include "./world.hpp"
#include "./circle.hpp"

sf::Font font;

sf::Text generateText() {
    sf::Text text;
    font.loadFromFile("./Courier.ttf");
    text.setFont(font);
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(0.5);
    text.setOutlineColor(sf::Color::White);
    return text;
}

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
        sf::Text circleCount = generateText();
        circleCount.setString("Circles: " + std::to_string(world.circles.size()));
        circleCount.setPosition(10.f, 10.f);
        sf::Text calculationsCount = generateText();
        calculationsCount.setString("Calculations/s: " + std::to_string(world.collisionIterations * world.circles.size() * world.circles.size()));
        calculationsCount.setPosition(10.f, 20.f);
        window.draw(circleCount);
        window.draw(calculationsCount);
        window.display();
    }
}