#ifndef HOLE_H
#define HOLE_H

#include "ball.h"

class Hole {
private:
    float x;       // X-coordinate of the hole
    float y;       // Y-coordinate of the hole
    float radius;  // Radius of the hole

public:
    // Constructor
    Hole(float x, float y, float radius) : x(x), y(y), radius(radius) {}

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }

    // Setters
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    void setRadius(float newRadius) { radius = newRadius; }

    // Check if a point is inside the hole
    bool isBallInHole(Ball ball) const {
        float dx = ball.getPosition().x - x;
        float dy = ball.getPosition().y - y;
        return (dx * dx + dy * dy) <= (radius * radius);
    }
};

#endif // HOLE_H