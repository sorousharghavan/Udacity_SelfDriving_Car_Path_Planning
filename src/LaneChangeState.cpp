#include "LaneChangeState.h"
#include "CruiseState.h"
#include <math.h>
#include <iostream>

LaneChangeState::LaneChangeState() {}

LaneChangeState::~LaneChangeState() {}

//whether changing lanes has begun. In this case we don't want to consider changing lanes again and just focus on following through safely
bool in_lane_change = false;
//after lane change started, what is the departing lane and the target lane. This is used to check whether we are too close to any cars
//in our target lane or initial lane to avoid crashing
int initial_lane = -1;
int target_lane = -1;

void LaneChangeState::process(Car car, vector<vector<double>> sensor_fusion, int prev_size) {
	cout << "LaneChangeState::process" << endl;
	
	double car_s = car.s;
	double car_speed = car.speed;
	
	int* lane = _state_machine->lane;
	double* ref_vel = _state_machine->target_velocity;
	
	//wether the lanes are safe (only safe if no cars are in those lanes)
	bool is_left_safe = true;
	bool is_right_safe = true;
	
	//flag to check whether we are too close to the car in front to begin lane change and slow down if necessary
	bool too_close = false;
	//safe speed is the lowest of speed of the car in front or speed limit
	double safe_speed = 49;
	//if we are dangerously close to the car in front, brake harder.
	bool brake_immediately = false;
	
	cout << "Initial lane: " << initial_lane << endl;
	cout << "Target lane: " << target_lane << endl;
	
	if (in_lane_change && target_lane != -1 && car.d<(4*(target_lane+1)-1.5) && car.d > (4*(target_lane)+1.5)){
		std::cout << "Going to cruise state" << std::endl;
		//deinitialize variables
		target_lane = -1;
		initial_lane = -1;
		in_lane_change = false;
		//change to cruise state
		_state_machine->_state = new CruiseState();
		_state_machine->_state->init(_state_machine);
		return;
	}
	
	cout << "Check surroundings" << endl;
	
	for (int i=0; i<sensor_fusion.size();i++){
		float d = sensor_fusion[i][6];
		double vx = sensor_fusion[i][3];
		double vy = sensor_fusion[i][4];
		double check_speed = sqrt(vx*vx+vy*vy);
		double check_car_s = sensor_fusion[i][5];
		check_car_s+=((double)prev_size*0.02*check_speed);		
		
		//monitoring lanes
		if (in_lane_change == false){
			//our lane
			if (d<(4*(*lane+1)) && d > (4*(*lane))){
				//if the car ahead is too close
				if (check_car_s>car_s && (check_car_s-car_s)<50){
					too_close = true;
					//if (check_speed < safe_speed) {safe_speed = check_speed;}
					cout << "TOO CLOSE NEED TO CHANGE" << endl;
					
					//if it is VERY close, brake NOW
					if (fabs(check_car_s-car_s)<10) {
						brake_immediately = true;
					}
				}
			} 
			//checking left lane
			if (*lane == 0 || (d<((*lane)*4) && d > ((*lane-1)*4))){
				//if we are in left lane, mark our left lane as unsafe.
				//else if there is a car 10 units behind us or 60 in front, lane is occupied and unsafe to go to
				if (*lane == 0 || (check_car_s > (car_s - 10) && check_car_s < (car_s + 60))){
					is_left_safe = false;
					cout << "**********LEFT IS OCCUPIED!" << endl;
				}
			} 
			//same as left lane but now for right lane.
			if (*lane == 2 || (d<(4*(*lane+2)) && d > (4*((*lane)+1)))){
				if (*lane == 2 || (check_car_s > (car_s - 10) && check_car_s < (car_s + 60))){
					is_right_safe = false;
					cout << "**********RIGHT IS OCCUPIED!" << endl;
				}
			}
		} else if (initial_lane != -1 && target_lane != -1){
			//We are in the middle of a lane change. let's check speeds to see if we are safe or need to brake.
			if (fabs(check_car_s-car_s)<40 && ((d<(4*(initial_lane+1)) && d > (4*(initial_lane))) || (d<(4*(target_lane+1)) && d > (4*(target_lane)) ) )){				
				//if (check_speed < safe_speed) {safe_speed = check_speed;}
				too_close = true;
				cout << "TOO CLOSE during lane change" << endl;
				//cout << "My D: " << car.d << endl;
				//cout << "Their D: " << d << endl;
				//cout << "My S: " << car.s << endl;
				//cout << "Their S: " << check_car_s << endl;
			} 
		}
	}
	
	if (too_close){
		//minimum speed is 35, for safety
		if (*(_state_machine->target_velocity) > 35) {
			*(_state_machine->target_velocity) -= 0.15;
		} 
		//even if speed is 35 but we are too close, brake.
		if (brake_immediately) {
			*(_state_machine->target_velocity) -= 0.22;
		}
		if (in_lane_change == false){
			initial_lane = *lane;
			//change lane to left. left has priority. we go to right if left is blocked.
			if (*lane > 0 && is_left_safe){
				cout << "Moving to LEFT lane" << endl;
				*lane -= 1;
				target_lane = *lane;
				in_lane_change = true;
			} else if (*lane < 2 && is_right_safe){
				//change lane to right. if right is blocked too, just slow down and follow the car ahead.
				cout << "Moving to RIGHT lane" << endl;
				*lane += 1;
				target_lane = *lane;
				in_lane_change = true;
			}
		}
	} else if (*ref_vel < 49){
		//we have enough room, give it some gas (or electrons)!
		*(_state_machine->target_velocity) += 0.15;
	}
}

