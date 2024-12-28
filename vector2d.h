#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QtMath>
class Vector2D {
public:
    float x,y;
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};
    Vector2D():x(0),y(0) {};
    Vector2D operator+(const Vector2D &op) const {
        return Vector2D(x+op.x,y+op.y);
    }
    Vector2D operator-(const Vector2D &op) const {
        return Vector2D(x-op.x,y-op.y);
    }
    // Multiplication par un scalaire
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    /*
     * return a normed vector
     */
    Vector2D normed() const {
        float d = sqrt(x*x+y*y);
        return Vector2D(x/d,y/d);
    }
    /*
     * return an orthogonal vector
     */
    Vector2D ortho() const {
        return Vector2D(-y,x);
    }
    double length() const {
        return std::sqrt(x * x + y * y);
    }
    /*
     * Return the dot product between this vector and another vector.
     */
    float dot(const Vector2D &op) const {
        return x * op.x + y * op.y;
    }
    /*
     * Calculate the perpendicular projection of point M on the line segment AB.
     */
    static Vector2D projection(const Vector2D &A, const Vector2D &B, const Vector2D &M) {
        Vector2D AB = B - A;   // Vector AB
        Vector2D AM = M - A;   // Vector AM

        // Projection factor t
        float t = AM.dot(AB) / (AB.length() * AB.length());

        // Clamp t to the range [0, 1]
        t = std::max(0.0f, std::min(1.0f, t));

        // Point H = A + t * AB
        return A + AB * t;
    }

};

#endif // VECTOR2D_H
