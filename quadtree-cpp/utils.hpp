#pragma once
#include <SFML/Graphics.hpp>

sf::Color getRandomColor() {
    return sf::Color(rand() % 256, rand() % 256, rand() % 256);
}

float fastSqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F; 

    x2 = number * 0.5F; 
    y = number; 
    i = * ( long * ) &y;            // evil floating point bit level hacking
    i = 0x5f3759df - ( i >> 1 );    // what the fuck?
    y = * ( float * ) &i; 
    y = y * ( threehalfs - ( x2 * y * y ) );    // 1st iteration
    // y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed 

    return 1.f/y; 
}