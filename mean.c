#include <stdio.h>
#include <windows.h>

void ComputeMean(double numbers[], double* mean) {
    double sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum += numbers[i];
    }
    *mean = sum / 3;
}

int main() {
    double numbers[3], mean;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &numbers, sizeof(numbers), &dwRead, NULL) || dwRead != sizeof(numbers)) {
        fprintf(stderr, "MeanProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление среднего
    ComputeMean(numbers, &mean);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &mean, sizeof(mean), &dwWritten, NULL) || dwWritten != sizeof(mean)) {
        fprintf(stderr, "MeanProcess: WriteFile failed\n");
        return 1;
    }

    printf("MeanProcess: Mean computed and sent to parent process\n");
    return 0;
}
