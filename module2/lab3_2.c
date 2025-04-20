#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Преобразование строки IP в целое число
uint32_t ip_to_int(const char *ip_str) {
    uint8_t octets[4];
    if (sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", 
               &octets[0], &octets[1], 
               &octets[2], &octets[3]) != 4) {
        fprintf(stderr, "Invalid IP format: %s\n", ip_str);
        exit(EXIT_FAILURE);
    }
    return (octets[0] << 24) | (octets[1] << 16) | 
           (octets[2] << 8) | octets[3];
}

// Проверка корректности маски подсети
int is_valid_mask(uint32_t mask) {
    uint32_t inverted = ~mask;
    return (inverted & (inverted + 1)) == 0;
}

// Генерация полностью случайного IP-адреса
uint32_t generate_random_ip() {
    return ((uint32_t)(rand() & 0xFF) << 24) |
           ((rand() & 0xFF) << 16) |
           ((rand() & 0xFF) << 8) |
           (rand() & 0xFF);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <gateway> <mask> <N>\n", argv[0]);
        return 1;
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Парсинг параметров
    uint32_t gateway = ip_to_int(argv[1]);
    uint32_t mask = ip_to_int(argv[2]);
    int N = atoi(argv[3]);
    
    if (!is_valid_mask(mask)) {
        fprintf(stderr, "Invalid subnet mask!\n");
        return 1;
    }

    uint32_t subnet = gateway & mask;

    // Отладочный вывод
    printf("Gateway: 0x%08X\n", gateway);
    printf("Mask:    0x%08X\n", mask);
    printf("Subnet:  0x%08X\n", subnet);

    // Обработка пакетов
    int same_subnet = 0;
    for (int i = 0; i < N; i++) {
        uint32_t ip = generate_random_ip();
        
        // Отладочный вывод (раскомментируйте для проверки)
        // printf("Generated IP: 0x%08X\n", ip);

        if ((ip & mask) == subnet) {
            same_subnet++;
        }
    }

    // Вывод статистики
    printf("\nResults:\n");
    printf("Same subnet: %d (%.2f%%)\n", 
           same_subnet, (double)same_subnet / N * 100);
    printf("Other networks: %d (%.2f%%)\n", 
           N - same_subnet, (double)(N - same_subnet) / N * 100);

    return 0;
}