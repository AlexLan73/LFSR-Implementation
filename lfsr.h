#ifndef LFSR_H
#define LFSR_H

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

/**
 * @class LFSR
 * @brief Linear Feedback Shift Register implementation
 * 
 * LFSR generates pseudorandom sequences using linear feedback.
 * Supports register sizes from 3 to 16 bits with primitive polynomials
 * for maximum period (2^n - 1).
 */
class LFSR {
private:
    uint16_t register_state;     // Current state of the shift register
    uint16_t polynomial_mask;    // Mask for feedback polynomial
    uint8_t register_size;       // Size of the register (3-16 bits)
    uint32_t period_counter;     // Counter to track period
    uint32_t max_period;         // Maximum possible period (2^n - 1)
    
    // Primitive polynomials for different register sizes
    // These ensure maximum period of 2^n - 1
    static const std::vector<uint16_t> PRIMITIVE_POLYNOMIALS;
    
    /**
     * @brief Calculate the next bit using XOR feedback
     * @return The next output bit
     */
    bool calculateNextBit();
    
    /**
     * @brief Validate register size
     * @param size Register size to validate
     * @throw std::invalid_argument if size is not in range [3, 16]
     */
    void validateSize(uint8_t size) const;

public:
    /**
     * @brief Constructor
     * @param size Register size (3-16 bits)
     * @param initial_seed Initial state (0 means use default)
     * @throw std::invalid_argument if size is invalid
     */
    explicit LFSR(uint8_t size, uint16_t initial_seed = 0);
    
    /**
     * @brief Generate next bit in the sequence
     * @return Next pseudorandom bit
     */
    bool nextBit();
    
    /**
     * @brief Generate next byte (8 bits)
     * @return Next pseudorandom byte
     */
    uint8_t nextByte();
    
    /**
     * @brief Generate next 16-bit word
     * @return Next pseudorandom 16-bit word
     */
    uint16_t nextWord();
    
    /**
     * @brief Get current register state
     * @return Current state as 16-bit value
     */
    uint16_t getState() const { return register_state; }
    
    /**
     * @brief Set new register state
     * @param new_state New state (must be non-zero)
     * @throw std::invalid_argument if state is zero
     */
    void setState(uint16_t new_state);
    
    /**
     * @brief Get register size
     * @return Size of the register in bits
     */
    uint8_t getSize() const { return register_size; }
    
    /**
     * @brief Get current period counter
     * @return Number of bits generated since last reset
     */
    uint32_t getPeriodCounter() const { return period_counter; }
    
    /**
     * @brief Get maximum possible period
     * @return Maximum period (2^n - 1)
     */
    uint32_t getMaxPeriod() const { return max_period; }
    
    /**
     * @brief Check if sequence has completed one full period
     * @return true if period is complete
     */
    bool isPeriodComplete() const { return period_counter >= max_period; }
    
    /**
     * @brief Reset the generator to initial state
     * @param new_seed New seed value (0 means use current state)
     */
    void reset(uint16_t new_seed = 0);
    
    /**
     * @brief Get string representation of current state
     * @return Binary string representation
     */
    std::string getStateString() const;
    
    /**
     * @brief Get polynomial representation
     * @return String representation of the feedback polynomial
     */
    std::string getPolynomialString() const;
    
    /**
     * @brief Generate sequence and check for repetition
     * @param max_bits Maximum number of bits to generate
     * @return Vector of generated bits
     */
    std::vector<bool> generateSequence(uint32_t max_bits = 0);
    
    /**
     * @brief Test the generator for proper operation
     * @return true if test passes
     */
    bool selfTest();
};

#endif // LFSR_H