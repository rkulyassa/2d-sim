#pragma once
#include <SFML/Graphics.hpp>
#include "./utils.hpp"

struct Circle {
    float x, y, dx, dy, radius;
    bool isDragging = false;
    sf::Color color;

    Circle() {
        x = rand() % 800;
        y = rand() % 800;
        //x = y = 400;
        // dy = rand() % 20 - 10;
        // dx = rand() % 20 - 10;
        dx = dy = 0;
        // radius = 12;
        // radius = rand() % 10 + 5;
        radius = 12;
        // radius = rand() % 20 + 10;
        // radius = rand() % 40 + 20;
        color = getRandomColor();
        // color = sf::Color::Blue;
    }

    sf::CircleShape getShape(int borderWidth) {
        sf::CircleShape circleShape(radius - borderWidth);
        circleShape.setFillColor(color);
        circleShape.setPosition(x - radius + borderWidth, y - radius + borderWidth);
        return circleShape;
    }

    sf::CircleShape getBorder() {
        sf::CircleShape circleShape(radius);
        circleShape.setFillColor(sf::Color::Red);
        circleShape.setPosition(x - radius, y - radius);
        return circleShape;
    }

    float getMass() {
        return 3.14 * radius * radius;
    }
};