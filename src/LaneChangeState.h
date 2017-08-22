#include "State.h"
#include "Car.h"

#ifndef LANE_CHANGE_STATE_H
	#define LANE_CHANGE_STATE_H

//State defined for changing lanes. State machine enters this state if car is too close to the car in front	
class LaneChangeState : public State {
	public:
		LaneChangeState();
		~LaneChangeState();
		virtual void process(Car car, vector<vector<double>> sensor_fusion_data, int prev_size);
};
#endif //lane_change_state_h

