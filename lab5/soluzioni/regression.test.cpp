#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "regression.hpp"

#include "doctest.h"

TEST_CASE("Testing Regression") {
  pf::Regression reg;
  pf::Point p1{1., 2.};
  pf::Point p2{2., 3.};

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
    reg.add(pf::Point{0., 0.});
    CHECK_THROWS(reg.fit());
  }

  SUBCASE("Fitting on two, vertically aligned, points throws - use Point") {
    reg.add(pf::Point{1., 0.});
    reg.add(pf::Point{1., 2.});
    CHECK_THROWS(reg.fit());
  }

  SUBCASE("Fitting on two points - use Point") {
    reg.add(pf::Point{0., 0.});
    reg.add(pf::Point{1., 1.});
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(0));
    CHECK(result.B == doctest::Approx(1));
  }

  SUBCASE("Fitting on two points, negative slope - use Point") {
    reg.add(pf::Point{0., 1.});
    reg.add(pf::Point{1., 0.});
    auto result = reg.fit();
    CHECK(result.A == doctest::Approx(1));
    CHECK(result.B == doctest::Approx(-1));
  }

  SUBCASE("Fitting on five points - use Point") {
    reg.add(pf::Point{2.1, 3.2});
    reg.add(pf::Point{6.9, 7.3});
    reg.add(pf::Point{2.2, 3.0});
    reg.add(pf::Point{0.1, 1.3});
    reg.add(pf::Point{4.7, 5.6});
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
    pf::Point p3{5., 4.};
    CHECK(reg.remove(p3) == false);
    CHECK(reg.size() == 2);
  }
}

TEST_CASE("Testing sum of two Regression objects") {
  pf::Regression reg1;
  reg1.add(0., 1.);
  pf::Regression reg2;
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
