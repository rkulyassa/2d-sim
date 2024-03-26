#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>

sf::Color getRandomColor() {
    // static bool initialized = false;
    // if (!initialized) {
    //     std::srand(std::time(nullptr));
    //     initialized = true;
    // }
    return sf::Color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
}

struct Circle {
    float x, y, dx, dy, radius;
    sf::Color color;

    Circle () {
        x = rand() % 800;
        // y = rand() % 800;
        y = 0;
        // dx = dy = 0;
        dy = 10;
        dx = rand() % 20 - 10;
        radius = 50;
        color = getRandomColor();
    }

    sf::CircleShape getCircleShape() {
        sf::CircleShape circleShape(radius);
        circleShape.setFillColor(color);
        circleShape.setPosition(x, y);
        return circleShape;
    }

};

struct World {
    static constexpr float gravity = 9.81;
    static constexpr float energyLossFactor = 0.8;
    static constexpr float surfaceFrictionFactor = 0.95;
    std::vector<Circle> circles;

    void update() {
        for (Circle& circle : circles) {
            // std::cout << circle.y << " " << circle.dy << std::endl;
            circle.dy += gravity;
            circle.x += circle.dx;
            circle.y += circle.dy;
            if (circle.y + circle.radius*2 >= 800) {
                circle.y = 800 - circle.radius*2;
                circle.dy = -circle.dy * energyLossFactor;
                circle.dx *= surfaceFrictionFactor;
            }
            if (circle.x <= 0 || circle.x + circle.radius*2 > 800) {
                circle.dx = -circle.dx;
            }
            if (circle.x <= 0) {
                circle.x = 0;
            }
            if (circle.x + circle.radius*2 > 800) {
                circle.x = 800 - circle.radius*2;
            }
        }
    }
};

int main() {
    World world;

    sf::RenderWindow window(sf::VideoMode(800, 800), "2d Physics Simulation");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        world.update();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    Circle circle;
                    world.circles.push_back(circle);
                }
            }
        }
        window.clear(sf::Color::Black);
        for (Circle circle : world.circles) {
            window.draw(circle.getCircleShape());
        }
        window.display();
    }
}