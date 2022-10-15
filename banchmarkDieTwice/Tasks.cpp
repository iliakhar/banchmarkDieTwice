#include "Tasks.h"

void task1() {
	char memType, unit;
	int expCount;
	Banchmark bnch;
	std::ofstream myfile;
	myfile.open("Statistic_t1.csv", std::ios_base::out);
	myfile.close();
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
	SaveToCSV(bnch, "Statistic_t1.csv");
}

void task2() {

	std::ofstream myfile;
	myfile.open("Statistic_t1.csv", std::ios_base::out);
	myfile.close();

	for (int i(64); i <= 256; i += 64) {
		Banchmark bnch;
		bnch.blockSizeWithOrigUnit = bnch.blockSize = i;
		bnch.blockUnit = "b";
		bnch.memType = "RAM";
		TestMem(10, bnch);
		SaveToCSV(bnch, "Statistic_t2.csv");
	}
	std::string filename("test.txt");
	std::string memType("HDD");
	for (int j(0); j < 2; j++) {
		for (int i(4); i <= 16; i += 4) {
			Banchmark bnch;
			bnch.blockSizeWithOrigUnit = i;
			bnch.blockSize = i * 1024 * 1024;
			bnch.blockUnit = "Mb";
			bnch.memType = memType;
			TestMem(20, bnch, filename);
			SaveToCSV(bnch, "Statistic_t2.csv");
		}
		filename = "test.txt";
		memType = "Flash";
	}

}

void ClearCsv(std::string filename) {
	std::ofstream file;
	file.open(filename, std::ios_base::out);
	file.close();
}

void SaveBndVecsInCsv(int expCount, int step, int stepCount, char memType, bool wrWithX) {
	Banchmark bnch;
	std::ofstream bandR, bandWr;
	std::vector<double> wrBandVec(stepCount);
	std::vector<double> rBandVec(stepCount);
	std::vector<int> num(stepCount);
	std::string writeTo("");
	std::string fullMemType("RAM");
	bandR.open("banchPy\\banchPy\\BandR.csv", std::ios_base::app);
	bandWr.open("banchPy\\banchPy\\BandWr.csv", std::ios_base::app);

	switch (memType) {
	case 'h':
		fullMemType = "HDD";
		writeTo = "test.txt";
		break;
	case 'f':
		fullMemType = "Flash";
		writeTo = "F:\\test.txt";
		break;
	}

	for (int i(1); i <= stepCount; i++) {
		bnch.blockSize = i * step;
		TestMem(expCount, bnch, writeTo);
		num[i - 1] = bnch.blockSize;
		wrBandVec[i - 1] = bnch.wrBandwidth;
		rBandVec[i - 1] = bnch.rBandwidth;
	}
	if (wrWithX) {	//blockSize
		WriteVecToCSV(num, bandR, "blockSize");
		WriteVecToCSV(num, bandWr, "blockSize");
	}
	WriteVecToCSV(wrBandVec, bandWr, fullMemType);
	WriteVecToCSV(rBandVec, bandR, fullMemType);

	bandR.close();
	bandWr.close();
}

void task3() {

	int stepCount(20), expCount(10), step(512);
	ClearCsv("banchPy\\banchPy\\BandR.csv");
	ClearCsv("banchPy\\banchPy\\BandWr.csv");
	SaveBndVecsInCsv(expCount, step, stepCount, 'r', true);
	SaveBndVecsInCsv(expCount, step, stepCount, 'h');
	SaveBndVecsInCsv(expCount, step, stepCount, 'f');

	std::ofstream errFile;
	errFile.open("banchPy\\banchPy\\Err.csv", std::ios_base::out);
	Banchmark bnch;
	bnch.blockSize = 1024;
	step = 5;
	stepCount = 10;
	std::vector<double> vecErrWr(stepCount);
	std::vector<double> vecErrR(stepCount);
	std::vector<double> num(stepCount);
	for (int i(0); i < stepCount; i++) {
		num[i] = (i + 1) * step;
		TestMem(num[i], bnch);
		for (int j(0); j < num[i]; j++) {
			vecErrWr[i] += bnch.wrRelErr[j];
			vecErrR[i] += bnch.rRelErr[j];
		}
		vecErrWr[i] /= num[i];
		vecErrR[i] /= num[i];
	}
	WriteVecToCSV(num, errFile, "ExpCount");		//expCount
	WriteVecToCSV(vecErrWr, errFile, "Write");	//wr
	WriteVecToCSV(vecErrR, errFile, "Read");	//r
	errFile.close();

}