#ifndef RAYTRACER_VECTORS_H
#define RAYTRACER_VECTORS_H

#include <cmath>
#include <iostream>
#include <utility>

namespace scg
{

template<int DIM, typename T>
class Vector
{
public:
    union {
        // TODO: check order
        struct {
            T x;
            T y;
            T z;
            T w;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        };
        T data[DIM];
    };

    Vector(){};

    Vector(Vector const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] = arg.data[i];
        }
    }

//    template<typename... Args>
//    explicit Vector(Args&&... args) : data{std::forward<T>(args)...} {
//        static_assert(sizeof...(Args) == DIM, "You must provide N arguments.");
//    }

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

    inline Vector& normalise()
    {
        T length = this->length();
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] /= length;
        }
        return *this;
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

    inline T operator [](int i) const { return this->data[i]; }
    inline T& operator [](int i) { return this->data[i]; }

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

typedef Vector<2, int> Vec2i;
typedef Vector<3, float> Vec3f;

template<typename T>
inline Vector<3, T> cross(Vector<3, T> const& a, Vector<3, T> const& b)
{
    return Vector<3, T>(
        a.y * b.z - b.y * a.z,
        a.z * b.x - b.z * a.x,
        a.x * b.y - b.x * a.y);
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

}

#endif //RAYTRACER_VECTORS_H
