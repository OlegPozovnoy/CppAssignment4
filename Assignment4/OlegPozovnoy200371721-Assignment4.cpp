
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<map>
#include <numeric>

using namespace std;

struct outputStatistics {
	int min;
	int max;
	int range;
	
	double mean;
	double median;

	vector<int> mode;
};

void readVector(vector<int>& val); // geting user input and storing it in a vector
int* readPointer(int& size);// geting user input and storing it with a pointer
outputStatistics calcStat(vector<int> vals);//calculate statistics for vector
outputStatistics calcStat(int* vals, const int& size);//calculate statistics for pointer
void writeVectorSorted(ostream& out, vector<int> stat);//write to out info on vector values and sorted values for vector
void writeVectorSorted(ostream& out, int* pointer, const int size);//write to out info on vector values and sorted values for pointer
void writeStatistics(ostream& out, outputStatistics stat);////write to out statistics
void resultsSaveInfo(const string& filePath);//write to cout "results are stored to *.txt" // "no filename has been provided"
int centralTendency();// entry point for the Central Tendency task
int importer();//entry point for the Importer task

int main() {

	int pick;

	cout << "\n\nAssignment4, please select the task: ";

	do {
		cout << "\n\n(1). Importer";
		cout << "\n(2). Central Tendency";
		cout << "\n(3). Exit";
		cout << "\n\nPlease make your selection:";

		cin >> pick;

		switch (pick) {
		case 1:
			importer();
			break;
		case 2:
			centralTendency();
			break;
		case 3:
			return 0;
		default:
			cout << "\nWrong input, please try again";
		}
	} while (true);


}

void readVector(vector<int>& val) {
	int N;
	
	cout << "\n\nHow many terms would you like to enter? ";
	cin >> N;

	cout << "\n";
	for (auto i = 0; i < N; i++) {
		int next;
		cout << "Term " << i+1<<": ";
		cin >> next;
		val.push_back(next);
	}
}

int* readPointer(int& size) {

	cout << "\n\nHow many terms would you like to enter? ";
	cin >> size;
	int* pointer = new int[size];

	cout << "\n";
	for (auto i = 0; i < size; i++) {
		cout << "Term " << i+1 << ": ";
		cin >> pointer[i];
	}

	return pointer;
}

outputStatistics calcStat(vector<int> vals) {
	
	outputStatistics result;

	map<int, int> frequency;//<value, frequency>

	if (vals.size() == 0)
		return result;

	sort(vals.begin(), vals.end());

	result.min = vals[0];
	result.max = vals[vals.size() - 1];
	result.range = result.max - result.min;
	
	if (vals.size() % 2 == 0)
		result.median = (vals[vals.size() / 2 - 1] + vals[vals.size() / 2])/2.0;
	else
		result.median = vals[(vals.size() - 1) / 2];

	result.mean = static_cast<double>(std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size());

	for (auto s : vals) 
		frequency[s]++;

	//getting the max frequency
	int max_frequency = 0;
	for (auto s : frequency)
		max_frequency = max(max_frequency, s.second);

	if (max_frequency <= 1)// if it's 1 there's no mode by definition
		return result;
	//filling modes vector
	for (auto s : frequency)
		if (s.second == max_frequency)
			result.mode.push_back(s.first);

	return result;
}


outputStatistics calcStat(int* vals, const int& size) {

	outputStatistics result;

	if (size == 0)
		return result;

	sort(vals, vals +size);

	result.min = vals[0];
	result.max = vals[size - 1];
	result.range = result.max - result.min;

	if (size % 2 == 0)
		result.median = (vals[size / 2 - 1] + vals[size / 2]) / 2.0;
	else
		result.median = vals[(size - 1) / 2];

	result.mean = static_cast<double>(std::accumulate(vals, vals+size, 0.0) / size);
	
	//a nice algorythm of modes computation came to mind, faster then creating a map of frequencies 
	int counter=1; //number of consequent equal vals[i] - we use that the array is already sorted
	int maxfrequency = 2;//frequency of current mode, =2 because I read that if max freq = 1 - there's no mode by definition.
	
	for (auto i = 0; i  < size; i++) {
		if (i==size-1 || vals[i] != vals[i + 1]) {
			if (counter == maxfrequency) {// a vals[i] is as frequent as old modes 
				result.mode.push_back(vals[i]);
			}
			else if (counter > maxfrequency) {//a vals[i] is more frequent as old modes
				result.mode.clear();
				result.mode.push_back(vals[i]);
			}
			counter = 1;//reset counter 
		}
		else {
			counter++;
		}
	}

	return result;
}


void writeVectorSorted(ostream& out, vector<int> stat) {

	out << "\n\nProvided data: ";
	for (auto i = 0; i < stat.size(); i++) {
		if (i == 0)
			out << stat[0];
		else
			out << ", " << stat[i];
	}

	sort(stat.begin(), stat.end());

	out << "\nSorted data: ";
	for (auto i = 0; i < stat.size(); i++) {
		if (i == 0)
			out << stat[0];
		else
			out << ", " << stat[i];
	}
	out << "\n";

}

void writeVectorSorted(ostream& out, int* pointer, const int size ) {

	if (size == 0)
		cout << "\nNo data to write";

	//could avoid additional memory allocation, but we call this functiontwice, for cout and for fout - we can't afford to sort initial values
	int* array_copy = new int[size];
	copy(pointer, pointer + size, array_copy);

	out << "\nProvided data: ";
	for (auto i = 0; i < size; i++) {
		if (i == 0)
			out << array_copy[0];
		else
			out << ", " << array_copy[i];
	}

	sort(array_copy, array_copy + size);

	out << "\nSorted data: ";
	for (auto i = 0; i < size; i++) {
		if (i == 0)
			out << array_copy[0];
		else
			out << ", " << array_copy[i];
	}

	delete[] array_copy;
}


void writeStatistics(ostream& out, outputStatistics stat) {
	out << "\n\nStatistical Results :"; //computed based on provided user data (real time)
	out <<"\nMin:\t" << setw(10) << std::right << stat.min;
	out <<"\nMax:\t" << setw(10) << std::right <<stat.max; //allow for floating point data entry
	out <<"\nRange:\t" << setw(10) << std::right << stat.range;
	out <<"\nMean:\t" << setw(10) << std::fixed << std::setprecision(1) << std::right << stat.mean; //limit all to 1 decimal point for printed solution
	out <<"\nMedian:\t" << setw(10) << std::right <<  stat.median;
	out <<"\nMode(s):\t";
	if (stat.mode.size() == 0)
		out << "doesn't exist";
	else
		for (auto t : stat.mode)
			out<<t<<" ";

}

void resultsSaveInfo(const string& filePath) {
	if (filePath.size() == 0) {
		cout << "\n\nThis could not be saved as no file path has been provided by the user.";
	}
	else {
		cout << "\n\nThe results have been saved to: " << filePath;
	}

}

int centralTendency() {

	int pick;
	string filePath = "";


	cout << "\n\nAvailable Operations: ";

	do {
		cout << "\n\n(1). Store and Compute with Vector";
		cout << "\n(2). Store and Compute with Pointer";
		cout << "\n(3). Set File Saving Path";
		cout << "\n(4). Exit";
		cout << "\n\nPlease make your selection:";

		cin >> pick;

		switch (pick) {
		case 1:
			{
				ofstream fout;
				vector<int> vals;
				readVector(vals);

				if (vals.size() > 0) {
					writeVectorSorted(cout, vals);
					outputStatistics stat = calcStat(vals);
					writeStatistics(cout, stat);

					if (filePath.size() > 0) {
						fout.open(filePath, ios_base::app);
						writeVectorSorted(fout, vals);
						writeStatistics(fout, stat);
						fout.close();
					}
				}
				resultsSaveInfo(filePath);
			}
			break;

		case 2:
			{
				int size = 0;
				int* pointer = readPointer( size);
				
				if (size > 0) {
					writeVectorSorted(cout, pointer, size);
					outputStatistics stat = calcStat(pointer, size);
					writeStatistics(cout, stat);

					ofstream fout;
					if (filePath.size() > 0) {
						fout.open(filePath, ios_base::app);
						writeVectorSorted(fout, pointer, size);
						writeStatistics(fout, stat);
						fout.close();
					}
				}
				delete[] pointer;
				resultsSaveInfo(filePath);
			}
			break;
		case 3:
			cout << "\nPlease enter the file path: ";
			cin >> filePath;
			break;
		case 4:
			return 0;
		default:
			cout << "\nWrong input, please try again";
		}
	} while (true);

}

int importer() {

	string file_path;

		cout << "\n\nPlease specify the file name or full path: ";
		cin >> file_path;

		ifstream in(file_path);

		if (in.fail())
		{
			cout << "File open error, please check path/file name!";
			return 1;
		}

		int64_t sum = 0;
		int counter = 0;
		vector<int> values;
		{
			while (!in.eof()) {
				int val;
				in >> val;
				//cout << val << endl;
				if (in.peek() != ',')
				{
					cout << "\nWrong file format, expected ',', found " << in.peek();
				}
				in.ignore(1);
				in>> val;
				//cout << val << endl;
				sum += val;
				++counter;
			}
		}

		in.close();

		cout << "\nThe average of the input data is " << static_cast<double>(sum) / counter
			<< "\nthe sum is " << sum << "\nthe number of addends is "<< counter;
		return 0;
}
