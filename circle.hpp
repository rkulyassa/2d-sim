#pragma once

#include <SFML/Graphics.hpp>
#include "./utils.hpp"

struct Circle {
    float x, y, dx, dy, radius;
    sf::Color color;

    Circle() {
        x = rand() % 800;
        y = rand() % 800;
        dy = rand() % 20 - 10;
        dx = rand() % 20 - 10;
        // radius = 50;
        radius = rand() % 50 + 50;
        color = getRandomColor();
    }

    sf::CircleShape getCircleShape() {
        sf::CircleShape circleShape(radius);
        circleShape.setFillColor(color);
        circleShape.setPosition(x - radius, y - radius);
        return circleShape;
    }

    float getMass() {
        return 3.14 * radius * radius;
    }
};