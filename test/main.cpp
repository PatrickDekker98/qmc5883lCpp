#include "ostream"
#include <vector.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Equality, equal") {
  vector v(3, 4);
  REQUIRE(v == vector(3, 4));
}

TEST_CASE("Equality, unequal") {
  vector v(4, 4);
  REQUIRE(!(v == vector(3, 4)));
}

TEST_CASE("Constructor, zero_parameters") {
  vector v;
  std::stringstream s;
  s << v;
  REQUIRE(s.str() == "[0/0]");
}

TEST_CASE("Constructor, two_parameters") {
  vector v(3, 4);
  std::stringstream s;
  s << v;
  REQUIRE(s.str() == "[3/4]");
}

TEST_CASE("Add two vectors") {
  vector v(4, 3);
  vector x = v + vector(5, 4);
  REQUIRE(x == vector(9, 8));
}

TEST_CASE("Add one vectors") {
  vector v(4, 4);
  REQUIRE(v == +v);
}

TEST_CASE("Add vector into vector") {
  vector v(4, 4);
  vector x(5, 8);
  REQUIRE(vector(9, 12) == (v += x));
}

TEST_CASE("Multiply vector by int") {
  vector v(4, 3);
  vector x = v * 3;
  REQUIRE(x == vector(12, 9));
}

TEST_CASE("Multiply int by vector") {
  vector v(4, 3);
  vector x = 3 * v;
  REQUIRE(x == vector(12, 9));
}

TEST_CASE("Multiply int into vector") {
  vector v(4, 11);
  REQUIRE(vector(12, 33) == (v * 3));
}
