#include <stdio.h>
#include <windows.h>

void ComputeDiff(double numbers[], double div, double diff[]) {
    for (int i = 0; i < 3; ++i) {
        diff[i] = numbers[i] - div;
    }
}

int main() {
    struct {
        double numbers[3];
        double sum;
        double div;
        double diff[3];
        double sq_diff[3];
    } input;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &input, sizeof(input), &dwRead, NULL) || dwRead != sizeof(input)) {
        fprintf(stderr, "DiffProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление отклонений
    ComputeDiff(input.numbers, input.div, input.diff);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &input.diff, sizeof(input.diff), &dwWritten, NULL) || dwWritten != sizeof(input.diff)) {
        fprintf(stderr, "DiffProcess: WriteFile failed\n");
        return 1;
    }

    printf("DiffProcess: Diffs computed and sent to parent process\n");
    return 0;
}
