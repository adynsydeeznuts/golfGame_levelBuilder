#ifndef LINE_H
#define LINE_H

#include <cmath>
#include <raylib.h>
#include <raymath.h>

class Line {
private:
    Vector2 point1; // First point (x1, y1)
    Vector2 point2; // Second point (x2, y2)
public:
    // Constructor to initialize the line with two points
    Line(Vector2 point1, Vector2 point2)
        : point1(point1), point2(point2) {}

    // Getters for the points
    Vector2 getPoint1() const { return point1; }
    Vector2 getPoint2() const { return point2; }

    // Setters for the points
    void setPoint1(float x, float y) { point1 = {x, y}; }
    void setPoint2(float x, float y) { point2 = {x, y}; }

    // Function to calculate the length of the line
    float length() const {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        return sqrt(dx * dx + dy * dy);
    }

    void Draw() const {
        DrawLineV(point1, point2, DARKBROWN);
    }

};

#endif