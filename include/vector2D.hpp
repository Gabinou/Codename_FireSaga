#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <stdio.h>


class Vector2D {
    public:
        short int unsigned x;
        short int unsigned y;
        
        Vector2D();
        Vector2D(short unsigned int x, short unsigned int y);
        
        Vector2D& add(const Vector2D& vec);
        Vector2D& subtract(const Vector2D& vec);
        Vector2D& multiply(const Vector2D& vec);
        Vector2D& divide(const Vector2D& vec);
        
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