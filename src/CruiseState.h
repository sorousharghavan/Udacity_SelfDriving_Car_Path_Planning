#include "State.h"
#include "Car.h"

#ifndef CRUISE_STATE_H
	#define CRUISE_STATE_H
	
//Car is in this state by default and goes to this state after every lane change
class CruiseState : public State {
	public:
		CruiseState();
		~CruiseState();
		virtual void process(Car car, vector<vector<double>> sensor_fusion_data, int prev_size);
};
#endif //cruise_state_h

