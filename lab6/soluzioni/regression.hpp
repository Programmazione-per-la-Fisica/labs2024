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

bool operator==(Point const&, Point const&);

class Regression {
  std::vector<Point> points_;

 public:
  size_t size() const;
  const std::vector<Point>& points() const;
  void add(Point const&);
  void add(double x, double y);
  void add(std::vector<Point> const&);
  Result fit() const;
  Result fit_algo() const;
  bool remove(Point const&);
  bool remove(double x, double y);
  Regression& operator+=(Regression const&);
  void sort_by_x();
  void sort_by_y();
  double median_x();
  double median_y();
  double median_x_nth() const;
  double median_y_nth() const;
};

Regression operator+(Regression const&, Regression const&);
Result fit(Regression const&);

}  // namespace pf

#endif