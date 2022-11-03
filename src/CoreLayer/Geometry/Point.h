/**
 * @file Point.h
 * @author zcx
 * @brief 
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright NJUMeta (c) 2022 
 * www.njumeta.com
 * 
 */
#pragma once
#include "Vector.h"
#include "Eigen/Dense"

/// \ingroup Geometry
/// \brief Point2 
template <typename T>
struct TPoint2 {
    /*--- data field ---*/
    T x, y;

    /*--- constructor ---*/
    TPoint2() { }

    TPoint2(T _x, T _y) : x(_x), y(_y) { }

    explicit TPoint2(T t) : x(t), y(t) { }

    /*--- operator overloading ---*/
    TPoint2 operator+(const TVector2<T> &rhs) const {
        return TPoint2(x+rhs.x, y+rhs.y);
    }

	TPoint2 operator+(const TPoint2<T>& rhs) const {
		return TPoint2(x + rhs.x, y + rhs.y);
	}

    TPoint2& operator+=(const TVector2<T> &rhs) {
        x += rhs.x, y += rhs.y;
        return *this;
    }

    TPoint2 operator-(const TVector2<T> &rhs) const {
        return TPoint2(x-rhs.x, y-rhs.y);
    }

	TVector2<T> operator-(const TPoint2<T>& rhs) const {
		return TVector2(x - rhs.x, y - rhs.y);
	}

    TPoint2& operator-=(const TVector2<T> &rhs) {
        x -= rhs.x, y -= rhs.y;
        return *this;
    }

    TPoint2 operator*(T t) const {
        return TPoint2(x*t, y*t);
    }

    TPoint2& operator*=(T t) {
        x *= t, y *= t;
        return *this;
    }

    TPoint2 operator/(T t) const {
        assert(t != 0);
        T recip = (T) 1 / t;
        return TPoint2(x*recip, y*recip);
    }

    TPoint2& operator/=(T t) {
        assert(t != 0);
        T recip = (T) 1 / t;
        x *= recip, y *= recip;
        return *this;
    }

    TPoint2 operator-() const {
        return TPoint2(-x, -y);
    }

    bool operator==(const TPoint2 &rhs) const {
        return x==rhs.x && y==rhs.y;
    }

    bool operator!=(const TPoint2 &rhs) const {
        return x!=rhs.x || y!=rhs.y;
    }

    T operator[] (int i) const {
        return (&x)[i];
    }

    T& operator[] (int i) {
        return (&x)[i];
    }

    bool isZero() const {
        return x==0 && y==0;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream &os, const TPoint2<T> &p) {
    os << p.x << " " << p.y;
    return os;
}

template <typename T>
TPoint2<T> operator*(T t, const TPoint2<T> &v) {
    return TPoint2(t*v.x, t*v.y);
}

template<>
inline TPoint2<int> TPoint2<int>::operator/(int i) const {
    assert(i!=0);
    return TPoint2<int>(x/i, y/i);
}

template<>
inline TPoint2<int>& TPoint2<int>::operator/=(int i) {
    assert(i!=0);
    x/=i, y/=i;
    return *this; 
}

/// \ingroup Geometry
/// \brief Point3
template <typename T>
struct TPoint3 {
    /*--- data field ---*/
    T x, y, z;

    /*--- constructor ---*/
    TPoint3() { }

    TPoint3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }

    explicit TPoint3(T t) : x(t), y(t), z(t) { }

    /*--- operator overloading ---*/
    TPoint3 operator+(const TVector3<T> &rhs) const {
        return TPoint3(x+rhs.x, y+rhs.y, z+rhs.z);
    }

	TPoint3 operator+(const TPoint3<T>& rhs) const {
		return TPoint3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

    TPoint3& operator+=(const TVector3<T> &rhs) {
        x += rhs.x, y += rhs.y, z += rhs.z;
        return *this;
    }

    TPoint3 operator-(const TVector3<T> &rhs) const {
        return TPoint3(x-rhs.x, y-rhs.y, z-rhs.z);
    }

    TVector3<T> operator-(const TPoint3<T> &rhs) const {
        return TVector3<T>(x-rhs.x, y-rhs.y, z-rhs.z);
    }

    TPoint3& operator-=(const TVector3<T> &rhs) {
        x -= rhs.x, y -= rhs.y, z -= rhs.z;
        return *this;
    }

    TPoint3 operator*(T t) const {
        return TPoint3(x*t, y*t, z*t);
    }

    TPoint3& operator*=(T t) {
        x *= t, y *= t, z *= t;
        return *this;
    }

    TPoint3 operator/(T t) const {
        assert(t != 0);
        T recip = (T) 1 / t;
        return TPoint3(x*recip, y*recip, z*recip);
    }

    TPoint3& operator/=(T t) {
        assert(t != 0);
        T recip = (T) 1 / t;
        x *= recip, y *= recip, z *= recip;
        return *this;
    }

    TPoint3 operator-() const {
        return TPoint3(-x, -y, -z);
    }

    bool operator==(const TPoint3 &rhs) const {
        return x==rhs.x && y==rhs.y && z==rhs.z;
    }

    bool operator!=(const TPoint3 &rhs) const {
        return x!=rhs.x || y!=rhs.y || z!=rhs.z;
    }

    T operator[] (int i) const {
        return (&x)[i];
    }

    T& operator[] (int i) {
        return (&x)[i];
    }

    bool isZero() const {
        return x==0 && y==0 && z==0;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream &os, const TPoint3<T> &p) {
    os << p.x << " " << p.y << " " << p.z;
    return os;
}

template <typename T>
TPoint3<T> operator*(T t, const TPoint3<T> &v) {
    return TPoint3(t*v.x, t*v.y, t*v.z);
}

template<>
inline TPoint3<int> TPoint3<int>::operator/(int i) const {
    assert(i!=0);
    return TPoint3<int>(x/i, y/i, z/i);
}

template<>
inline TPoint3<int>& TPoint3<int>::operator/=(int i) {
    assert(i!=0);
    x/=i, y/=i, z/i;
    return *this; 
}

inline TPoint3<double> 
eigenToPoint3d(Eigen::DenseBase<Eigen::MatrixXd>::ConstColXpr col) {
    return TPoint3<double>{
        col.x(), col.y(), col.z()
    };
}