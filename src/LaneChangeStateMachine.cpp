#include "LaneChangeStateMachine.h"
#include "CruiseState.h"

void LaneChangeStateMachine::init(int* lane, double* vel) {
	this->lane = lane;
	this->target_velocity = vel;
	this->_state = new CruiseState();
	_state->init(this);
}

LaneChangeStateMachine::~LaneChangeStateMachine(){
	delete _state;
}

void LaneChangeStateMachine::processLane(Car car, vector<vector<double>> sensor_fusion_data, int prev_size) {
	_state->process(car, sensor_fusion_data, prev_size);
}

