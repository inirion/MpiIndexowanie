#include "omp.h"
#include <iostream>
#include <time.h>
#include <conio.h>
#include <iomanip>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

struct timeCount {
	int start;
	int end;
	void startTime() { start = GetTickCount(); }
	void endTime() { end = GetTickCount(); }
	int measure() {
		return end - start;
	}
	void reset() { start = 0; end = 0; }
};
class StringArray {
private:
	int Row;
	int Column;
	string Text;
public:
	StringArray() : Row(0), Column(0), Text("") {}
	StringArray(int r, int c, string t) : Row(r), Column(c), Text(t) {}
	int getRow() { return Row; }
	int getColumn() { return Column; }
	string getText() { return Text; }
	void setText(string text,bool przed = false) {
		if(!przed)
			this->Text = text;
		else {
			this->Text = text + " " + this->Text;
		}
	}
};

class StringArrayContainer {
private:
	int RowCount;
	int ColCount;
	vector<StringArray> object;
public :
	StringArrayContainer(){}
	int getRowCount() { return RowCount+1; }
	int getColCount() { return ColCount+1; }
	void addElementAt(int row, int col, string text) {
		object.push_back(StringArray(row, col, text));
		if (row > RowCount) RowCount = row;
		if (col > ColCount) ColCount = col;
	}
	string getElementAt(int row,int col) {
		for (int i = 0; i < object.size(); i++) {
			if (object[i].getRow() == row && object[i].getColumn() == col)
				return object[i].getText();
		}
	}
	void AddDataFromSource(int *data,int col,int howMuch, int dataSourceSize) {
		for (int i = dataSourceSize - howMuch, count = 2; i < dataSourceSize; i++, count++) {
			for (int j = 1; j <= ColCount; j++) {
				addElementAt(count, j, "");
			}
			addElementAt(count, col, to_string(data[i]));
		}
	}
	void setElementAt(int row, int col, string text) {
		for (int i = 0; i < object.size(); i++) {
			if (object[i].getRow() == row && object[i].getColumn() == col)
				object[i].setText(text);
		}
	}
	void SetDataFromSource(int *data, int col, int howMuch, int dataSourceSize) {
		for (int i = dataSourceSize - howMuch, count = 2; i < dataSourceSize; i++, count++) {
			setElementAt(count, col, to_string(data[i]));
		}
	}
	void setTime(int row, int col, string text) {
		for (int i = 0; i < object.size(); i++) {
			if (object[i].getRow() == row && object[i].getColumn() == col)
				object[i].setText(text,true);
		}
	}
	void drawTable() {
		for (int i = 0; i <= RowCount; i++) {
			for (int j = 0; j <= ColCount; j++) {
				cout << setw(10) << left << getElementAt(i, j);
			}
			cout << endl;
		}
	}
	void InitializateArrayContainer() {
		addElementAt(0, 0, "");
		addElementAt(0, 1, "Threads \032");
		addElementAt(0, 2, "1");
		addElementAt(0, 3, "2");
		addElementAt(0, 4, "4");
		addElementAt(0, 5, "8");
		addElementAt(0, 6, "16");
		addElementAt(1, 0, "X[n] \031");
		addElementAt(1, 1, "Time    \032");
		addElementAt(1, 2, "ms");
		addElementAt(1, 3, "ms");
		addElementAt(1, 4, "ms");
		addElementAt(1, 5, "ms");
		addElementAt(1, 6, "ms");
		RowCount = 1;
		ColCount = 6;
	}
	
};

void ParallelPrefixSum(int *X, int *S, unsigned long size, unsigned short threadsNumber)
{
	int NumberOfThreads, *sChunks, *s = S;
#pragma omp parallel num_threads(threadsNumber)
	{
		int i;
#pragma omp single
		{
			sChunks = new int[threadsNumber + 1];
			sChunks[0] = 0;
		}
		int ThreadID = omp_get_thread_num();
		int sum = 0;
#pragma omp for schedule(static) 
		for (i = 0; i<size; i++) {
			sum += X[i];
			s[i] = sum;
		}
		sChunks[ThreadID + 1] = sum;
#pragma omp barrier
		int offset = 0;
		for (i = 0; i<(ThreadID + 1); i++) {
			offset += sChunks[i];
		}
#pragma omp for schedule(static)
		for (i = 0; i<size; i++) {
			s[i] += offset;
		}
	}
}

void SequencePrefixSum(int *X, int *S, unsigned long size) {
	int i;
	S[0] = X[0];
	for (i = 1; i < size; i++) {
		S[i] = S[i - 1] + X[i];
	}
}

void RandomArrayFill(int *X, int size, int minRange, int maxRange) {
	int i;
	for (i = 0; i < size; i++) {
		X[i] = minRange + (rand() % (int)(maxRange - minRange + 1));
	}
}

int main() {
	srand(time(NULL));
	StringArrayContainer obj;
	obj.InitializateArrayContainer();
	int N, *X, *S, *Sparallel, i = 0;
	int ileOstatnichWyswietlic = 20;
	cout << "Podaj wielkosc tablicy X[n], S[n]." << endl << "n = ";
	cin >> N;
	cout << endl;
	timeCount timediff{ 0,0 };
	X = new int[N];
	S = new int[N];
	Sparallel = new int[N];
	RandomArrayFill(X, N, -50, 50);
	obj.AddDataFromSource(X, 0, ileOstatnichWyswietlic, N);
	timediff.startTime();
	SequencePrefixSum(X,S,N);
	obj.SetDataFromSource(S, 2, ileOstatnichWyswietlic, N);
	timediff.endTime();
	obj.setTime(1, 2, to_string(timediff.measure()));
	timediff.reset();
	for (int j = 0; j < 4; j++) {
		timediff.startTime();
		int threadsNumber = 2 << j;
		ParallelPrefixSum(X, Sparallel, N, threadsNumber);
		timediff.endTime();
		obj.SetDataFromSource(S, j+3, ileOstatnichWyswietlic, N);
		obj.setTime(1, j + 3, to_string(timediff.measure()));
		timediff.reset();
	}
	obj.drawTable();
	_getch();
	return 0;
}
