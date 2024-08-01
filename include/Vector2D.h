#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0);

    Vector2D operator+(const Vector2D& v) const;
    Vector2D operator-(const Vector2D& v) const;
    Vector2D operator*(double scalar) const;

    double magnitude() const;
    Vector2D normalized() const;
};

#endif // VECTOR2D_H
