#include "vector2D.hpp"
#include <stdio.h>

Vector2D::Vector2D() {
    x = 0;
    y = 0;
}

Vector2D::Vector2D( short unsigned int x,  short unsigned int y) {
    this->x = x;
    this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec){
    this->x += vec.x;
    this->y += vec.y;
    
    return(*this);
}
 
Vector2D& Vector2D::Subtract(const Vector2D& vec){
    this->x -= vec.x;
    this->y -= vec.y;
    
    return(*this);
}

Vector2D& Vector2D::Multiply(const Vector2D& vec){
    this->x *= vec.x;
    this->y *= vec.y;
    
    return(*this);
}
 
Vector2D& Vector2D::Divide(const Vector2D& vec){
    this->x /= vec.x;
    this->y /= vec.y;
    
    return(*this);
}

Vector2D& operator+(Vector2D& v1, const Vector2D& v2){
    return(v1.Add(v2));
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2){
    return(v1.Subtract(v2));
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2){
    return(v1.Multiply(v2));
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2){
    return(v1.Divide(v2));
}

Vector2D& Vector2D::operator+=(const Vector2D& vec){
    return(this->Add(vec));
}


Vector2D& Vector2D::operator-=(const Vector2D& vec){
    return(this->Subtract(vec));
}


Vector2D& Vector2D::operator*=(const Vector2D& vec){
    return(this->Multiply(vec));
}


Vector2D& Vector2D::operator/=(const Vector2D& vec){
    return(this->Divide(vec));
}
void Vector2D::print(){
    printf("[%d,%d]", this->x, this->y);
}
