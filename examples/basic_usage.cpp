/**
 * @file basic_usage.cpp
 * @brief Базовые примеры использования LFSR
 * @author Александр (AlexLan73)
 * @date 2025
 */

#include "../lfsr.h"
#include <iostream>
#include <iomanip>
#include <bitset>

int main() {
    std::cout << "=== Базовые примеры использования LFSR ===\n\n";
    
    // Пример 1: Простая генерация битов
    std::cout << "1. Генерация битов (3-битный LFSR):\n";
    LFSR lfsr1(3, 1);  // Начальное состояние: 001
    
    std::cout << "   Полином: " << lfsr1.getPolynomialString() << "\n";
    std::cout << "   Начальное состояние: " << lfsr1.getStateString() << "\n";
    std::cout << "   Последовательность: ";
    
    for (int i = 0; i < 10; i++) {
        bool bit = lfsr1.nextBit();
        std::cout << (bit ? '1' : '0');
        if ((i + 1) % 4 == 0) std::cout << " ";
    }
    std::cout << "\n\n";
    
    // Пример 2: Генерация байтов
    std::cout << "2. Генерация байтов (8-битный LFSR):\n";
    LFSR lfsr2(8, 0xAB);
    
    std::cout << "   Полином: " << lfsr2.getPolynomialString() << "\n";
    std::cout << "   Начальное состояние: 0x" << std::hex << lfsr2.getState() << std::dec << "\n";
    std::cout << "   Сгенерированные байты: ";
    
    for (int i = 0; i < 5; i++) {
        uint8_t byte = lfsr2.nextByte();
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << "\n\n";
    
    // Пример 3: Анализ состояния
    std::cout << "3. Анализ состояния (4-битный LFSR):\n";
    LFSR lfsr3(4, 0x5);
    
    std::cout << "   Полином: " << lfsr3.getPolynomialString() << "\n";
    std::cout << "   Максимальный период: " << lfsr3.getMaxPeriod() << " бит\n";
    
    std::cout << "   Шаги генерации:\n";
    for (int i = 0; i < 8; i++) {
        bool bit = lfsr3.nextBit();
        std::cout << "   Шаг " << std::setw(2) << (i + 1) << ": " 
                  << lfsr3.getStateString() << " -> " << (bit ? '1' : '0') << "\n";
    }
    std::cout << "\n";
    
    // Пример 4: Проверка периода
    std::cout << "4. Проверка периода:\n";
    LFSR lfsr4(3, 1);
    
    std::cout << "   Тест периода для 3-битного LFSR...\n";
    bool test_result = lfsr4.selfTest();
    std::cout << "   Результат: " << (test_result ? "ПРОЙДЕН" : "НЕ ПРОЙДЕН") << "\n";
    std::cout << "   Сгенерировано бит: " << lfsr4.getPeriodCounter() << "\n";
    std::cout << "   Ожидаемый период: " << lfsr4.getMaxPeriod() << "\n\n";
    
    // Пример 5: Сброс и повторное использование
    std::cout << "5. Сброс и повторное использование:\n";
    LFSR lfsr5(5, 0x1F);
    
    std::cout << "   Начальное состояние: " << lfsr5.getStateString() << "\n";
    
    // Генерируем несколько битов
    for (int i = 0; i < 5; i++) {
        lfsr5.nextBit();
    }
    std::cout << "   После 5 битов: " << lfsr5.getStateString() << "\n";
    
    // Сбрасываем
    lfsr5.reset(0x1F);
    std::cout << "   После сброса: " << lfsr5.getStateString() << "\n\n";
    
    std::cout << "=== Примеры завершены ===\n";
    return 0;
}