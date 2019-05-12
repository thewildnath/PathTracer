#ifndef RAYTRACER_VECTORS_H
#define RAYTRACER_VECTORS_H

#include "math_utils.h"

#include <cmath>
#include <iostream>
#include <utility>

namespace scg
{

template<int DIM, typename T>
class Vector
{
public:
    union
    {
        // TODO: check order
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };
        struct
        {
            T r;
            T g;
            T b;
            T a;
        };
        struct
        {
            T u;
            T v;
        };
        T data[DIM];
    };

    Vector()
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] = 0;
        }
    };

    Vector(Vector const& arg) = default;

    template<typename V>
    Vector(V arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] = static_cast<T>(arg);
        }
    }

    template <typename... Args>
    /*explicit*/ Vector(Args... args) : data{ T(args)... } {
        static_assert(sizeof...(Args) == DIM, "Wrong number of arguments.");
    }

    inline T length() const
    {
        T length = 0;
        for (int i = 0; i < DIM; ++i)
        {
            length += this->data[i] * this->data[i];
        }
        return std::sqrt(length);
    }

    friend std::ostream& operator<<(std::ostream& out, Vector const& arg)
    {
        out << "Vector{ ";
        for (int i = 0; i < DIM; ++i)
        {
            out << arg.data[i] << " ";
        }
        out << "}";
        return out;
    }

    //inline T operator [](int i) const { return this->data[i]; }
    //inline T& operator [](int i) { return this->data[i]; }

    // Unary operators
    inline Vector operator -() const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = -this->data[i];
        }
        return result;
    }

    // Binary operators
    template<typename V>
    inline Vector operator +(V const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] + static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator +(Vector<DIM, V> const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] + static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator -(V const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] - static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator -(Vector<DIM, V> const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] - static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator *(V const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] * static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator *(Vector<DIM, V> const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] * static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator /(V const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] / static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator /(Vector<DIM, V> const& arg) const
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] / static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector& operator =(V const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] = static_cast<T>(arg.data[i]);
        }
        return *this;
    }

    template<typename V>
    inline Vector& operator +=(V const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] += static_cast<T>(arg);
        }
        return *this;
    }

    template<typename V>
    inline Vector& operator +=(Vector<DIM, V> const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] += static_cast<T>(arg.data[i]);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator -=(V const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] -= static_cast<T>(arg);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator -=(Vector<DIM, V> const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] -= static_cast<T>(arg.data[i]);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator *=(V const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] *= static_cast<T>(arg);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator *=(Vector<DIM, V> const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] *= static_cast<T>(arg.data[i]);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator /=(V const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] /= static_cast<T>(arg);
        }
        return *this;
    }

    template<typename V>
    inline Vector operator /=(Vector<DIM, V> const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] /= static_cast<T>(arg.data[i]);
        }
        return *this;
    }
};

// Useful types
typedef Vector<2, int> Vec2i;
typedef Vector<2, float> Vec2f;
typedef Vector<3, float> Vec3f;
typedef Vector<4, float> Vec4f;

// Vector methods
template<int DIM, typename T>
inline Vector<DIM, T> normalise(Vector<DIM, T> const& a)
{
    Vector<DIM, T> result;
    T length = a.length();
    for (int i = 0; i < DIM; ++i)
    {
        result.data[i] = a.data[i] / length;
    }
    return result;
}

template<typename T>
inline Vector<3, T> cross(Vector<3, T> const& a, Vector<3, T> const& b)
{
    return Vector<3, T>{
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y
    };
}

template<int DIM, typename T>
inline T dot(Vector<DIM, T> const& a, Vector<DIM, T> const& b)
{
    T result = 0;
    for (int i = 0; i < DIM; ++i)
    {
        result += a.data[i] * b.data[i];
    }
    return result;
}

template<int DIM, typename T>
inline Vector<DIM, T> multiply(Vector<DIM, T> const& a, Vector<DIM, T> const& b)
{
    Vector<DIM, T> result;
    for (int i = 0; i < DIM; ++i)
    {
        result.data[i] = a.data[i] * b.data[i];
    }
    return result;
}

template<typename T>
inline Vector<3, T> rotateX(Vector<3, T> const& a, float radians)
{
    float sin = std::sin(radians);
    float cos = std::cos(radians);

    return Vector<3, T>{
        a.x,
        a.y * cos - a.z * sin,
        a.y * sin + a.z * cos
    };
}

template<typename T>
inline Vector<3, T> rotateY(Vector<3, T> const& a, float radians)
{
    float sin = std::sin(radians);
    float cos = std::cos(radians);

    return Vector<3, T>{
        a.x * cos + a.z * sin,
        a.y,
        -a.x * sin + a.z * cos
    };
}

template<typename T>
inline Vector<3, T> rotateZ(Vector<3, T> const& a, float radians)
{
    float sin = std::sin(radians);
    float cos = std::cos(radians);

    return Vector<3, T>(
        a.x * cos - a.y * sin,
        a.x * sin + a.y * cos,
        a.z
    );
}

template<typename T>
inline Vector<3, T> rotate(Vector<3, T> const& a, Vector<3, T> const& rot)
{
    return rotateZ(rotateX(rotateY(a, toRadians(rot.y)), toRadians(rot.x)), toRadians(rot.z));
}

}

#endif //RAYTRACER_VECTORS_H
