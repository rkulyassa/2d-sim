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
            float d = std::sqrt(dx*dx + dy*dy);
            circle.dx = dx * generalAccelerationFactor;
            circle.dy = dy * generalAccelerationFactor;
            // circle.dy += gravity;
            circle.x += circle.dx;
            circle.y += circle.dy;
            resolveWallCollision(circle);
            for (Circle& other : circles) {
                if (&circle == &other) continue;
                if (areIntersecting(circle, other)) {
                    resolveCollision(circle, other);
                }
            }
        }
    }

    bool areIntersecting(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx*dx + dy*dy);
        return d <= (a.radius + b.radius);
    }

    bool areTouching(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx*dx + dy*dy);
        int r = std::round(a.radius + b.radius);
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

    // void resolveCollision(Circle& a, Circle& b) {
    //     float dx = b.x - a.x;
    //     float dy = b.y - a.y;
    //     float d = std::sqrt(dx*dx + dy*dy);
    //     float nx = dx/d;
    //     float ny = dy/d;
    //     float p = 2 * (a.dx * nx + a.dy * ny - b.dx * nx - b.dy * ny) / (a.getMass() + b.getMass());
    //     float lerp = a.radius + b.radius - d;
    //     float m = a.getMass() + b.getMass();
    //     a.x -= dx/d * a.getMass()/m * lerp;
    //     a.y -= dy/d * a.getMass()/m * lerp;
    //     b.x += dx/d * b.getMass()/m * lerp;
    //     b.y += dy/d * b.getMass()/m * lerp;
    //     a.dx -= p * a.getMass() * nx * circleCollisionEnergyLossFactor;
    //     a.dy -= p * a.getMass() * ny * circleCollisionEnergyLossFactor;
    //     b.dx += p * b.getMass() * nx * circleCollisionEnergyLossFactor;
    //     b.dy += p * b.getMass() * ny * circleCollisionEnergyLossFactor;
    // }

    void resolveCollision(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx * dx + dy * dy);
        float m = a.radius + b.radius - d;
        if (m <= 0) return;
        if (d == 0) d = 1, dx = 1, dy = 0;
        else dx /= d, dy /= d;
        float M = a.getMass() + b.getMass();
        float aM = b.getMass() / M;
        float bM = a.getMass() / M;
        a.x -= dx * m * aM;
        a.y -= dy * m * aM;
        b.x += dx * m * bM;
        b.y += dy * m * bM;
    }
};