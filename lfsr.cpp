#include "lfsr.h"
#include <iostream>
#include <bitset>
#include <algorithm>
#include <sstream>

// Primitive polynomials for maximum period (2^n - 1)
// Format: polynomial coefficients as bit mask (excluding x^n term)
const std::vector<uint16_t> LFSR::PRIMITIVE_POLYNOMIALS = {
    0x0000,  // n=0 (unused)
    0x0000,  // n=1 (unused)
    0x0000,  // n=2 (unused)
    0x0003,  // n=3: x^3 + x^1 + 1
    0x0009,  // n=4: x^4 + x^1 + 1
    0x0012,  // n=5: x^5 + x^2 + 1
    0x0021,  // n=6: x^6 + x^1 + 1
    0x0041,  // n=7: x^7 + x^1 + 1
    0x008E,  // n=8: x^8 + x^4 + x^3 + x^2 + 1
    0x0108,  // n=9: x^9 + x^4 + 1
    0x0204,  // n=10: x^10 + x^3 + 1
    0x0402,  // n=11: x^11 + x^2 + 1
    0x0829,  // n=12: x^12 + x^6 + x^4 + x^1 + 1
    0x100D,  // n=13: x^13 + x^4 + x^3 + x^1 + 1
    0x2015,  // n=14: x^14 + x^5 + x^3 + x^1 + 1
    0x4001,  // n=15: x^15 + x^1 + 1
    0x8016   // n=16: x^16 + x^5 + x^3 + x^2 + 1
};

LFSR::LFSR(uint8_t size, uint16_t initial_seed) 
    : register_size(size), period_counter(0) {
    
    validateSize(size);
    
    // Set polynomial mask from primitive polynomial
    polynomial_mask = PRIMITIVE_POLYNOMIALS[size];
    
    // Calculate maximum period
    max_period = (1U << size) - 1;
    
    // Set initial state
    if (initial_seed == 0) {
        // Use a default non-zero seed
        register_state = 1;
    } else {
        // Ensure seed is within valid range for register size
        uint16_t mask = (1U << size) - 1;
        register_state = initial_seed & mask;
        if (register_state == 0) {
            register_state = 1; // Avoid all-zero state
        }
    }
}

void LFSR::validateSize(uint8_t size) const {
    if (size < 3 || size > 16) {
        throw std::invalid_argument("Register size must be between 3 and 16 bits");
    }
}

bool LFSR::calculateNextBit() {
    // Calculate feedback bit using XOR of taps specified by polynomial
    bool feedback = false;
    uint16_t temp = register_state & polynomial_mask;
    
    // Count number of set bits (XOR of all taps)
    while (temp) {
        feedback ^= (temp & 1);
        temp >>= 1;
    }
    
    // Shift register right and insert feedback bit at MSB
    register_state = (register_state >> 1) | (feedback ? (1U << (register_size - 1)) : 0);
    
    // Increment period counter
    period_counter++;
    
    return feedback;
}

bool LFSR::nextBit() {
    return calculateNextBit();
}

uint8_t LFSR::nextByte() {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        result |= (calculateNextBit() ? (1U << i) : 0);
    }
    return result;
}

uint16_t LFSR::nextWord() {
    uint16_t result = 0;
    for (int i = 0; i < 16; i++) {
        result |= (calculateNextBit() ? (1U << i) : 0);
    }
    return result;
}

void LFSR::setState(uint16_t new_state) {
    if (new_state == 0) {
        throw std::invalid_argument("State cannot be zero (all-zero state is invalid)");
    }
    
    uint16_t mask = (1U << register_size) - 1;
    register_state = new_state & mask;
    period_counter = 0;
}

void LFSR::reset(uint16_t new_seed) {
    if (new_seed == 0) {
        register_state = 1; // Default non-zero state
    } else {
        uint16_t mask = (1U << register_size) - 1;
        register_state = new_seed & mask;
        if (register_state == 0) {
            register_state = 1;
        }
    }
    period_counter = 0;
}

std::string LFSR::getStateString() const {
    std::bitset<16> bits(register_state);
    std::string result = bits.to_string();
    // Show only relevant bits
    return result.substr(16 - register_size);
}

std::string LFSR::getPolynomialString() const {
    std::ostringstream oss;
    oss << "x^" << static_cast<int>(register_size);
    
    // Add lower degree terms
    for (int i = register_size - 1; i >= 0; i--) {
        if (polynomial_mask & (1U << i)) {
            if (i == 0) {
                oss << " + 1";
            } else if (i == 1) {
                oss << " + x";
            } else {
                oss << " + x^" << i;
            }
        }
    }
    
    return oss.str();
}

std::vector<bool> LFSR::generateSequence(uint32_t max_bits) {
    std::vector<bool> sequence;
    uint32_t limit = (max_bits == 0) ? max_period : std::min(max_bits, max_period);
    
    sequence.reserve(limit);
    
    for (uint32_t i = 0; i < limit; i++) {
        sequence.push_back(nextBit());
    }
    
    return sequence;
}

bool LFSR::selfTest() {
    // Test 1: Check that we don't get stuck in all-zero state
    uint16_t original_state = register_state;
    uint32_t original_counter = period_counter;
    
    // Generate a few bits and check state is not zero
    for (int i = 0; i < 10; i++) {
        nextBit();
        if (register_state == 0) {
            register_state = original_state;
            period_counter = original_counter;
            return false;
        }
    }
    
    // Test 2: Check period completion
    reset(original_state);
    uint16_t start_state = register_state;
    uint32_t bits_generated = 0;
    
    do {
        nextBit();
        bits_generated++;
        
        // Safety check to prevent infinite loop
        if (bits_generated > max_period + 100) {
            register_state = original_state;
            period_counter = original_counter;
            return false;
        }
    } while (register_state != start_state);
    
    // Restore original state
    register_state = original_state;
    period_counter = original_counter;
    
    // Check if we completed exactly one period
    return (bits_generated == max_period);
}