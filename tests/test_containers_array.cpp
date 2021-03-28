#include "catch.hpp"
#include "../src/containers/array.hpp"

TEST_CASE("Test Array initialization", "[Containers::Array]") {
    const size_t array_size = 5;
    Array<int, array_size> a;

    REQUIRE(a.Length() == array_size);
    REQUIRE(a.Size() == array_size * sizeof(int));
}

TEST_CASE("Test Array read & write", "[Containers::Array]") {
    const size_t array_size = 5;
    Array<int, array_size> a;
    
    a[0] = 1;
    a[2] = 3;
    a[4] = 5;

    REQUIRE(a[0] == 1);
    REQUIRE(a[2] == 3);
    REQUIRE(a[4] == 5);
}