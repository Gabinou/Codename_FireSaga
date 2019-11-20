#include "vector2D.hpp"
#include <stdio.h>

Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

Vector2D::Vector2D(int x, int y) {
    this->x = x;
    this->y = y;
}

Vector2D & Vector2D::add(const Vector2D & vec) {
    this->x += vec.x;
    this->y += vec.y;
    return (*this);
}

Vector2D & Vector2D::subtract(const Vector2D & vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return (*this);
}

Vector2D & Vector2D::multiply(const Vector2D & vec) {
    this->x *= vec.x;
    this->y *= vec.y;
    return (*this);
}

Vector2D & Vector2D::divide(const Vector2D & vec) {
    this->x /= vec.x;
    this->y /= vec.y;
    return (*this);
}

Vector2D & operator+(Vector2D & v1, const Vector2D & v2) {
    return (v1.add(v2));
}

Vector2D & operator-(Vector2D & v1, const Vector2D & v2) {
    return (v1.subtract(v2));
}

Vector2D & operator*(Vector2D & v1, const Vector2D & v2) {
    return (v1.multiply(v2));
}

Vector2D & operator/(Vector2D & v1, const Vector2D & v2) {
    return (v1.divide(v2));
}

Vector2D & Vector2D::operator+=(const Vector2D & vec) {
    return (this->add(vec));
}

Vector2D & Vector2D::operator-=(const Vector2D & vec) {
    return (this->subtract(vec));
    
}

Vector2D & Vector2D::operator*=(const Vector2D & vec) {
    return (this->multiply(vec));
}

Vector2D & Vector2D::operator/=(const Vector2D & vec) {
    return (this->divide(vec));
}

void Vector2D::print() {
    printf("[%d,%d]", this->x, this->y);
}
