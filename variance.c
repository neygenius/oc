#include <stdio.h>
#include <windows.h>

void ComputeVariance(double sum_of_squares, double* variance) {
    *variance = sum_of_squares / 2;
}

int main() {
    double sum_of_squares, variance;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &sum_of_squares, sizeof(sum_of_squares), &dwRead, NULL) || dwRead != sizeof(sum_of_squares)) {
        fprintf(stderr, "VarianceProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление дисперсии
    ComputeVariance(sum_of_squares, &variance);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &variance, sizeof(variance), &dwWritten, NULL) || dwWritten != sizeof(variance)) {
        fprintf(stderr, "VarianceProcess: WriteFile failed\n");
        return 1;
    }

    printf("VarianceProcess: Variance computed and sent to parent process\n");
    return 0;
}
