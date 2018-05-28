#include "opencv2/opencv.hpp"
#include "useful_dip_lib.h"
#include "training.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <stack>
using namespace cv;
using namespace std;

std::stack<clock_t> tictoc_stack;

void tic() {
	tictoc_stack.push(clock());
}

void toc() {
	std::cout << "Time elapsed: "
			<< ((double) (clock() - tictoc_stack.top())) / CLOCKS_PER_SEC
			<< " sec" << std::endl;
	tictoc_stack.pop();
}
int main() {
	/*string videoFolder = "videos_for_training/";
	String trainDataFile = "trainData.csv";
	String responsesFile = "responses.csv";
	generate_database(videoFolder,trainDataFile,responsesFile);
	*/
	string videoFolder = "videos_for_testing/";
	String testDataFile = "testData.csv";
	String realValuesTestData = "realData.csv";
	generate_database(videoFolder,testDataFile,realValuesTestData);
	return 0;

}
