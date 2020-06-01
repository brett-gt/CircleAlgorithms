// CircleTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Circle.h"
#include "Draw.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std;

/*-------------------------------------------------------------------------------*/
void printResult(baseCircle *result)
{
	for (int i = 0; i < result->getLength(); i++)
	{
		Point point = result->getResult()[i];
		std::cout << "( " << point.X << "," << point.Y << ")\n";
	}
}

/*-------------------------------------------------------------------------------*/
void writeResults(baseCircle *result, string fileName)
{
	fstream fout;
	fout.open(fileName, ios::out);

	fout << "X,Y" << endl;
	for (int i = 0; i < result->getLength(); i++)
	{
		Point point = result->getResult()[i];
		fout << point.X << "," << point.Y << "\n";
	}
	fout.close();
}

/*-------------------------------------------------------------------------------*/
float timeIt(baseCircle *circle, int interations)
{
	int radius = 320;

	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < interations; i++)
	{
		circle->calc(radius);
	}
	auto stop = chrono::high_resolution_clock::now();

	std::chrono::duration<float, std::milli> fp_ms = stop - start;

	return fp_ms.count()/(float)interations;
}

/*-------------------------------------------------------------------------------*/
int main(int argc, char** argv)
{

	vector<baseCircle*> pointers;
	pointers.push_back(new naiveCircle());
	pointers.push_back(new naiveEightCircle());
	pointers.push_back(new expansionCircle());
	pointers.push_back(new bruteForceCircle());
	pointers.push_back(new strictBruteCircle());

	//https://en.wikibooks.org/wiki/C%2B%2B_Programming/Classes/Abstract_Classes
	for (auto func : pointers)
	{
		float duration = timeIt(func, 1000);
		cout << func->name << " : Duration (ms) = " << duration << endl;
		string filename = func->name + ".csv";
		writeResults(func, filename);
	}

	
	
	/*
	expansionCircle test5;
	//test.calc(300, 16);
	//printResult(&test);

	test5.calc(300, 1200);
	writeResults(&test5, "test.csv");
	*/

	/*
	cout << "\n\n";
	bruteForceCircle test1;
	test1.calc(120);
	printResult(&test1);
	writeResults(&test1, "test.csv");
	*/

	

	std::cout << "Press any key to continue\n";
	std::cin.get();

	
}



int runAndTime()
{
	return  0;
}

