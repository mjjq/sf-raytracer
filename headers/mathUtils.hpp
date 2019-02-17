#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <cmath>
#include <iostream>

namespace Constants
{
    const double PI = 3.14159265359;
}

struct Quaternion
{
    double real;
    double i;
    double j;
    double k;

    Quaternion(double _real, double _i, double _j, double _k)
    : real{_real}, i{_i}, j{_j}, k{_k} {}

    Quaternion operator + (Quaternion q)
    {
        double newReal = real + q.real;
        double newI = i + q.i;
        double newJ = j + q.j;
        double newK = k + q.k;

        return Quaternion(newReal, newI, newJ, newK);
    }

    Quaternion operator - (Quaternion q)
    {
        double newReal = real - q.real;
        double newI = i - q.i;
        double newJ = j - q.j;
        double newK = k - q.k;

        return Quaternion(newReal, newI, newJ, newK);
    }

    Quaternion operator * (Quaternion q)
    {
        double newReal = real*q.real - i*q.i - j*q.j - k*q.k;
        double newI = real*q.i + i*q.real + j*q.k - k*q.j;
        double newJ = real*q.j + j*q.real + k*q.i - i*q.k;
        double newK = real*q.k + k*q.real + i*q.j - j*q.i;

        return Quaternion(newReal, newI, newJ, newK);
    }

    void print()
    {
        std::cout << real << " + " << i << "i + " << j << "j + " << k << "k";
    }
};

struct Vec3d
{
    Vec3d(double i, double j, double k) {x=i; y=j; z=k;}
    Vec3d() {}
    double x;
    double y;
    double z;

    Vec3d operator + (Vec3d v) {return Vec3d(x+v.x, y+v.y, z+v.z);}
    Vec3d operator - (Vec3d v) {return Vec3d(x-v.x, y-v.y, z-v.z);}
    Vec3d operator * (double d) {return Vec3d(x*d, y*d, z*d);}
    Vec3d operator / (double d) {return Vec3d(x/d, y/d, z/d);}

    double dot(Vec3d v) {return x*v.x + y*v.y + z*v.z;}
    static Vec3d norm(Vec3d v) {return v/sqrt(v.dot(v));}
    static Vec3d cross(Vec3d a, Vec3d b) {return {a.y*b.z - a.z*b.y,
                                                a.z*b.x - a.x*b.z,
                                                a.x*b.y - a.y*b.x};};

    Vec3d rotate(double angleInDeg, Vec3d axis)
    {
        double cosine = cos(angleInDeg * Constants::PI / 360.0);
        double sine = sin(angleInDeg * Constants::PI / 360.0);
        axis = Vec3d::norm(axis) * -sine;
        Quaternion q(cosine, axis.x, axis.y, axis.z);
        Quaternion p(0.0, x, y, z);
        Quaternion qInv(cosine, -axis.x, -axis.y, -axis.z);

        Quaternion result = q * p * qInv;

        return {result.i, result.j, result.k};
    }

    friend std::ostream& operator << (std::ostream &os, const Vec3d &v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};

struct Ray
{
    Ray(Vec3d start, Vec3d dir, double _tMax = 1e+15, int _order = 0)
        {o = start; d = dir; tMax = _tMax; order = _order;}
    Ray() {}
    double tMax = 1e+15;
    int order = 0;
    Vec3d o;
    Vec3d d;
};

struct ONormalSet
{
    Vec3d i = {1.0, 0.0, 0.0};
    Vec3d j = {0.0, 1.0, 0.0};
    Vec3d k = {0.0, 0.0, 1.0};

    Vec3d iRot;
    Vec3d jRot;
    Vec3d kRot;

    ONormalSet(Vec3d _i, Vec3d _j, Vec3d _k)
    {
        double epsilon = 1e-25;
        if(std::abs(_i.dot(_j)) < epsilon &&
           std::abs(_i.dot(_k)) < epsilon &&
           std::abs(_j.dot(_k)) < epsilon)
        {
            k = Vec3d::norm(_k);
            i = Vec3d::norm(_i);
            j = Vec3d::norm(_j);
        }

        iRot = i;
        jRot = j;
        kRot = k;
    }

    ONormalSet() {iRot=i; jRot=j; kRot=k;}

    ONormalSet(Vec3d planeDirection)
    {
        iRot=i; jRot=j; kRot=k;

        Vec3d orthogVector = Vec3d::cross(kRot, Vec3d::norm(planeDirection));
        double cosine = Vec3d::norm(planeDirection).dot(kRot);
        double angle = 180.0 * acos(cosine) / Constants::PI;
        if(std::abs(cosine - 1.0) > 1e-20)
            absRotate(-angle, Vec3d::norm(orthogVector));

        if(std::abs(kRot.dot(k)) < 1.0)
        {
            cosine = jRot.z / sqrt(1-kRot.z*kRot.z);
            angle = 180.0 * acos(cosine) / Constants::PI;
            if(angle > 90.0)
                angle = 180.0 - angle;
            relRotate(angle, kRot);
        }
    }

    void absRotate(double angleInDeg, Vec3d axis)
    {
        iRot = i.rotate(angleInDeg, axis);
        jRot = j.rotate(angleInDeg, axis);
        kRot = k.rotate(angleInDeg, axis);
    }

    void relRotate(double angleInDeg, Vec3d axis)
    {
        iRot = iRot.rotate(angleInDeg, axis);
        jRot = jRot.rotate(angleInDeg, axis);
        kRot = kRot.rotate(angleInDeg, axis);
    }
};

inline
sf::Color operator * (double value, sf::Color origColor)
{
    origColor.r = (sf::Uint8)(value * (double)origColor.r);
    origColor.g = (sf::Uint8)(value * (double)origColor.g);
    origColor.b = (sf::Uint8)(value * (double)origColor.b);

    return origColor;
}

#endif // MATH_UTILS_HPP
