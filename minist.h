#pragma once
#include <cstdio>   // fopen
#include <cstdlib>  // abort
#include <cstring>  // string
#include <cassert>  // assert
#include <iostream> // cout
#include <cmath>    // sqrt 
#include <ctime>    // time
#include<fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class MNIST
{
private:

public:
	MNIST() {}
	~MNIST() {}
	int magic;
	int numImages;
	int rows;
	int cols;
	void loadDigits(const string &fileName, Mat &digits)
	{
		assert(fileName.empty() == false);

		FILE *file;
		fopen_s(&file, fileName.c_str(), "rb");
		if (file == NULL) {
			printf("Could not open %s\n", fileName.c_str());
			abort();
		}

		// load digits data

		// load magic number
		fread(&magic, sizeof(magic), 1, file);
		magic = reverseInt4MNIST(magic);
		if (magic != 2051) {
			fclose(file);
			printf("Bad magic number in %s\n", fileName.c_str());
			abort();
		}

		// load number, rows and cols of images
		fread(&numImages, sizeof(int), 1, file);
		fread(&rows, sizeof(int), 1, file);
		fread(&cols, sizeof(int), 1, file);

		//numImages = reverseInt4MNIST(numImages);              //10000
		numImages = 600;
		rows = reverseInt4MNIST(rows);                       //28
		cols = reverseInt4MNIST(cols);                       //28

		// convert to OpenCV structure
		digits.create(numImages, rows*cols, CV_8UC1);
		if (digits.isContinuous())
			fread(digits.data, sizeof(uchar), rows * cols*numImages, file);
		fclose(file);
	}

	void loadLabels(const string &fileName, Mat &labels)
	{
		assert(fileName.empty() == false);
		FILE *file;
		fopen_s(&file, fileName.c_str(), "rb");
		if (file == NULL) {
			printf("Could not open %s in %s line", fileName, __LINE__);
			abort();
		}

		// load digits data
		int magic, numImages;

		fread(&magic, sizeof(int), 1, file);
		magic = reverseInt4MNIST(magic);                           //2049

		if (magic != 2049) {
			fclose(file);
			printf("Bad magic number in %s in %s line", fileName, __LINE__);
			abort();
		}

		fread(&numImages, sizeof(int), 1, file);
		//numImages = reverseInt4MNIST(numImages);
		numImages = 600;
		labels = Mat::zeros(1, numImages, CV_8UC1);
		fread(labels.data, sizeof(uchar), numImages, file);


		labels.convertTo(labels, CV_32S);
		fclose(file);
	}

	void showDigits(const vector<Mat> &digits, int showNumber = 100)
	{
		vector<int> index(digits.size(), CV_8UC1);
		//uchar *ptr = (uchar *)index.data;
		for (int i = 0; i < digits.size(); ++i)
		{
			index[i] = i;
		}
		srand((uchar)time(NULL));
		std::random_shuffle(index.begin(), index.end());

		int sr = 0;
		int sc = -1;
		int numPerRow = ceil(sqrt(showNumber));

		Mat showed(numPerRow * 28, numPerRow * 28, digits[0].type());
		for (int i = 0; i < showNumber; i++) {
			sr = i % numPerRow;
			if (sr == 0) sc += 1;
			digits[index[i]].copyTo(showed(Range(sr * 28, (sr + 1) * 28), Range(sc * 28, (sc + 1) * 28)));
		}
		imshow("random digits", showed);
		waitKey(0);

		index.clear();
		showed.release();
	}

	bool writeDate(const string &fileName, vector<Mat>&digits)
	{
		ofstream outfile;
		outfile.open(fileName.c_str(), ios::out);
		for (int i = 0; i < numImages; i++)
		{
			for (int j = 0; j < 28; j++)
			{
				for (int k = 0; k < 28; k++)
				{
					int data = (int)digits[i].at<uchar>(j, k);
					outfile << data << "\t";
				}
			}
			outfile << endl;
		}
		outfile.close();
		return true;
	}


protected:
	int reverseInt4MNIST(const int i)
	{
		uchar ch1, ch2, ch3, ch4;
		ch1 = i & 255;
		ch2 = (i >> 8) & 255;
		ch3 = (i >> 16) & 255;
		ch4 = (i >> 24) & 255;
		return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
	}

private:
	MNIST(const MNIST &rhs); // do not allow copy constructor
	const MNIST &operator = (const MNIST &); // nor assignement operator
};