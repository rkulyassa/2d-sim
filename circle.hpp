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
        //x = y = 400;
        // dy = rand() % 20 - 10;
        // dx = rand() % 20 - 10;
        dx = dy = 0;
        radius = 30;
        // radius = rand() % 60 + 30;
        color = getRandomColor();
        // color = sf::Color::Blue;
    }

    sf::CircleShape getShape() {
        sf::CircleShape circleShape(radius);
        circleShape.setFillColor(color);
        circleShape.setPosition(x - radius, y - radius);
        return circleShape;
    }

    sf::CircleShape getBorder() {
        int size = 2;
        sf::CircleShape circleShape(radius + size);
        circleShape.setFillColor(sf::Color::Red);
        circleShape.setPosition(x - radius - size, y - radius - size);
        return circleShape;
    }

    float getMass() {
        return 3.14 * radius * radius;
    }
};