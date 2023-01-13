#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>

struct Train {
	std::string name;
	int time = 0;
};

std::vector<Train> station;
std::mutex station_mutex;

void GoWaitGo(Train train) {
	std::this_thread::sleep_for(std::chrono::seconds(train.time));

	station_mutex.lock();
	std::cout << "Train " << train.name << " arrival to the station!" << std::endl;
	std::string command;
	std::cout << "Input command for depart:" << std::endl;
	std::cin >> command;
	while (command != "depart") {
		std::cout << "ERROR input. Try again:" << std::endl;
		std::cin >> command;
	}
	station.push_back(train);
	std::cout << "Train " << train.name << " departs from station!" << std::endl;
	station_mutex.unlock();
	std::cout << "-------------------------------------------------" << std::endl;
}

void Wait(int totalRemainTime) {
	int currRemainTime = 1;
	while(totalRemainTime > currRemainTime) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "\t\t" << currRemainTime << "...." << std::endl;
		currRemainTime++;
	}
}

int main() {
	std::vector<Train> trains;
	for (int i = 0; i < 3; ++i) {
		Train in_train;
		std::cout << "Input time to arrival train ";
		if (i == 0) {
			in_train.name = "A";
			std::cout << in_train.name;
		}
		if (i == 1) {
			in_train.name = "B";
			std::cout << in_train.name;
		}
		if (i == 2) {
			in_train.name = "C";
			std::cout << in_train.name;
		}
		std::cout << " on station:" << std::endl;
		std::cin >> in_train.time;
		assert(in_train.time > 0);
		trains.push_back(in_train);
	}
	int timeMin = 1000000000;
	for (int i = 0; i < trains.size(); ++i) {
		if (trains[i].time < timeMin) timeMin = trains[i].time;
	}

	std::cout << "Trains start....0...." << std::endl;
	
	std::thread tr_wait(Wait, timeMin);

	std::thread tr_A(GoWaitGo, trains[0]);
	std::thread tr_B(GoWaitGo, trains[1]);
	std::thread tr_C(GoWaitGo, trains[2]);

	tr_wait.join();
	tr_A.join();
	tr_B.join();
	tr_C.join();

	std::cout << "Exit program. Good by." << std::endl;

	return 0;
}
