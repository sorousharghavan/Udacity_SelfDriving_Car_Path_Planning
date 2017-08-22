#include "CruiseState.h"
#include "LaneChangeState.h"
#include <math.h>
#include <iostream>

CruiseState::CruiseState() {
}

CruiseState::~CruiseState() {
	
}

void CruiseState::process(Car car, vector<vector<double>> sensor_fusion, int prev_size) {
	cout << "CruiseState::process" << endl;
	double car_s = car.s;
	double car_speed = car.speed;
	int* lane = _state_machine->lane;
	double* ref_vel = _state_machine->target_velocity;

	bool too_close = false;
	
	//find whether we are too close to the car in front or not
	for (int i=0; i<sensor_fusion.size();i++){
		//car in my lane
		float d = sensor_fusion[i][6];
		double vx = sensor_fusion[i][3];
		double vy = sensor_fusion[i][4];
		double check_speed = sqrt(vx*vx+vy*vy);
		double check_car_s = sensor_fusion[i][5];
		check_car_s+=((double)prev_size*0.02*check_speed);
		
		//if it is in our lane
		if (d<(4*(*lane+1)) && d > (4*(*lane))){
			//if we are too close
			if (check_car_s>car_s && check_car_s-car_s<50){
				too_close = true;
				std::cout << "Going to lane change state" << std::endl;
				_state_machine->_state = new LaneChangeState();
				_state_machine->_state->init(_state_machine);
				return;
			}
		}
	}
	//brake if too close
	if (too_close == false && *ref_vel < 49) {
		*ref_vel += 0.2;
	}
}

