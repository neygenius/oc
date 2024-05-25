#include <stdio.h>
#include <windows.h>

void ComputeSum(double values[], double* sum) {
    double temp = 0;
    for (int i = 0; i < 3; ++i) {
        temp += values[i];
    }
    *sum = temp;
}

int main() {
    double values[3], sum;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &values, sizeof(values), &dwRead, NULL) || dwRead != sizeof(values)) {
        fprintf(stderr, "SumProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление суммы
    ComputeSum(values, &sum);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &sum, sizeof(sum), &dwWritten, NULL) || dwWritten != sizeof(sum)) {
        fprintf(stderr, "SumProcess: WriteFile failed\n");
        return 1;
    }

    printf("SumProcess: Sum computed and sent to parent process\n");
    return 0;
}
