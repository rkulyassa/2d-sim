#pragma once

#include <SFML/Graphics.hpp>
#include "./utils.hpp"

struct Circle {
    float x, y, dx, dy, radius;
    bool isDragging;
    sf::Color color;

    Circle() {
        x = rand() % 800;
        y = rand() % 800;
        // dy = rand() % 20 - 10;
        // dx = rand() % 20 - 10;
        dx = dy = 0;
        // radius = 50;
        radius = rand() % 25 + 25;
        color = getRandomColor();
    }

    sf::CircleShape getShape() {
        sf::CircleShape circleShape(radius);
        circleShape.setFillColor(color);
        circleShape.setPosition(x - radius, y - radius);
        return circleShape;
    }

    float getMass() {
        return 3.14 * radius * radius;
    }
};