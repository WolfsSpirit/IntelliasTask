#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

void readFile(const string fileName, vector<vector<string>>& rInputFile);
void processData(vector<vector<string>>& rInputData, vector<vector<string>>& rOutputData);
void writeInFile(const string fileName, const vector<vector<string>>& rOutputData);
int findIndex(vector<vector<string>>& data, string name, string fdate);

int main()
{
	vector<vector<string>> input_data, output_data;
	string input_fname, output_fname;
	
	cout << "Enter the input file name: ";
	cin >> input_fname;
	readFile(input_fname, input_data);

	processData(input_data, output_data);

	cout << "Enter the output file name: ";
	cin >> output_fname;
	writeInFile(output_fname, output_data);
	
	return 0;
}

void readFile(const string fileName, vector<vector<string>> &rInputFile) {
	vector<string> row;
	string line, word;

	fstream file(fileName, ios::in);
	if (file.is_open())
	{
		std::cout << "Reading..." << '\n';

		while (getline(file, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ';'))
				row.push_back(word);
			rInputFile.push_back(row);
		}
		file.close();
	}
	else
		std::cerr << "Error: Could not open the file\n";

	std::cout << "Done." << '\n';
};

void processData(vector<vector<string>> &rInputData, vector<vector<string>> &rOutputData) {
	std::cout << "Processing..." << '\n';
	
	vector<string> row;
	string name, date, formatted_date, month, year, hours;
	ptime pt;
	int index;

	while (rInputData.size() != 1) {
		name = rInputData[1][0];
		date = rInputData[1][6];
		hours = rInputData[1][7];

		pt = time_from_string(date + " 23:59:59.000");
		month = string(pt.date().month().as_long_string());
		year = to_string(pt.date().year());
		formatted_date = month + " " + year;

		row.clear();
		index = findIndex(rOutputData, name, formatted_date);
		if (index >= 0) {
			rOutputData[index][2] = to_string(stoi(rOutputData[index][2]) + stoi(hours));
			rInputData.erase(rInputData.begin() + 1);
		}
		else {
			row.push_back(name);
			row.push_back(formatted_date);
			row.push_back(hours);
			rOutputData.push_back(row);
			rInputData.erase(rInputData.begin() + 1);
		}
	}

	std::cout << "Done." << '\n';
};

void writeInFile(const string fileName, const vector<vector<string>>& rOutputData) {
	std::cout << "Writing..." << '\n';
	
	fstream file(fileName, ios::out);
	if (file.is_open()) {
		file << "Name;Month;Total hours" << '\n';
		std::cout << '\n';
		for (auto row : rOutputData) {
			file << row[0] << ';' << row[1] << ';' << row[2] << '\n';
			std::cout << row[0] << ';' << row[1] << ';' << row[2] << '\n';
		}
		std::cout << '\n';
		file.close();
	}
	else
		std::cerr << "Error: Could not open the file\n";
	
	std::cout << "Done." << '\n';
}

int findIndex(vector<vector<string>> &data, string name, string fdate) {
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i][0].compare(name) == 0 && data[i][1].compare(fdate) == 0) {
			return i;
		}
	}
	return -1;
}