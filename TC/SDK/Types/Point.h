#pragma once

namespace sdk
{
    //------------------------------------------------------------------------------
    template <class T>
    struct Point2 {
    public:
        using value_type = T;

        Point2() : x_(0), y_(0) {}
        explicit Point2(T x) : x_(x), y_(x) {}
        Point2(T x, T y) : x_(x), y_(y) {}

        T x() const { return x_; }
        T y() const { return y_; }
        T& x() { return x_; }
        T& y() { return y_; }

        // Comparison operators
        bool operator == (const Point2<T>& v) const { return x_ == v.x_ && y_ == v.y_; }
        bool operator != (const Point2<T>& v) const { return x_ != v.x_ || y_ != v.y_; }

        // Assignment operators
        Point2& operator= (const Point2& v) { x_ = v.x_; y_ = v.y_; return *this; }
        Point2& operator+= (const Point2& v) { x_ += v.x_; y_ += v.y_; return *this; }
        Point2& operator-= (const Point2& v) { x_ -= v.x_; y_ -= v.y_; return *this; }
        Point2& operator*= (float s) { x_ *= s; y_ *= s; return *this; }
        Point2& operator/= (float s) { x_ /= s; y_ /= s; return *this; }

        // Unary operators
        Point2 operator+ () const { return *this; }
        Point2 operator- () const { return Point2(-x, -y); }

    private:
        T x_;
        T y_;
    };

    // Binary operators
    template <class T> Point2<T> operator+ (const Point2<T>& v1, const Point2<T>& v2) { return Point2<T>(v1.x() + v2.x(), v1.y() + v2.y()); }
    template <class T> Point2<T> operator- (const Point2<T>& v1, const Point2<T>& v2) { return Point2<T>(v1.x() - v2.x(), v1.y() - v2.y()); }
    template <class T> Point2<T> operator* (const Point2<T>& v, float s) { return Point2<T>(v.x() * s, v.y() * s); }
    template <class T> Point2<T> operator/ (const Point2<T>& v, float s) { return Point2<T>(v.x() / s, v.y() / s); }
    template <class T> Point2<T> operator* (float s, const Point2<T>& v) { return Point2<T>(s * v.x(), s * v.y()); }

    template <class T> T dot(const Point2<T>& v1, const Point2<T>& v2) { return v1.x() * v2.x() + v1.y() * v2.y(); }
    template <class T> T cross(const Point2<T>& v1, const Point2<T>& v2) { return v1.x() * v2.y() - v1.y() * v2.x(); }

    // Input : dx, dy : coordinates of a(difference) vector.
    // Output : a number from the range[-2 .. 2] which is monotonic
    //         in the angle this vector makes against the x axis.
    //         and with the same discontinuity as atan2
    template <class T>
    inline double pseudoangle(Point2<T> p)
    {
        return std::copysign(1. - p.x() / (fabs(p.x()) + fabs(p.y())), p.y());
    }

    // Checks if p2 follows p1 in ccw direction wrt reference
    // Returns > 0 if ccw, < 0 if cw and 0 otherwise
    template <class T>
    inline T ccw(Point2<T> reference, Point2<T> p1, Point2<T> p2)
    {
        return cross(p1 - reference, p2 - reference);
    }
}

