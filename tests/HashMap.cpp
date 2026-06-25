#include <iostream>
#include <string>
#include <cassert>
#include <stdexcept>
#include "HashMap.h"

void testSetAndGet() {
    HashMap<std::string, int> map;
    map.set("Alice", 100);
    map.set("Bob", 200);

    assert(map.get("Alice") == 100);
    assert(map.get("Bob") == 200);

    map.set("Alice", 999);
    assert(map.get("Alice") == 999);
}

void testGetException() {
    HashMap<std::string, int> map;
    map.set("Real", 1);

    bool caught = false;
    try {
        map.get("Ghost");
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
}

void testRehashing() {
    HashMap<std::string, int> map;

    for (int i = 0; i < 25; ++i) {
        map.set("Key_" + std::to_string(i), i * 10);
    }

    for (int i = 0; i < 25; ++i) {
        assert(map.get("Key_" + std::to_string(i)) == i * 10);
    }
}

void testRuleOfThree() {
    HashMap<std::string, int> map1;
    map1.set("A", 1);
    map1.set("B", 2);

    HashMap<std::string, int> map2 = map1;
    assert(map2.get("A") == 1);
    assert(map2.get("B") == 2);

    map2.set("A", 99);
    assert(map1.get("A") == 1);
    assert(map2.get("A") == 99);

    HashMap<std::string, int> map3;
    map3.set("Junk", 0);
    map3 = map1;

    assert(map3.get("B") == 2);

    map3 = map3;
    assert(map3.get("B") == 2);
}

int main() {
    testSetAndGet();
    testGetException();
    testRehashing();
    testRuleOfThree();

    std::cout << "Phase 1 tests (set, get, rehash, Rule of Three) passed successfully!\n";
    return 0;
}