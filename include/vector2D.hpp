#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <stdio.h>

class Vector2D {
    public:
        short int unsigned x;
        short int unsigned y;
        
        Vector2D();
        Vector2D(short unsigned int x, short unsigned int y);
        
        Vector2D& Add(const Vector2D& vec);
        Vector2D& Subtract(const Vector2D& vec);
        Vector2D& Multiply(const Vector2D& vec);
        Vector2D& Divide(const Vector2D& vec);
        
        friend Vector2D& operator+(Vector2D& v1, const Vector2D& V2);
        friend Vector2D& operator-(Vector2D& v1, const Vector2D& V2);
        friend Vector2D& operator*(Vector2D& v1, const Vector2D& V2);
        friend Vector2D& operator/(Vector2D& v1, const Vector2D& V2);
        
        Vector2D& operator+=(const Vector2D& vec);
        Vector2D& operator-=(const Vector2D& vec);
        Vector2D& operator*=(const Vector2D& vec);
        Vector2D& operator/=(const Vector2D& vec);
        
        void print();
};

#endif /* VECTOR2D_HPP */