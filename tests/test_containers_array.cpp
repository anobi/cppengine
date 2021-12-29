#include "catch.hpp"
#include "../src/containers/array.hpp"

TEST_CASE("Test Array initialization", "[Containers::Array]") {
    const size_t array_size = 5;
    Array<int, array_size> a;

    REQUIRE(a.Length() == array_size);
    REQUIRE(a.Size() == array_size * sizeof(int));
    REQUIRE(a[0] == 0);
    REQUIRE(a[array_size - 1] == 0);
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

TEST_CASE("Test Array clear function", "[Containers::Array]") {
    const size_t array_size = 5;
    Array<int, array_size> a;

    // Init the array with positive, non-zero numbers
    for (unsigned int i = 0; i < array_size; i++) {
        a[i] = i + 1;
    }

    // Check that it is in fact filled with positive numbers! Because why not.
    for (unsigned int i = 0; i < array_size; i++) {
        REQUIRE(a[0] != 0);
    }

    // Clear it and check that the content is all zeroes
    a.Clear();
    for (unsigned int i = 0; i < array_size; i++) {
        REQUIRE(a[0] == 0);
    }
}

TEST_CASE("Test Array copy", "[Containers::Array::operator=]") {
    Array<int, 3> a1;
    Array<int, 3> a2;

    a1[0] = 1;
    a1[1] = 2;
    a1[2] = 3;
    REQUIRE(a1[0] != a2[0]);
    REQUIRE(a1[1] != a2[1]);
    REQUIRE(a1[2] != a2[2]);

    a2 = a1;
    REQUIRE(a1[0] == a2[0]);
    REQUIRE(a1[1] == a2[1]);
    REQUIRE(a1[2] == a2[2]);
}