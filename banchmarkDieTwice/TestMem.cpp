#include "TestMem.h";

void TestMem(int expCount, Banchmark& bnch, std::string filename) {
	bnch.timeR.resize(expCount);
	bnch.timeWr.resize(expCount);
	bnch.launchNum.resize(expCount);
	bnch.rAbsErr.resize(expCount);
	bnch.wrAbsErr.resize(expCount);
	bnch.rRelErr.resize(expCount);
	bnch.wrRelErr.resize(expCount);
	bnch.avTimeR = 0;
	bnch.avTimeWr = 0;

	for (int i(0); i < expCount; i++) {
		bnch.launchNum[i] = i;
		char* arr = new char[bnch.blockSize];
		if (filename == "") {
			bnch.timeR[i] = ReadFromRam(bnch.blockSize, arr);
			bnch.timeWr[i] = WriteToRam(bnch.blockSize, arr);
		}
		else {
			bnch.timeWr[i] = WriteToFile(bnch.blockSize, filename);
			bnch.timeR[i] = ReadFromFile(filename);
		}
		bnch.avTimeWr += bnch.timeWr[i];
		bnch.avTimeR += bnch.timeR[i];
	}
	bnch.avTimeR /= expCount;
	bnch.avTimeWr /= expCount;
	bnch.wrBandwidth = bnch.blockSize / bnch.avTimeWr * 1000000;
	bnch.rBandwidth = bnch.blockSize / bnch.avTimeR * 1000000;

	for (int i(0); i < expCount; i++) {
		bnch.rAbsErr[i] = abs(bnch.timeR[i] - bnch.avTimeR);
		bnch.rRelErr[i] = (bnch.rAbsErr[i] / bnch.avTimeR) * 100;

		bnch.wrAbsErr[i] = abs(bnch.timeWr[i] - bnch.avTimeWr);
		bnch.wrRelErr[i] = (bnch.wrAbsErr[i] / bnch.avTimeWr) * 100;
	}
	SaveToCSV(bnch);
}

// Чтение из RAM
double ReadFromRam(int size, char* arr) {
	char symb;
	double time = 0;
	std::chrono::steady_clock::time_point startTimer, stopTimer;
	for (int i(0); i < size; i++) {
		startTimer = std::chrono::high_resolution_clock::now();
		symb = arr[i];
		stopTimer = std::chrono::high_resolution_clock::now();
		time += std::chrono::duration_cast<std::chrono::nanoseconds>(stopTimer - startTimer).count();
	}
	return time;
}


// Запись в RAM
double WriteToRam(int size, char* arr) {
	char symb;
	double time = 0;
	std::chrono::steady_clock::time_point startTimer, stopTimer;
	for (int i(0); i < size; i++) {
		symb = rand() % 255 - 128;
		char symb = '0';
		startTimer = std::chrono::high_resolution_clock::now();
		arr[i] = symb;
		stopTimer = std::chrono::high_resolution_clock::now();
		time += std::chrono::duration_cast<std::chrono::nanoseconds>(stopTimer - startTimer).count();
	}
	return time;
}

// Чтение из файла (hdd/flash mem)
double ReadFromFile(std::string path) {
	FILE* fp;
	fopen_s(&fp, path.c_str(), "rb");
	char symb = '0';
	double time;
	std::chrono::steady_clock::time_point startTimer, stopTimer;
	startTimer = std::chrono::high_resolution_clock::now();
	while ((symb = fgetc(fp)) != EOF) {
		//fscanf_s(fp, "%c", &symb);
		//std::cout << symb;
	};
	stopTimer = std::chrono::high_resolution_clock::now();
	time = std::chrono::duration_cast<std::chrono::nanoseconds>(stopTimer - startTimer).count();
	fclose(fp);
	return time;
}

 // Запись в файл (hdd/flash mem)
double WriteToFile(int size, std::string path) {
	FILE* fp;
	fopen_s(&fp, path.c_str(), "wb");
	char symb;
	double time = 0;
	std::chrono::steady_clock::time_point startTimer, stopTimer;
	for (int i(0); i < size; i++) {
		symb = rand() % 255 - 128;
		if (symb == -1) symb = 'o';
		startTimer = std::chrono::high_resolution_clock::now();
		fprintf(fp, "%c", symb);
		stopTimer = std::chrono::high_resolution_clock::now();
		time += std::chrono::duration_cast<std::chrono::nanoseconds>(stopTimer - startTimer).count();
	}
	fclose(fp);
	return time;
}

template<typename T>
void WriteVecToCSV(std::string vecName, std::vector<T>& vec, std::ofstream& myfile) {
	myfile << vecName << ";";
	for (int i(0); i < vec.size(); i++)
		myfile << vec[i] << ";";
	myfile << "\n";
}

void SaveToCSV(Banchmark& bnch) {
	std::ofstream myfile;
	myfile.open("Statistics.csv", std::ios_base::out);
	//myfile.open("Statistics.csv", std::ios_base::app);
	myfile << "Memory type;" << bnch.memType << "\n";
	myfile << "Block Size;" << bnch.blockSizeWithOrigUnit << " " << bnch.blockUnit << "\n";
	myfile << "Element type;" << bnch.elemType<< "\n";
	myfile << "BufferSize;" << bnch.bufferSize<< "\n";
	WriteVecToCSV("LaunchNum", bnch.launchNum, myfile);
	myfile << "Timer;" << bnch.timer<< "\n";

	myfile << "AverageWriteTime;" << bnch.avTimeWr << "\n";
	WriteVecToCSV("WriteTime", bnch.timeWr, myfile);
	myfile << "WriteBandWidth;" << bnch.wrBandwidth<< "\n";
	WriteVecToCSV("AbsError(write)", bnch.wrAbsErr, myfile);
	WriteVecToCSV("RelError(write)", bnch.wrRelErr, myfile);

	myfile << "AverageReadTime;" << bnch.avTimeR << "\n";
	WriteVecToCSV("ReadTime", bnch.timeR, myfile);
	myfile << "ReadBandWidth;" << bnch.rBandwidth << "\n";
	WriteVecToCSV("AbsError(read)", bnch.rAbsErr, myfile);
	WriteVecToCSV("RelError(read)", bnch.rRelErr, myfile);


	myfile.close();
}