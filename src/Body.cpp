#include "Body.h"
#include "Vector2D.h"

const double G = 6.67430e-11; // Gravitational constant

Body::Body(Vector2D position, Vector2D velocity, double mass)
    : position(position), velocity(velocity), mass(mass) {}

void Body::updatePosition(double timeStep) {
    position = position + velocity * timeStep;
}

void Body::updateVelocity(Vector2D acceleration, double timeStep) {
    velocity = velocity + acceleration * timeStep;
}

Vector2D Body::gravitationalForce(const Body& other) const {
    Vector2D direction = other.position - position;
    double distance = direction.magnitude();
    double forceMagnitude = (G * mass * other.mass) / (distance * distance);
    return direction.normalized() * forceMagnitude;
}
