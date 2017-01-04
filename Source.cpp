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
	void reset() { start = 0; end = 0; }
};
int main() {
	srand(time(NULL));
	int N, *X, *S, *Sp;
	cout << "Podaj wielkosc tablicy X[n], S[n]." << endl << "n = ";
	cin >> N;
	cout << endl;
	timeCount timediff{ 0,0 };
	timeCount timediffP{ 0,0 };
	X = new int[N];
	S = new int[N + 1];
	Sp = new int[N + 1];
	
	for (int i = 0; i < N; i++) {
		X[i] = (rand() % 20) - (rand() % 20);
	}
	
	//Sekwencyjne
	timediff.startTime();
	S[0] = 0;
	for (int i = 0; i < N; i++) {
		S[i+1] = S[i] + X[i];
	}
	timediff.endTime();
	cout << setw(5) << "X[]" << setw(3) << "|" << setw(5) << "S[]" << endl;
	for (int i = 0; i <= 20; i++) {
		if (i<N)
			cout << setw(5) << right << X[i];

		if (i == N) {
			cout << setw(8) << right << "|";
			cout << setw(2) << " ";
			cout << setw(5) << left << S[i];
		}

		else {
			cout << setw(3) << "|";
			cout << setw(2) << " ";
			cout << setw(5) << left << S[i];
		}

		cout << endl;
	}
	
	cout << "Uplynelo " << timediff.measure() << "ms - Sekwencyjne" << endl << endl << endl;
	//Koniec Sekwencyjnego 
	//Równoleg³e
	for (int j = 0; j < 4; j++) {
		
		timediffP.startTime();
		Sp[0] = 0;
		int i;
		int threadsNumber = 2 << j;
#pragma omp parallel for private(i) shared(N,X) num_threads(threadsNumber)
		for (i = 0; i < N; i++) {
			Sp[i + 1] = Sp[i] + X[i];
		}
		timediffP.endTime();
		cout << setw(5) << right << "X[]" << setw(3) << "|" << setw(5) << "S[]" << endl;
		for (int i = 0; i <= 20; i++) {
			if (i < N)
				cout << setw(5) << right << X[i];

			if (i == N) {
				cout << setw(8) << right << "|";
				cout << setw(2) << " ";
				cout << setw(5) << left << Sp[i];
			}

			else {
				cout << setw(3) << "|";
				cout << setw(2) << " ";
				cout << setw(5) << left << Sp[i];
			}

			cout << endl;
		}
		cout << "Uplynelo " << timediffP.measure() << "ms - Parallel. Dla " << threadsNumber <<" watkow" << endl << endl << endl;
		timediffP.reset();
	}
	//Koniec Równoleg³ego
	_getch();
	return 0;
}