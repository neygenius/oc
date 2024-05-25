#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct {
    double numbers[3];
    double sum;
    double div;
    double diff[3];
    double sq_diff[3];
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
    HANDLE hSumPipeRead, hSumPipeWrite;
    //HANDLE hSum2PipeRead, hSum2PipeWrite;
    HANDLE hDivPipeRead, hDivPipeWrite;
    //HANDLE hDiv2PipeRead, hDiv2PipeWrite;
    HANDLE hDiffPipeRead, hDiffPipeWrite;
    HANDLE hSqDiffPipeRead, hSqDiffPipeWrite;
    PROCESS_INFORMATION piSum, piDiv, piDiff, piSqDiff;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // Ввод данных
    printf("Enter three nums: ");
    scanf("%lf %lf %lf", &data.numbers[0], &data.numbers[1], &data.numbers[2]);

    // Создание каналов
    if (!CreatePipe(&hSumPipeRead, &hSumPipeWrite, &sa, 0) ||
        !CreatePipe(&hDivPipeRead, &hDivPipeWrite, &sa, 0) ||
        //!CreatePipe(&hSum2PipeRead, &hSum2PipeWrite, &sa, 0) ||
        //!CreatePipe(&hDiv2PipeRead, &hDiv2PipeWrite, &sa, 0) ||
        !CreatePipe(&hDiffPipeRead, &hDiffPipeWrite, &sa, 0) ||
        !CreatePipe(&hSqDiffPipeRead, &hSqDiffPipeWrite, &sa, 0)) {
        fprintf(stderr, "CreatePipe failed\n");
        return 1;
    }

    // Запуск процессов
    CreateChildProcess("SumProcess.exe", hSumPipeRead, hSumPipeWrite, &piSum);
    CreateChildProcess("DivProcess.exe", hDivPipeRead, hDivPipeWrite, &piDiv);

    //CreateChildProcess("SumProcess.exe", hSum2PipeRead, hSum2PipeWrite, &piSum2);
    //CreateChildProcess("DivProcess.exe", hDiv2PipeRead, hDiv2PipeWrite, &piDiv2);

    CreateChildProcess("DiffProcess.exe", hDiffPipeRead, hDiffPipeWrite, &piDiff);
    CreateChildProcess("SqDiffProcess.exe", hSqDiffPipeRead, hSqDiffPipeWrite, &piSqDiff);

    // Передача данных в процесс для вычисления cуммы
    WriteToPipe(hSumPipeWrite, &data.numbers, sizeof(data.numbers));

    // Ожидание завершения процесса вычисления суммы
    WaitForSingleObject(piSum.hProcess, INFINITE);

    // Чтение суммы
    ReadFromPipe(hSumPipeRead, &data.sum, sizeof(data.sum));
    printf("Sum computed: %lf\n", data.sum);

    // Передача данных в процесс для вычисления среднего
    WriteToPipe(hDivPipeWrite, &data.sum, sizeof(data.sum));

    // Ожидание завершения процесса вычисления среднего
    WaitForSingleObject(piDiv.hProcess, INFINITE);

    // Чтение среднего значения
    ReadFromPipe(hDivPipeRead, &data.div, sizeof(data.div));
    printf("Mean computed: %lf\n", data.div);

    // Передача данных в процесс для вычисления отклонений
    WriteToPipe(hDiffPipeWrite, &data, sizeof(data));

    // Ожидание завершения процесса вычисления отклонений
    WaitForSingleObject(piDiff.hProcess, INFINITE);

    // Чтение отклонений
    ReadFromPipe(hDiffPipeRead, &data.diff, sizeof(data.diff));
    printf("Diff1 computed: %lf\n", data.diff[0]);
    printf("Diff2 computed: %lf\n", data.diff[1]);
    printf("Diff3 computed: %lf\n", data.diff[3]);

    // Передача данных в процесс для вычисления квадратов отклонений
    WriteToPipe(hSqDiffPipeWrite, &data.diff, sizeof(data.diff));

    // Ожидание завершения процесса вычисления квадратов отклонений
    WaitForSingleObject(piSqDiff.hProcess, INFINITE);

    // Чтение квадратов отклонений
    ReadFromPipe(hSqDiffPipeRead, &data.sq_diff, sizeof(data.sq_diff));
    printf("SqDiff1 computed: %lf\n", data.sq_diff[0]);
    printf("SqDiff2 computed: %lf\n", data.sq_diff[1]);
    printf("SqDiff3 computed: %lf\n", data.sq_diff[2]);

    CloseHandle(piSum.hProcess);
    CloseHandle(piSum.hThread);
    CloseHandle(piDiv.hProcess);
    CloseHandle(piDiv.hThread);
    CloseHandle(hSumPipeRead);
    CloseHandle(hSumPipeWrite);
    CloseHandle(hDivPipeRead);
    CloseHandle(hDivPipeWrite);

    if (!CreatePipe(&hSumPipeRead, &hSumPipeWrite, &sa, 0) ||
        !CreatePipe(&hDivPipeRead, &hDivPipeWrite, &sa, 0)) {
        fprintf(stderr, "CreatePipe failed\n");
        return 1;
    }

    CreateChildProcess("SumProcess.exe", hSumPipeRead, hSumPipeWrite, &piSum);
    CreateChildProcess("DivProcess.exe", hDivPipeRead, hDivPipeWrite, &piDiv);


    // Передача данных в процесс для вычисления суммы квадратов отклонений
    WriteToPipe(hSumPipeWrite, &data.diff, sizeof(data.diff));

    // Ожидание завершения процесса вычисления суммы квадратов отклонений
    WaitForSingleObject(piSum.hProcess, INFINITE);

    // Чтение суммы квадратов отклонений
    ReadFromPipe(hSumPipeRead, &data.sum, sizeof(data.sum));
    printf("Sum sqdiff computed: %lf\n", data.sum);

    // Передача данных в процесс для вычисления дисперсии
    WriteToPipe(hDivPipeWrite, &data.sum, sizeof(data.sum));

    // Ожидание завершения процесса вычисления дисперсии
    WaitForSingleObject(piDiv.hProcess, INFINITE);

    // Чтение дисперсии
    ReadFromPipe(hDivPipeRead, &data.div, sizeof(data.div));
    printf("Variance computed: %lf\n", data.div);

    // Закрытие дескрипторов
    CloseHandle(piSum.hProcess);
    CloseHandle(piSum.hThread);
    CloseHandle(piDiv.hProcess);
    CloseHandle(piDiv.hThread);
    //CloseHandle(piSum2.hProcess);
    //CloseHandle(piSum2.hThread);
    //CloseHandle(piDiv2.hProcess);
    //CloseHandle(piDiv2.hThread);
    CloseHandle(piDiff.hProcess);
    CloseHandle(piDiff.hThread);
    CloseHandle(piSqDiff.hProcess);
    CloseHandle(piSqDiff.hThread);
    CloseHandle(hSumPipeRead);
    CloseHandle(hSumPipeWrite);
    CloseHandle(hDivPipeRead);
    CloseHandle(hDivPipeWrite);
    //CloseHandle(hSum2PipeRead);
    //CloseHandle(hSum2PipeWrite);
    //CloseHandle(hDiv2PipeRead);
    //CloseHandle(hDiv2PipeWrite);
    CloseHandle(hDiffPipeRead);
    CloseHandle(hDiffPipeWrite);
    CloseHandle(hSqDiffPipeRead);
    CloseHandle(hSqDiffPipeWrite);
    return 0;
}
