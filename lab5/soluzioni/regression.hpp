#ifndef PF_REGRESSION_HPP
#define PF_REGRESSION_HPP

#include <vector>

namespace pf {
struct Result {
  double A;
  double B;
};

struct Point {
  double x;
  double y;
};

class Regression {
  std::vector<Point> points_;

 public:
  int size() const;

  const std::vector<Point>& points() const;

  void add(Point const& p);

  void add(double x, double y);

  void add(std::vector<Point> const& new_points);

  bool remove(Point const& to_be_removed);

  bool remove(double x, double y);

  Result fit() const;

  Regression& operator+=(Regression const& r);
};

Regression operator+(Regression const& l, Regression const& r);

Result fit(Regression const& reg);

}  // namespace pf
#endif