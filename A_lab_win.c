#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
    double numbers[3];
    double mean;
    double sum_of_squares;
    double variance;
} SharedData;

// Функция для запуска процессов
void CreateChildProcess(const char* processName, HANDLE hReadPipe, HANDLE hWritePipe, PROCESS_INFORMATION* pi) {
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = hReadPipe;
    si.hStdOutput = hWritePipe;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcess(NULL, processName, NULL, NULL, TRUE, 0, NULL, NULL, &si, pi)) {
        fprintf(stderr, "CreateProcess failed for %s\n", processName);
        exit(1);
    }
}

// Функция записи в канал
void WriteToPipe(HANDLE hPipe, void* data, DWORD size) {
    DWORD dwWritten;
    if (!WriteFile(hPipe, data, size, &dwWritten, NULL) || dwWritten != size) {
        fprintf(stderr, "WriteToPipe failed\n");
        exit(1);
    }
}

// Функция чтения из канала
void ReadFromPipe(HANDLE hPipe, void* data, DWORD size) {
    DWORD dwRead;
    if (!ReadFile(hPipe, data, size, &dwRead, NULL) || dwRead != size) {
        fprintf(stderr, "ReadFromPipe failed\n");
        exit(1);
    }
}

int main() {
    SharedData data;
    DWORD dwWritten, dwRead;
    HANDLE hMeanPipeRead, hMeanPipeWrite;
    HANDLE hSumOfSquaresPipeRead, hSumOfSquaresPipeWrite;
    HANDLE hVariancePipeRead, hVariancePipeWrite;
    PROCESS_INFORMATION piMean, piSumOfSquares, piVariance;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // Ввод данных
    printf("Enter three nums: ");
    scanf("%lf %lf %lf", &data.numbers[0], &data.numbers[1], &data.numbers[2]);

    // Создание каналов
    if (!CreatePipe(&hMeanPipeRead, &hMeanPipeWrite, &sa, 0) ||
        !CreatePipe(&hSumOfSquaresPipeRead, &hSumOfSquaresPipeWrite, &sa, 0) ||
        !CreatePipe(&hVariancePipeRead, &hVariancePipeWrite, &sa, 0)) {
        fprintf(stderr, "CreatePipe failed\n");
        return 1;
    }

    // Запуск процессов
    CreateChildProcess("MeanProcess.exe", hMeanPipeRead, hMeanPipeWrite, &piMean);
    CreateChildProcess("SumOfSquaresProcess.exe", hSumOfSquaresPipeRead, hSumOfSquaresPipeWrite, &piSumOfSquares);
    CreateChildProcess("VarianceProcess.exe", hVariancePipeRead, hVariancePipeWrite, &piVariance);

    // Передача данных в процесс для вычисления среднего
    WriteToPipe(hMeanPipeWrite, &data.numbers, sizeof(data.numbers));

    // Ожидание завершения процесса вычисления среднего
    WaitForSingleObject(piMean.hProcess, INFINITE);

    // Чтение среднего значения
    ReadFromPipe(hMeanPipeRead, &data.mean, sizeof(data.mean));
    printf("Mean computed: %lf\n", data.mean);

    // Передача данных в процесс для вычисления суммы квадратов отклонений
    WriteToPipe(hSumOfSquaresPipeWrite, &data, sizeof(data));

    // Ожидание завершения процесса вычисления суммы квадратов отклонений
    WaitForSingleObject(piSumOfSquares.hProcess, INFINITE);

    // Чтение суммы квадратов отклонений
    ReadFromPipe(hSumOfSquaresPipeRead, &data.sum_of_squares, sizeof(data.sum_of_squares));
    printf("Sum of squares computed: %lf\n", data.sum_of_squares);

    // Передача данных в процесс для вычисления дисперсии
    WriteToPipe(hVariancePipeWrite, &data.sum_of_squares, sizeof(data.sum_of_squares));

    // Ожидание завершения процесса вычисления дисперсии
    WaitForSingleObject(piVariance.hProcess, INFINITE);

    // Чтение дисперсии
    ReadFromPipe(hVariancePipeRead, &data.variance, sizeof(data.variance));
    printf("Variance computed: %lf\n", data.variance);

    // Закрытие дескрипторов
    CloseHandle(piMean.hProcess);
    CloseHandle(piMean.hThread);
    CloseHandle(piSumOfSquares.hProcess);
    CloseHandle(piSumOfSquares.hThread);
    CloseHandle(piVariance.hProcess);
    CloseHandle(piVariance.hThread);
    CloseHandle(hMeanPipeRead);
    CloseHandle(hMeanPipeWrite);
    CloseHandle(hSumOfSquaresPipeRead);
    CloseHandle(hSumOfSquaresPipeWrite);
    CloseHandle(hVariancePipeRead);
    CloseHandle(hVariancePipeWrite);

    return 0;
}
