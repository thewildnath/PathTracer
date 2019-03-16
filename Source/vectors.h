#ifndef RAYTRACER_VECTORS_H
#define RAYTRACER_VECTORS_H

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
        };
        struct {
            T r;
            T g;
            T b;
        };
        T data[DIM];
    };

    Vector(){};

    template<typename... Args> //can't use float... anyway
    explicit Vector(Args&&... args) : data{std::forward<T>(args)...} {
        static_assert(sizeof...(Args) == DIM, "You must provide N arguments.");
    }

    Vector(Vector const& arg)
    {
        for (int i = 0; i < DIM; ++i)
        {
            this->data[i] = arg.data[i];
        }
    }

    //inline const T cross(Vector<DIM, T> const& arg) const;

    inline T dot(Vector const& arg) const
    {
        T result = 0;
        for (int i = 0; i < DIM; ++i)
        {
            result += this->data[i] * arg.data[i];
        }
        return result;
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
    inline Vector operator +(V const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] + static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator +(Vector<DIM, V> const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] + static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator -(V const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] - static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator -(Vector<DIM, V> const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] - static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator *(V const& arg)
    {
        Vector result;
        #pragma GCC unroll 10
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] * static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator *(Vector<DIM, V> const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] * static_cast<T>(arg.data[i]));
        }
        return result;
    }

    template<typename V>
    inline Vector operator /(V const& arg)
    {
        Vector result;
        for (int i = 0; i < DIM; ++i)
        {
            result.data[i] = (this->data[i] / static_cast<T>(arg));
        }
        return result;
    }

    template<typename V>
    inline Vector operator /(Vector<DIM, V> const& arg)
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

}

#endif //RAYTRACER_VECTORS_H
