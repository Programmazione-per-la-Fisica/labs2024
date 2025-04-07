#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include <stdexcept>
#include <vector>

struct Result {
  double A;
  double B;
};

struct Point {
  double x;
  double y;
};

bool operator==(Point const& lhs, Point const& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

class Regression {
  std::vector<Point> points_;

 public:
  auto size() const { return points_.size(); }

  const std::vector<Point>& points() const { return points_; }

  void add(Point const& p) { points_.push_back(p); }

  void add(double x, double y) { add(Point{x, y}); }

  void add(std::vector<Point> const& new_points) {
    points_.insert(points_.end(), new_points.begin(), new_points.end());
  }

  bool remove(Point const& to_be_removed) {
    for (auto it = points_.begin(), end = points_.end(); it != end; ++it) {
      if (*it == to_be_removed) {
        points_.erase(it);
        return true;
      }
    }
    return false;
  }

  bool remove(double x, double y) {
    return remove(Point{x, y});
  }

  auto fit() const {
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

  Regression& operator+=(Regression const& r) {
    add(r.points_);
    return *this;
  }
};

Regression operator+(Regression const& l, Regression const& r) {
  Regression sum{l};
  return sum += r;
}

auto fit(Regression const& reg) {
  // the following call would fail compilation if the fit method weren't const
  return reg.fit();
}

TEST_CASE("Testing Regression") {
  Regression reg;
  Point p1{1., 2.};
  Point p2{2., 3.};

  REQUIRE(reg.size() == 0);

  SUBCASE("Fitting with no points throws") { CHECK_THROWS(reg.fit()); }

  SUBCASE("Fitting on one point throws - use coordinates") {
    reg.add(0., 0.);
    CHECK_THROWS(reg.fit());
  }

  SUBCASE(
      "Fitting on two, vertically aligned, points throws - use coordinates") {
    reg.add(1., 0.);
    reg.add(1., 2.);
    CHECK_THROWS(reg.fit());
  }

  SUBCASE("Fitting on two points - use coordinates") {
    reg.add(0., 0.);
    reg.add(1., 1.);
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(0));
    CHECK(result.B == doctest::Approx(1));
  }

  SUBCASE("Fitting on two points, negative slope - use coordinates") {
    reg.add(0., 1.);
    reg.add(1., 0.);
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(1));
    CHECK(result.B == doctest::Approx(-1));
  }

  SUBCASE("Fitting on five points - use coordinates") {
    reg.add(2.1, 3.2);
    reg.add(6.9, 7.3);
    reg.add(2.2, 3.0);
    reg.add(0.1, 1.3);
    reg.add(4.7, 5.6);
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(1.2).epsilon(0.01));
    CHECK(result.B == doctest::Approx(0.9).epsilon(0.01));
  }

  SUBCASE("Fitting on one point throws - use Point") {
    reg.add(Point{0., 0.});
    CHECK_THROWS(reg.fit());
  }

  SUBCASE("Fitting on two, vertically aligned, points throws - use Point") {
    reg.add(Point{1., 0.});
    reg.add(Point{1., 2.});
    CHECK_THROWS(reg.fit());
  }

  SUBCASE("Fitting on two points - use Point") {
    reg.add(Point{0., 0.});
    reg.add(Point{1., 1.});
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(0));
    CHECK(result.B == doctest::Approx(1));
  }

  SUBCASE("Fitting on two points, negative slope - use Point") {
    reg.add(Point{0., 1.});
    reg.add(Point{1., 0.});
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(1));
    CHECK(result.B == doctest::Approx(-1));
  }

  SUBCASE("Fitting on five points - use Point") {
    reg.add(Point{2.1, 3.2});
    reg.add(Point{6.9, 7.3});
    reg.add(Point{2.2, 3.0});
    reg.add(Point{0.1, 1.3});
    reg.add(Point{4.7, 5.6});
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(1.2).epsilon(0.01));
    CHECK(result.B == doctest::Approx(0.9).epsilon(0.01));
  }

  SUBCASE("Removing an existing point - remove with coordinates") {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(1., 2.) == true);
    CHECK(reg.size() == 1);
  }

  SUBCASE("Removing a non-existing point - remove with coordinates") {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(5., 4.) == false);
    CHECK(reg.size() == 2);
  }

  SUBCASE("Removing an existing point - remove with Point") {
    reg.add(p1);
    reg.add(p2);
    CHECK(reg.remove(p1) == true);
    CHECK(reg.size() == 1);
  }

  SUBCASE("Removing a non-existing point - remove with Point") {
    reg.add(p1);
    reg.add(p2);
    Point p3{5., 4.};
    CHECK(reg.remove(p3) == false);
    CHECK(reg.size() == 2);
  }
}

TEST_CASE("Testing sum of two Regression objects") {
  Regression reg1;
  reg1.add(0., 1.);
  Regression reg2;
  reg2.add(2., 3.);
  auto sum = reg1 + reg2;
  CHECK(sum.size() == 2);
  if (sum.size() == 2) {
    CHECK(sum.points()[0].x == doctest::Approx(0.0));
    CHECK(sum.points()[0].y == doctest::Approx(1.0));
    CHECK(sum.points()[1].x == doctest::Approx(2.0));
    CHECK(sum.points()[1].y == doctest::Approx(3.0));
  }
}
