/**
 * @file vector.h
 * @author zcx 
 * @brief Template class for vector
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */
#pragma once
#include <assert.h>
#include "FastMath.h"
#include <iostream>
#include "Eigen/Dense"

/// \ingroup Geometry
/// \brief Vector2
template <typename T>
struct TVector2 {
    
    /*---data field---*/
    T x, y;

    /*----constructor---*/
    TVector2() { }

    TVector2(T _x, T _y) : x(_x), y(_y) { }

    explicit TVector2(T t) : x(t), y(t) { }

    /*---operator overloading---*/
    TVector2 operator+(const TVector2 &rhs) const {
        return TVector2(x + rhs.x, y + rhs.y);
    }
    
    TVector2& operator+=(const TVector2 &rhs) {
        x += rhs.x, y += rhs.y;
        return *this;
    }

    TVector2 operator-(const TVector2 &rhs) const {
        return TVector2(x - rhs.x, y - rhs.y);
    }

    TVector2& operator-=(const TVector2 &rhs) {
        x -= rhs.x, y-= rhs.y;
        return *this;
    }

    TVector2 operator*(const T t) const {
        return TVector2(x * t, y * t);
    }

    TVector2& operator*=(const T t) {
        x *= t, y *= t;
        return *this;
    }

    TVector2 operator/(T t) const {
        assert(t != 0);
        T recip = (T) 1 / t;
        return TVector2(x * recip, y * recip);
    }

    TVector2& operator/=(T t) {
        assert(t != 0);
        T recip = (T) 1/t;
        x *= recip, y *= recip;
        return *this;
    }

    TVector2 operator-() const {
        return TVector2(-x, -y);
    }

    T operator[](int i) const {
        return (&x)[i];
    }

    T& operator[](int i) {
        return (&x)[i];
    }

    decltype(auto) length2() const {
        return x * x + y * y;
    }

    decltype(auto) length() const {
        return fm::sqrt(x*x + y*y);
    }

    bool operator==(const TVector2 &rhs) const {
        return x==rhs.x && y==rhs.y;
    }

    bool operator!=(const TVector2 &rhs) const {
        return x!=rhs.x || y!=rhs.y;
    }

    bool isZero() const {
        return x==0 && y==0;
    }

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const TVector2<T> &v) {
    os << v.x << " " << v.y ;
    return os;
}

template <typename T>
TVector2<T> operator*(T t, const TVector2<T> &v) {
    return TVector2<T>(v.x * t, v.y * t);
}

template <typename T>
decltype(auto) dot(const TVector2<T> &v1, const TVector2<T> &v2) {
    return v1.x*v2.x + v1.y * v2.y;
}

template <typename T>
decltype(auto) absDot(const TVector2<T> &v1, const TVector2<T> &v2) {
    return fm::abs(v1.x*v2.x + v1.y * v2.y);
}

template <typename T>
TVector2<double> normalize(const TVector2<T> &v) {
    double recip = 1.0f / v.length();
    return TVector2<double> (v.x * recip, v.y * recip);
}

template<> 
inline TVector2<int> TVector2<int>::operator/(int i) const {
    assert(i != 0);
    return TVector2<int>(x/i, y/i);
}

template<> 
inline TVector2<int>& TVector2<int>::operator/=(int i) {
    assert(i != 0);
    x/=i, y/=i;
    return *this;
}

/// \ingroup Geometry
/// \brief Vector3
template <typename T>
struct TVector3 {
    T x, y, z;

    TVector3 () { }

    TVector3 (T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }

    explicit TVector3 (T t) : x(t), y(t), z(t) { }

    /*--- operator overloading ---*/
    TVector3 operator+(T value) const {
         return TVector3(x + value , y + value, z + value);
    }

    TVector3 operator+(const TVector3 &rhs) const {
        return TVector3(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    TVector3& operator+=(const TVector3 &rhs) {
        x += rhs.x, y += rhs.y, z += rhs.z;
        return *this;
    }

    TVector3 operator-(T value) const {
        return TVector3(x - value , y - value, z -value);
    }

    TVector3 operator-(const TVector3 &rhs) const {
        return TVector3(x-rhs.x, y-rhs.y, z-rhs.z);
    }

    TVector3& operator-=(const TVector3 &rhs) {
        x -= rhs.x, y -= rhs.y, z -= rhs.z;
        return *this;
    }

    TVector3 operator*(const T t) const {
        return TVector3(x*t, y*t, z*t);
    }

    TVector3 operator*(const TVector3 &rhs) const {
        return TVector3(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    TVector3& operator*=(const T t) {
        x*=t, y*=t, z*=t;
        return *this;
    }

    TVector3 operator/(const T t) const {
        assert(t != 0);
        T recip = (T) 1/t;
        return TVector3(x*recip, y*recip, z*recip);
    }

    TVector3 operator/(const TVector3 &rhs) const {
        return TVector3(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    TVector3& operator/=(const T t) {
        assert(t != 0);
        T recip = (T) 1/t;
        x*=recip, y*=recip, z*=recip;
        return *this;
    }

    TVector3 operator-() const {
        return TVector3(-x, -y, -z);
    }

    T operator[](const int i) const {
        return (&x)[i];
    }

    T& operator[](const int i) {
        return (&x)[i];
    }

    decltype(auto) length2() const {
        return x*x + y*y + z*z;
    }
    
    decltype(auto) length() const {
        return fm::sqrt(x*x + y*y + z*z);
    }

    bool operator==(const TVector3 &rhs) const {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }

    bool operator!=(const TVector3 &rhs) const {
        return x!=rhs.x || y!=rhs.y || z!=rhs.z;
    }

    bool isZero() const {
        return x==0 && y==0 && z==0;
    }
};


template <typename T>
struct TVector4 {
    T x, y, z,w;

    TVector4 () { }

    TVector4 (T _x, T _y, T _z,T _w) : x(_x), y(_y), z(_z),w(_w) { }

    explicit TVector4 (T t) : x(t), y(t), z(t),w(t) { }

    /*--- operator overloading ---*/
    TVector4 operator+(T value) const {
        return TVector4(x + value , y + value, z + value, w + value);
    }

    TVector4 operator+(const TVector4 &rhs) const {
        return TVector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    TVector4& operator+=(const TVector4 &rhs) {
        x += rhs.x, y += rhs.y, z += rhs.z, w+=rhs.w;
        return *this;
    }

    TVector4 operator-(T value) const {
        return TVector4(x - value , y - value, z -value, w -value);
    }

    TVector4 operator-(const TVector4 &rhs) const {
        return TVector4(x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w);
    }

    TVector4& operator-=(const TVector4 &rhs) {
        x -= rhs.x, y -= rhs.y, z -= rhs.z,w -= rhs.w;
        return *this;
    }

    TVector4 operator*(const T t) const {
        return TVector4(x*t, y*t, z*t, w*t);
    }

    TVector4 operator*(const TVector4 &rhs) const {
        return TVector4(x * rhs.x, y * rhs.y, z * rhs.z,w * rhs.z);
    }

    TVector4& operator*=(const T t) {
        x*=t, y*=t, z*=t, w*=t;
        return *this;
    }

    TVector4 operator/(const T t) const {
        assert(t != 0);
        T recip = (T) 1/t;
        return TVector4(x*recip, y*recip, z*recip, w*recip);
    }

    TVector4 operator/(const TVector4 &rhs) const {
        return TVector4(x / rhs.x, y / rhs.y, z / rhs.z, w/rhs.w);
    }

    TVector4& operator/=(const T t) {
        assert(t != 0);
        T recip = (T) 1/t;
        x*=recip, y*=recip, z*=recip, w*=recip;
        return *this;
    }

    TVector4 operator-() const {
        return TVector4(-x, -y, -z,-w);
    }

    T operator[](const int i) const {
        return (&x)[i];
    }

    T& operator[](const int i) {
        return (&x)[i];
    }


    bool operator==(const TVector4 &rhs) const {
        return x==rhs.x && y==rhs.y && z==rhs.z && w==rhs.w;
    }

    bool operator!=(const TVector4 &rhs) const {
        return x!=rhs.x || y!=rhs.y || z!=rhs.z || w!=rhs.w;
    }

    bool isZero() const {
        return x==0 && y==0 && z==0 && w==0;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream &os, const TVector3<T> &v) {
    os << v.x << " " << v.y << " " << v.z;
    return os;
}

template <typename T>
TVector3<T> operator*(T t, const TVector3<T> &v) {
    return TVector3<T>(v.x*t, v.y*t, v.z*t);
}

template <typename T>
decltype(auto) dot(const TVector3<T> &v1, const TVector3<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
decltype(auto) absDot(const TVector3<T> &v1, const TVector3<T> &v2) {
    return fm::abs(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

template <typename T>
TVector3<double> normalize(const TVector3<T> &v) {
    double recip = 1.0f / v.length();
    return TVector3<double> (v.x * recip, v.y * recip, v.z * recip);
}

/**
 * @brief Only 3 deminsion vector can do this operation
 * 
 * @tparam T 
 * @param v1 
 * @param v2 
 * @return TVector3<T> 
 */
template <typename T>
TVector3<T> cross(const TVector3<T> &v1, const TVector3<T> &v2) {
    return TVector3<T> (
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

inline TVector3<double> 
eigenToVector3d(Eigen::DenseBase<Eigen::MatrixXd>::ConstColXpr col) {
    return TVector3<double>{
        col.x(), col.y(), col.z()
    };
}


