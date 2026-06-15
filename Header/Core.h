//
// Created by elder on 6/14/2026.
//

#ifndef BLACKHOLE_SIM_CORE_H
#define BLACKHOLE_SIM_CORE_H

#include <cmath>

class Vector3 {
public:
    float x;
    float y;
    float z;
private:
    float pad;

public:
    Vector3() : x(0), y(0), z(0) {pad = 0;}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {pad = 0;}

    void invert() {
        x = -x;
        y = -y;
        z = -z;
    }

    float magnitude() const {
        return std:: sqrtf(x*x + y*y + z*z);
    }

    float squareMagnitude() const {
        return x*x + y*y + z*z;
    }

    void normalize() {
        float length = magnitude();
        if (length > 0) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    // multiples value to current Vector3 object
    void operator*=(const float value) {
        x *= value;
        y *= value;
        z *= value;
    }

    // multiplies values to this and returns new Vector3 object
    Vector3 operator*(const float value) const {
        return { x * value, y * value, z * value};
    }

    // adds &vector to current Vector3 object
    void operator+=(const Vector3 &vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    // adds &vector to current Vector3 object and returns new object
    Vector3 operator+(const Vector3 &vector) const {
        return {x + vector.x, y + vector.y, z + vector.z};
    }

    // subtracts &vector3 values from this
    void operator-=(const Vector3& vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    // // subtracts &vector3 values from this and returns new object
    Vector3 operator-(const Vector3 &vector) const {
        return {x - vector.x, y - vector.y, z - vector.z};
    }

    float operator*(const Vector3& vector) const {
        return (x * vector.x) + (y * vector.y) + (z * vector.z);
    }

    void operator %=(const Vector3& vector) {
        *this = vectorCrossProduct(vector);
    }

    // same thing as vectorCrossProduct function but here
    // we would need to define as Vector3 c = a % b
    Vector3 operator%(const Vector3& vector) const {
        return {
            ((y * vector.z) - (z * vector.y)),
            ((z * vector.x) - (x * vector.z)),
            ((x * vector.y) - (y * vector.x))
        };
    }

    Vector3 operator/(const Vector3& vector) const {
        return { x / vector.x, y / vector.y, z / vector.z };
    }

    void operator/=(const Vector3& vector) {
        // divide all components to *this vector object
        x /= vector.x;
        y /= vector.y;
        z /= vector.z;
    }

    Vector3 operator/(const float &val) const {
        return { x / val, y / val, z / val };
    }

    void addScaledVector(const Vector3& vector, const float scale) {
        x += vector.x * scale;
        y += vector.y * scale;
        z += vector.z * scale;
    }

    // function to do vector * vector arithmatic, returns new object
    Vector3 componentProduct(const Vector3& vector) const {
        return { x * vector.x, y * vector.y, z * vector.z };
    }
    // function to do vector * vector arithmatic
    void componentUpdate(const Vector3& vector) {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
    }

    // function that will return a float scalar value from a dot b
    float scalarProduct(const Vector3& vector) const {
        return (x * vector.x) + (y * vector.y) + (z * vector.z);
    }

    [[nodiscard]] Vector3 vectorCrossProduct(const Vector3& vector) const {
        return {
            ((y * vector.z) - (z * vector.y)),
            ((z * vector.x) - (x * vector.z)),
            ((x * vector.y) - (y * vector.x))
        };
    }

    void makeOrthoNormalBasis(Vector3* a, Vector3* b, Vector3* c) {
        a->normalize();
        (*c) = (*a) % (*b);
        if (c->magnitude() == 0.0) return;

        c->normalize();
        (*b) = (*c) % (*a);
    }
};



#endif //BLACKHOLE_SIM_CORE_H