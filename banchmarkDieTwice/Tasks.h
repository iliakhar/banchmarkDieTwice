#pragma once
#include "TestMem.h"

void task1();
void task2();
void task3();

void ClearCsv(std::string filename);
void SaveBndVecsInCsv(int expCount, int step, int stepCount, char memType, bool wrWithX = false);