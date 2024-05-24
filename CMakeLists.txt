cmake_minimum_required(VERSION 3.23)
project(Untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(LabWin lab_win.c)

add_executable(MeanProcess mean.c)
add_executable(SumOfSquaresProcess sq_sum.c)
add_executable(VarianceProcess variance.c)
add_executable(ALab A_lab_win.c)