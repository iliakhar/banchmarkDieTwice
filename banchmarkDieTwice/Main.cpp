#include <iostream>
#include "TestMem.h"

int main() {
	srand(time(NULL));
	char memType, unit;
	int expCount;
	Banchmark bnch;

	std::cout << "Memory type: ";
	std::cin >> memType;
	std::cout << "\nUnit: ";
	std::cin >> unit;
	std::cout << "\nSize: ";
	std::cin >> bnch.blockSizeWithOrigUnit;
	std::cout << "\nExp count: ";
	std::cin >> expCount;

	switch (unit) {
	case 'k':
		bnch.blockSize = bnch.blockSizeWithOrigUnit * 1024;
		bnch.blockUnit = "Kb";
		break;
	case 'm':
		bnch.blockSize = bnch.blockSizeWithOrigUnit * 1024 * 1024;
		bnch.blockUnit = "Mb";
		break;
	default:
		bnch.blockSize = bnch.blockSizeWithOrigUnit;
		bnch.blockUnit = "b";
	}

	switch (memType) {
	case 'r':
		bnch.memType = "RAM";
		TestMem(expCount, bnch);
		break;
	case 'h':
		bnch.memType = "HDD";
		TestMem(expCount, bnch, "test.txt");
		break;
	case 'f':
		bnch.memType = "Flash";
		TestMem(expCount, bnch, "F:\\test.txt");
		break;
	}
	
}