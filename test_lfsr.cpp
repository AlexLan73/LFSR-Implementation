#include "lfsr.h"
#include <iostream>
#include <bitset>

int main() {
    std::cout << "=== Simple LFSR Test ===\n\n";
    
    // Test 3-bit LFSR
    std::cout << "Testing 3-bit LFSR:\n";
    LFSR lfsr3(3, 1);  // Start with state 001
    
    std::cout << "Polynomial: " << lfsr3.getPolynomialString() << "\n";
    std::cout << "Initial state: " << lfsr3.getStateString() << "\n";
    std::cout << "Max period: " << lfsr3.getMaxPeriod() << "\n\n";
    
    std::cout << "Sequence (should be 7 bits before repeating):\n";
    for (int i = 0; i < 10; i++) {
        bool bit = lfsr3.nextBit();
        std::cout << "Step " << i+1 << ": " << lfsr3.getStateString() 
                  << " -> " << (bit ? '1' : '0') << "\n";
    }
    
    std::cout << "\nTesting period completion:\n";
    bool test_result = lfsr3.selfTest();
    std::cout << "Period test: " << (test_result ? "PASSED" : "FAILED") << "\n";
    
    std::cout << "\n=== Test Complete ===\n";
    return 0;
}