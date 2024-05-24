#include <stdio.h>
#include <windows.h>

void ComputeSumOfSquares(double numbers[], double mean, double* sum_of_squares) {
    double sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum = (numbers[i] - mean) * (numbers[i] - mean) + sum;
    }
    *sum_of_squares = sum;
}

int main() {
    struct {
        double numbers[3];
        double mean;
        double sum_of_squares;
        double variance;
    } input;
    DWORD dwRead, dwWritten;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Чтение данных из канала
    if (!ReadFile(hStdIn, &input, sizeof(input), &dwRead, NULL) || dwRead != sizeof(input)) {
        fprintf(stderr, "SumOfSquaresProcess: ReadFile failed\n");
        return 1;
    }

    // Вычисление суммы квадратов отклонений
    ComputeSumOfSquares(input.numbers, input.mean, &input.sum_of_squares);

    // Запись результата в канал
    if (!WriteFile(hStdOut, &input.sum_of_squares, sizeof(input.sum_of_squares), &dwWritten, NULL) || dwWritten != sizeof(input.sum_of_squares)) {
        fprintf(stderr, "SumOfSquaresProcess: WriteFile failed\n");
        return 1;
    }

    printf("SumOfSquaresProcess: Sum of squares computed and sent to parent process\n");
    return 0;
}
