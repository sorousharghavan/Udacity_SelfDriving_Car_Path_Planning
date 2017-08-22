#include <vector>
#include "LaneChangeStateMachine.h"
#include "Car.h"

#ifndef STATE_H
	#define STATE_H

using namespace std;

//all states inherit from this base state
class State {
	public:
		void init(LaneChangeStateMachine* state_machine);
		~State();
		State();
		virtual void process(Car car, vector<vector<double>> sensor_fusion_data, int prev_size);
		LaneChangeStateMachine* _state_machine;
};
#endif //state_h

