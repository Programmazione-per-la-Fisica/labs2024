#include "regression.hpp"

#include <algorithm>
#include <numeric>

namespace pf {
bool operator==(Point const& lhs, Point const& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool cmp_less_x(Point const& lhs, Point const& rhs) { return lhs.x < rhs.x; }

bool cmp_less_y(Point const& lhs, Point const& rhs) { return lhs.y < rhs.y; }

size_t Regression::size() const { return points_.size(); }

const std::vector<Point>& Regression::points() const { return points_; }

void Regression::add(Point const& p) { points_.push_back(p); }

void Regression::add(double x, double y) { add(Point{x, y}); }

void Regression::add(std::vector<Point> const& new_points) {
  points_.insert(points_.end(), new_points.begin(), new_points.end());
}

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

Result Regression::fit_algo() const {
  auto const N = size();
  if (N < 2) {
    throw std::runtime_error{"Not enough points to run a fit"};
  }

  // double sum_x{
  //     std::accumulate(points_.begin(), points_.end(), 0.,
  //                     [](double acc, Point const& p) { return acc + p.x; })};
  // double sum_x2{std::accumulate(
  //     points_.begin(), points_.end(), 0.,
  //     [](double acc, Point const& p) { return acc + p.x * p.x; })};
  // double sum_y{
  //     std::accumulate(points_.begin(), points_.end(), 0.,
  //                     [](double acc, Point const& p) { return acc + p.y; })};
  // double sum_xy{std::accumulate(
  //     points_.begin(), points_.end(), 0.,
  //     [](double acc, Point const& p) { return acc + p.x * p.y; })};
  // auto const d = N * sum_x2 - sum_x * sum_x;
  // if (d == 0.) {
  //   throw std::runtime_error{"Trying to fit a vertical line"};
  // }
  // auto const a = (sum_y * sum_x2 - sum_x * sum_xy) / d;
  // auto const b = (N * sum_xy - sum_x * sum_y) / d;
  // return Result{a, b};

  struct Sums {
    double sum_x;
    double sum_x2;
    double sum_y;
    double sum_xy;
  };

  auto [sum_x, sum_x2, sum_y, sum_xy] =
      std::accumulate(points_.begin(), points_.end(), Sums{0., 0., 0., 0.},
                      [](Sums acc, Point const& p) {
                        acc.sum_x += p.x;
                        acc.sum_x2 += p.x * p.x;
                        acc.sum_y += p.y;
                        acc.sum_xy += p.x * p.y;
                        return acc;
                      });

  auto const d = N * sum_x2 - sum_x * sum_x;
  if (d == 0.) {
    throw std::runtime_error{"Trying to fit a vertical line"};
  }
  auto const a = (sum_y * sum_x2 - sum_x * sum_xy) / d;
  auto const b = (N * sum_xy - sum_x * sum_y) / d;
  return Result{a, b};
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

Regression& Regression::operator+=(Regression const& r) {
  add(r.points_);
  return *this;
}

void Regression::sort_by_x() {
  std::sort(points_.begin(), points_.end(), cmp_less_x);
}

void Regression::sort_by_y() {
  std::sort(points_.begin(), points_.end(), cmp_less_y);
}

Regression operator+(Regression const& l, Regression const& r) {
  Regression sum{l};
  return sum += r;
}

Result fit(Regression const& reg) {
  // the following call would fail compilation if the fit method weren't const
  return reg.fit();
}

double Regression::median_x() {
  const std::size_t N = size();
  if (N == 0) {
    throw std::runtime_error{"Not enough points to compute the median"};
  }

  sort_by_x();
  auto& p_sorted = points();
  return N % 2 == 0 ? (p_sorted[N / 2 - 1].x + p_sorted[N / 2].x) / 2.0
                    : p_sorted[N / 2].x;
}

double Regression::median_y() {
  const std::size_t N = size();
  if (N == 0) {
    throw std::runtime_error{"Not enough points to compute the median"};
  }

  sort_by_y();
  auto& p_sorted = points();
  return N % 2 == 0 ? (p_sorted[N / 2 - 1].y + p_sorted[N / 2].y) / 2.0
                    : p_sorted[N / 2].y;
}

double Regression::median_x_nth() const {
  const std::size_t N = size();
  if (N == 0) {
    throw std::runtime_error{"Not enough points to compute the median"};
  }

  auto p = points();
  auto it = p.begin() + N / 2;

  std::nth_element(p.begin(), it, p.end(), cmp_less_x);

  auto it_prev = std::max_element(p.begin(), it, cmp_less_x);

  return N % 2 == 0 ? ((*it).x + (*it_prev).x) / 2.0 : (*it).x;
}

double Regression::median_y_nth() const {
  const std::size_t N = size();
  if (N == 0) {
    throw std::runtime_error{"Not enough points to compute the median"};
  }

  auto p = points();
  auto it = p.begin() + N / 2;

  std::nth_element(p.begin(), it, p.end(), cmp_less_y);

  auto it_prev = std::max_element(p.begin(), it, cmp_less_y);

  return N % 2 == 0 ? ((*it).y + (*it_prev).y) / 2.0 : (*it).y;
}

}  // namespace pf