#include "ugly.h"

#include <memory>
#include <type_traits>

#include "bad.h"

struct SplineImpl {
    std::vector<double> x, y, y2;
    double a, b;

    SplineImpl(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
        : x(x), y(y), a(a), b(b) {
        y2.resize(x.size());
        mySplineSnd(x.data(), y.data(), x.size(), a, b, y2.data());
    }

    double Interpolate(double value) {
        double ans = 0.;
        mySplintCube(x.data(), y.data(), y2.data(), x.size(), value, &ans);
        return ans;
    }
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
    : impl_(std::make_shared<SplineImpl>(x, y, a, b)) {
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}
