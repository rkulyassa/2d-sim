#pragma once

#include <iostream>
#include "./circle.hpp"
#include "./utils.hpp"

struct World {
    static constexpr float windowX = 800;
    static constexpr float windowY = 800;

    // static constexpr float gravity = 0;
    // static constexpr float wallCollisionEnergyLossFactor = 1;
    // static constexpr float circleCollisionEnergyLossFactor = 1;
    // static constexpr float surfaceFrictionFactor = 1;
    static constexpr float gravity = 9.81;
    static constexpr float generalAccelerationFactor = 0.1;
    static constexpr float wallCollisionEnergyLossFactor = 0.8;
    static constexpr float circleCollisionEnergyLossFactor = 0.6;
    static constexpr float surfaceFrictionFactor = 0.95;

    std::vector<Circle> circles;

    void update(sf::Vector2i mousePos) {
        for (Circle& circle : circles) {
            if (circle.isDragging) {
                circle.x = mousePos.x;
                circle.y = mousePos.y;
            };
            float dx = windowX/2 - circle.x;
            float dy = windowY/2 - circle.y;
            float d = sqrt(dx*dx + dy*dy);
            circle.dx = dx * generalAccelerationFactor;
            circle.dy = dy * generalAccelerationFactor;
            // circle.dy += gravity;
            circle.x += circle.dx;
            circle.y += circle.dy;
            resolveWallCollision(circle);
        }
        for (int i = 0; i < 8; i++) {
            for (Circle& a : circles) {
                for (Circle& b : circles) {
                    if (&a == &b) continue;
                    resolveCollision(a, b);
                }
            }
        }
    }

    bool areIntersecting(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = sqrt(dx*dx + dy*dy);
        return d <= (a.radius + b.radius);
    }

    bool areTouching(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = sqrt(dx*dx + dy*dy);
        int r = round(a.radius + b.radius);
        return d <= r + 2;
    }

    void resolveWallCollision(Circle& c) {
        if (c.x - c.radius <= 0) {
            c.x = c.radius;
            c.dx = -c.dx;
        }
        if (c.x + c.radius > 800) {
            c.x = 800 - c.radius;
            c.dx = -c.dx;
        }
        if (c.y - c.radius <= 0) {
            c.y = c.radius;
            c.dy = -c.dy;
        }
        if (c.y + c.radius >= 800) {
            c.y = 800 - c.radius;
            c.dy = -c.dy * wallCollisionEnergyLossFactor;
            c.dx *= surfaceFrictionFactor;
        }
    }

    void resolveCollision(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx*dx + dy*dy);
        float r = a.radius + b.radius;
        if (d >= r) return;
        // float m = a.getMass() + b.getMass();
        float lerp = (r - d)/2;
        a.x -= dx/d * lerp;// * b.getMass()/m;
        a.y -= dy/d * lerp;// * b.getMass()/m;
        b.x += dx/d * lerp;// * a.getMass()/m;
        b.y += dy/d * lerp;// * a.getMass()/m;
    }
};