

#include "regression.hpp"

#include <stdexcept>

namespace pf {

bool operator==(Point const& lhs, Point const& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

int Regression::size() const { return points_.size(); }

const std::vector<Point>& Regression::points() const { return points_; }

void Regression::add(Point const& p) { points_.push_back(p); }

void Regression::add(double x, double y) { add(Point{x, y}); }

void Regression::add(std::vector<Point> const& new_points) {
  points_.insert(points_.end(), new_points.begin(), new_points.end());
}

bool Regression::remove(Point const& to_be_removed) {
  for (auto it = points_.begin(), end = points_.end(); it != end; ++it) {
    if (*it == to_be_removed) {
      points_.erase(it);
      return true;
    }
  }
  return false;
}

bool Regression::remove(double x, double y) { return remove(Point{x, y}); }

Result Regression::fit() const {
  auto const N = size();
  if (N < 2) {
    throw std::runtime_error{"Not enough points to run a fit"};
  }

  double sum_x{};
  double sum_x2{};
  double sum_y{};
  double sum_xy{};
  for (auto const& p : points_) {
    sum_x += p.x;
    sum_x2 += p.x * p.x;
    sum_y += p.y;
    sum_xy += p.x * p.y;
  }

  auto const d = N * sum_x2 - sum_x * sum_x;
  if (d == 0.) {
    throw std::runtime_error{"Trying to fit a vertical line"};
  }
  auto const a = (sum_y * sum_x2 - sum_x * sum_xy) / d;
  auto const b = (N * sum_xy - sum_x * sum_y) / d;
  return Result{a, b};
}

Regression& Regression::operator+=(Regression const& r) {
  add(r.points_);
  return *this;
}

Regression operator+(Regression const& l, Regression const& r) {
  Regression sum{l};
  return sum += r;
}

Result fit(Regression const& reg) {
  // the following call would fail compilation if the fit method weren't const
  return reg.fit();
}

}  // namespace pf
