#include <stdio.h>
#include <windows.h>

void ComputeSqDiff(double diff[], double sq_diff[]) {
    for (int i = 0; i < 3; ++i) {
        sq_diff[i] = diff[i] * diff[i];
    }
}

int main() {
    double diff[3], sq_diff[3];
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &diff, sizeof(diff), &dwRead, NULL) || dwRead != sizeof(diff)) {
        fprintf(stderr, "SqDiffProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление квадратов отклонений
    ComputeSqDiff(diff, sq_diff);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &sq_diff, sizeof(sq_diff), &dwWritten, NULL) || dwWritten != sizeof(sq_diff)) {
        fprintf(stderr, "SqDiff: WriteFile failed\n");
        return 1;
    }

    printf("SqDiff: Sq diffs computed and sent to parent process\n");
    return 0;
}
