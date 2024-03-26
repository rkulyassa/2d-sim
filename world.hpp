#pragma once

#include <iostream>
#include "./circle.hpp"
#include "./utils.hpp"

struct World {
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

    void update() {
        for (Circle& circle : circles) {
            // std::cout << circle.y << " " << circle.dy << std::endl;
            float dx = 400 - circle.x;
            float dy = 400 - circle.y;
            float d = std::sqrt(dx*dx + dy*dy);
            circle.dx = dx/d * std::abs(dx) * generalAccelerationFactor;
            circle.dy = dy/d * std::abs(dy) * generalAccelerationFactor;
            // circle.dy += gravity;
            circle.x += circle.dx;
            circle.y += circle.dy;
            resolveWallCollision(circle);
            for (Circle& other : circles) {
                if (&circle == &other) continue;
                if (checkIntersection(circle, other)) {
                    // std::cout << circle.x << " " << circle.y << " " << other.x << " " << other.y << std::endl;
                    //resolveOverlap(circle, other);
                    resolveCollision(circle, other);
                }
            }
        }
    }

    bool checkIntersection(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        //float d = fastSqrt(dx*dx + dy*dy);
        float d = std::sqrt(dx*dx + dy*dy);
        return d <= (a.radius + b.radius);
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
    //     std::cout << "I: " << a.dx << " " << a.dy << " " << b.dx << " " << b.dy << std::endl;
    //     float dx = b.x - a.x;
    //     float dy = b.y - a.y;
    //     float d = std::sqrt(dx*dx + dy*dy);
    //     dx /= d;
    //     dy /= d;

    //     float dvx = b.dx - a.dx;
    //     float dvy = b.dy - a.dy;
    //     float vNormal = dvx * dx + dvy * dy;
    //     // std::cout << vNormal << std::endl;
    //     if (vNormal > 0) return;

    //     float restitution = 1.0f;
    //     float impulseScalar = -(1 + restitution) * vNormal / (a.getMass() + b.getMass());
    //     std::cout << impulseScalar << std::endl;
    //     float impulseX = impulseScalar * dx;
    //     float impulseY = impulseScalar * dy;
    //     // std::cout << impulseX << " " << impulseY << std::endl;
    //     a.dx -= impulseX / a.getMass();
    //     a.dy -= impulseY / a.getMass();
    //     b.dx += impulseX / b.getMass();
    //     b.dy += impulseY / b.getMass();
    //     std::cout << "F: " << a.dx << " " << a.dy << " " << b.dx << " " << b.dy << std::endl;
    // }

    void resolveCollision(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx*dx + dy*dy);
        float nx = dx/d;
        float ny = dy/d;
        float p = 2 * (a.dx * nx + a.dy * ny - b.dx * nx - b.dy * ny) / (a.getMass() + b.getMass());
        float lerp = a.radius + b.radius - d;
        float m = a.getMass() + b.getMass();
        a.x -= dx/d * a.getMass()/m * lerp;
        a.y -= dy/d * a.getMass()/m * lerp;
        b.x += dx/d * b.getMass()/m * lerp;
        b.y += dy/d * b.getMass()/m * lerp;
        a.dx -= p * a.getMass() * nx * circleCollisionEnergyLossFactor;
        a.dy -= p * a.getMass() * ny * circleCollisionEnergyLossFactor;
        b.dx += p * b.getMass() * nx * circleCollisionEnergyLossFactor;
        b.dy += p * b.getMass() * ny * circleCollisionEnergyLossFactor;
    }

    // void resolveCollision(Circle& a, Circle& b) {
    //     float dx = b.x - a.x;
    //     float dy = b.y - a.y;
    //     float d = std::sqrt(dx*dx + dy*dy);
    //     float m = a.getMass() + b.getMass();
    //     float lerp = a.radius + b.radius - d;
    //     a.x -= dx/d * a.getMass()/m * lerp;
    //     a.y -= dy/d * a.getMass()/m * lerp;
    //     b.x += dx/d * b.getMass()/m * lerp;
    //     b.y += dy/d * b.getMass()/m * lerp;
    // }

    void resolveOverlap(Circle& a, Circle& b) {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float d = std::sqrt(dx*dx + dy*dy);
        float mx = (a.x + b.x) / 2;
        float my = (a.y + b.y) / 2;
        a.x = mx + a.radius * (a.x - b.x) / d;
        a.y = my + a.radius * (a.y - b.y) / d;
        b.x = mx + b.radius * (b.x - a.x) / d;
        b.y = my + b.radius * (b.y - a.y) / d;
    }
};