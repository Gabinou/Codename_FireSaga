#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

class Vector2D {
    public:
        short int x;
        short int y;
        
        Vector2D();
        Vector2D(short int x, short int y);
        
        Vector2D& Add(const Vector2D& vec);
        Vector2D& Substract(const Vector2D& vec);
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
        
    
}

#endif /* VECTOR2D_HPP */