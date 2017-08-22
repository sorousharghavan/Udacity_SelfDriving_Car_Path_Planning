#include <vector>
#include "Car.h"

#ifndef LANE_STATE_MACHINE_H
	#define LANE_STATE_MACHINE_H

using namespace std;

//forward declaration
class State;

class LaneChangeStateMachine {
	public:
		void init(int* lane, double* vel);
		~LaneChangeStateMachine();
		int* lane;
		double* target_velocity;
		void processLane(Car car, vector<vector<double>> sensor_fusion_data, int prev_size);
		State* _state;
};

#include "State.h"

#endif //lane_state_machine_h
