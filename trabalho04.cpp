#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <chrono>
#include <sched.h>
#include <thread>
#include <pthread.h>
#include <string>
#include <fstream>

using namespace std;

void fillVector(vector<float> & v,int seed){
	this_thread::sleep_for(chrono::milliseconds(100));
	//cout << "cpu = " << sched_getcpu() << endl;
	auto t1 = chrono::high_resolution_clock::now();

	srand(seed);
	for(int i = 0; i < v.size(); i++){
		v[i] = static_cast<float> (rand()) / static_cast<float>(RAND_MAX) * 2.0f - 1.0f;
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> tempo = t2 - t1;

	string path = "fill";
	ofstream file;
	file.open(path, ofstream::app);
	if(file.is_open()){
		file << fixed << setprecision(4) << tempo.count() << endl;
		file.close();
	}
}

void sum(vector<float> &v, string path){
	this_thread::sleep_for(chrono::milliseconds(100));
	auto t1 = chrono::high_resolution_clock::now();

	float s = 0.0f;
	for(int i = 0; i < v.size(); i++){
		s += v[i];
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> tempo = t2 - t1;

	path += to_string(sched_getcpu());
	ofstream file;
	file.open(path, ofstream::app);
	if(file.is_open()){
		file << fixed << setprecision(4) << tempo.count() << endl;
		file.close();
	}
}

void sumSin(vector<float> &v, string path){
	this_thread::sleep_for(chrono::milliseconds(100));
	auto t1 = chrono::high_resolution_clock::now();

	float s = 0.0f;
	for(int i = 0; i < v.size(); i++){
		s += sin(v[i]);
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> tempo = t2 - t1;

	path += to_string(sched_getcpu());
	ofstream file;
	file.open(path, ofstream::app);
	if(file.is_open()){
		file << fixed << setprecision(4) << tempo.count() << endl;
		file.close();
	}
}

void sumLog(vector<float> &v, string path){
	this_thread::sleep_for(chrono::milliseconds(100));
	auto t1 = chrono::high_resolution_clock::now();

	float s = 0.0f;
	for(int i = 0; i < v.size(); i++){
		if(v[i] > 0.0f){
			s += log10(v[i]);
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> tempo = t2 - t1;

	path += to_string(sched_getcpu());
	ofstream file;
	file.open(path, ofstream::app);
	if(file.is_open()){
		file << fixed << setprecision(4) << tempo.count() << endl;
		file.close();
	}

}

int main(){
	cpu_set_t cpuset;
	vector<float> v(100000000);
	for(int i = 0; i < 10; i++){
		float tempof, temposum, temposin, tempolog;
		thread tfill(fillVector, ref(v), i);
		tfill.join();

		string s = "sumT1_2CPU_";
		thread t(sum, ref(v), s);

		s = "sumT2_2CPU_";
		thread tsin(sum, ref(v), s);

		//string s = "log_";
		//thread t(sumLog, ref(v), s);

		//s = "log_";
		//thread tsin(sumLog, ref(v), s);

		//string s = "sum_";
		//thread t(sum, ref(v), s);

		//s = "sum_";
		//thread tsin(sum, ref(v), s);

		//thread tsum(sum, ref(v), s);
		//thread tlog(sumLog, ref(v), s);

		CPU_ZERO(&cpuset);
		CPU_SET(0, &cpuset);
		pthread_setaffinity_np(tsin.native_handle(), sizeof(cpu_set_t), &cpuset);
		CPU_ZERO(&cpuset);
		CPU_SET(1, &cpuset);
		pthread_setaffinity_np(t.native_handle(), sizeof(cpu_set_t), &cpuset);

		//tsum.join();
		tsin.join();
		//tlog.join();
		t.join();
	}

	return 0;
}
