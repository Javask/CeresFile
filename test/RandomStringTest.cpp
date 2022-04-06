#include <catch2/catch.hpp>
#include "RandomString.h"



TEST_CASE("Generated Random String", "[utility]") {
  auto str = generateRandomString(8);
  REQUIRE(str.size() == 8);
}
