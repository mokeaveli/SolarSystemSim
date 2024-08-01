#ifndef BODY_H
#define BODY_H

#include "Vector2D.h"

class Body {
public:
    Vector2D position;
    Vector2D velocity;
    double mass;

    Body(Vector2D position, Vector2D velocity, double mass);

    void updatePosition(double timeStep);
    void updateVelocity(Vector2D acceleration, double timeStep);
    Vector2D gravitationalForce(const Body& other) const;
};

#endif // BODY_H
