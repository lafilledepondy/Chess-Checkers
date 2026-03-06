#include <stdio.h>
#include <assert.h>

#include "position.hpp"

void test_position() {
    std::cout << "--- Test Position ---\n";

    Position p(1, 5); 

    assert(p.to_string() == "A5");
    
    std::cout << "Test Position : OK\n\n";
}