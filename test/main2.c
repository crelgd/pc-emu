#include <stdio.h>

int main() {
    FILE *file;
    unsigned char buffer[1]; // Буфер для хранения одного байта данных

    file = fopen("program.em", "rb"); // Открытие файла для чтения в бинарном режиме ("rb")

    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Чтение и вывод содержимого файла по байтам
    while (fread(buffer, sizeof(unsigned char), 1, file) == 1) {
        printf("%u ", buffer[0]); // Вывод байта как число
    }

    fclose(file); // Закрытие файла

    return 0;
}
