#include <vector>
#include "State.h"

using namespace std;

void State::init(LaneChangeStateMachine* state_machine){
	this->_state_machine = state_machine;
}

State::~State(){
	
}
State::State(){
	
}
void State::process(Car car, vector<vector<double>> sensor_fusion_data, int prev_size) {
}