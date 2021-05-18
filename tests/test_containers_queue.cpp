#include "catch.hpp"
#include "../src/containers/queue.hpp"

TEST_CASE("Test Queue initialization", "[Containers::Queue]") {
    const size_t queue_size = 5;
    Queue<int, queue_size> q;

    REQUIRE(q.Length() == 0);
    REQUIRE(q.Size() == queue_size * sizeof(int));
    REQUIRE(q.Front() == 0);
    REQUIRE(q.Back() == 0);
}

TEST_CASE("Test Queue items", "[Containers::Queue.Add]") {
    const size_t Queue_size = 5;
    Queue<int, Queue_size> q;

    q.Add(1);
    q.Add(2);
    q.Add(3);

    REQUIRE(q.Length() == 3);
    REQUIRE(q.Back() == 1);
    REQUIRE(q.Front() == 3);
}

//TEST_CASE("Test Queue emptying", "[Containers::Queue.Empty]") {
//    const size_t Queue_size = 5;
//    Queue<int, Queue_size> q;
//
//    // Init the Queue with positive, non-zero numbers
//    for (int i = 0; i < Queue_size; i++) {
//        q.Add(i + 1);
//    }
//
//    // Check that it is in fact filled with positive numbers! Because why not.
//    for (int i = 0; i < Queue_size; i++) {
//        REQUIRE(q.Length() == 5);
//    }
//
//    q.Clear();
//    for (int i = 0; i < Queue_size; i++) {
//        REQUIRE(q.Length() == 0);
//    }
//}