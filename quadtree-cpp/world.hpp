#pragma once
#include <iostream>
#include "./circle.hpp"
#include "./utils.hpp"

struct World {
    static constexpr float windowX = 800;
    static constexpr float windowY = 800;
    static constexpr int collisionIterations = 8;
    static constexpr int substeps = 8;
    static constexpr float dt = (1.f / 60.f) / static_cast<float>(substeps);
    static constexpr float generalAccelerationFactor = 500 * substeps;

    std::vector<Circle> circles;

    void update(sf::Vector2i mousePos) {
        for (int i = 0; i < substeps; i++) {
            for (Circle& circle : circles) {
                resolveMotion(circle, mousePos);
                resolveWallCollision(circle);
            }
            for (int i = 0; i < collisionIterations; i++) {
                for (Circle& a : circles) {
                    for (Circle& b : circles) {
                        if (&a == &b) continue;
                        resolveCollision(a, b);
                    }
                }
            }
        }
    }

    void resolveMotion(Circle& circle, sf::Vector2i mousePos) {
        if (circle.isDragging) {
            circle.x = mousePos.x;
            circle.y = mousePos.y;
        };
        float dx = windowX/2 - circle.x;
        float dy = windowY/2 - circle.y;
        circle.dx = dx * generalAccelerationFactor * dt;
        circle.dy = dy * generalAccelerationFactor * dt;
        circle.x += circle.dx * dt;
        circle.y += circle.dy * dt;
    }

    void resolveCollision(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        //float d = fastSqrt(dx*dx + dy*dy);
        float d = sqrt(dx*dx + dy*dy);
        if (d > round(a.radius + b.radius)) return;
        float lerp = (a.radius + b.radius - d)/2;
        a.x -= dx/d * lerp;
        a.y -= dy/d * lerp;
        b.x += dx/d * lerp;
        b.y += dy/d * lerp;
    }

    bool areColliding(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        // float d = sqrt(dx*dx + dy*dy);
        // return d <= round(a.radius + b.radius) + 1;
        float d2 = dx*dx + dy*dy;
        float r = round(a.radius + b.radius) + 1;
        return d2 <= r*r;
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
            c.dy = -c.dy;
        }
    }
};