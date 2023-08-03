#include <vector>
#include <memory>

struct SplineImpl;

class Spline {
public:
    // Build a spline through points (x[0], y[0]), (x[1], y[1]).
    Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b);
    ~Spline() = default;

    // Get spline value at a given point.
    double Interpolate(double x);

private:
    std::shared_ptr<SplineImpl> impl_;
};
