#include <stdio.h>
#include <windows.h>

void ComputeDiv(double sum, double* div) {
    *div = sum / 3;
}

int main() {
    double sum, div;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &sum, sizeof(sum), &dwRead, NULL) || dwRead != sizeof(sum)) {
        fprintf(stderr, "DivProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление среднего
    ComputeDiv(sum, &div);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &div, sizeof(div), &dwWritten, NULL) || dwWritten != sizeof(div)) {
        fprintf(stderr, "DivProcess: WriteFile failed\n");
        return 1;
    }

    printf("DivProcess: Div computed and sent to parent process\n");
    return 0;
}
