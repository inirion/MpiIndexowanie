#include "omp.h"
#include <iostream>
#include <time.h>
#include <conio.h>
#include <iomanip>
#include <Windows.h>

using namespace std;
struct timeCount{
	int start;
	int end;
	void startTime() { start = GetTickCount(); }
	void endTime() { end = GetTickCount();}
	int measure() {
		return end - start;
	}
};
int main() {
	srand(time(NULL));
	int N,*X,*S;

	cout << "Podaj wielkosc tablicy X[n], S[n]." << endl << "n = ";
	cin >> N;
	cout << endl;
	timeCount timediff{ 0,0 };
	X = new int[N];
	S = new int[N + 1];
	timediff.startTime();
	for (int i = 0; i < N; i++) {
		X[i] = (rand() % 400) - (rand() % 400);	
	}
	
	S[0] = 0;
	for (int i = 0; i <= N; i++) {
		S[i+1] = S[i] + X[i];
	}
	
	cout << setw(20) << "X[]" << setw(3)<< "|" << setw(5) << "S[]" << endl;
	for (int i = N - 20; i<=N; i++) {
		if(i<N)
			cout << setw(20) <<right<< X[i];
		
		if (i == N) {
			cout << setw(23) <<right<< "|";
			cout << setw(2) << " ";
			cout << setw(5) << left<< S[i];
		}
			
		else {
			cout << setw(3) << "|";
			cout << setw(2) << " ";
			cout << setw(5) << left << S[i];
		}
			
		cout << endl;
	}
	timediff.endTime();
	cout << setw(22) << "Uplynelo " << timediff.measure() << "ms";


	_getch();
	return 0;
}