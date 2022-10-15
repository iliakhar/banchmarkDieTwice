#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>

struct Banchmark {
	std::string memType;
	int blockSize;
	int blockSizeWithOrigUnit;
	std::string blockUnit;
	std::string elemType = "char";
	int bufferSize = 1;
	std::vector<int> launchNum;
	std::string timer = "std::chrono::high_resolution_clock::now";
	double avTimeWr;
	double avTimeR;
	double wrBandwidth;
	double rBandwidth;

	std::vector<double> rAbsErr;
	std::vector<double> rRelErr;
	std::vector<double> wrAbsErr;
	std::vector<double> wrRelErr;

	std::vector<double> timeWr;
	std::vector<double> timeR;
};

void TestMem(int expCount, Banchmark& bnch, std::string filename = "");
double WriteToFile(int size, std::string path);
double ReadFromFile(std::string path);

double ReadFromRam(int size, char* arr);
double WriteToRam(int size, char* arr);
void SaveToCSV(Banchmark& bnch, std::string csvname);

template<typename T>
void WriteVecToCSV(std::vector<T>& vec, std::ofstream& myfile, std::string vecName = "") {
	if(vecName!="")
		myfile << vecName << ";";
	for (int i(0); i < vec.size(); i++)
		myfile << vec[i] << ";";
	myfile << "\n";
}


