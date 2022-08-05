#include <iostream>
#include <string>
#include <sstream>
using namespace std;

enum switches {
	LIGHTS_INSIDE = 1,
	LIGHTS_OUTSIDE = 2,
	HEATERS = 4,
	WATER_PIPE_HEATING = 8,
	CONDITIONER = 16
};

void changeTime(int &time, int &day) {
	time++;
	if (time == 24) {
		time = 0;
		day++;
	}
}

void wphOnOff(int tempr_out, int &state) {
	if (tempr_out < 0 && !(state & WATER_PIPE_HEATING)) {
		state |= WATER_PIPE_HEATING;
		cout << "Water pipe heating is on!" << endl;
	}
	else if (tempr_out > 5 && state & WATER_PIPE_HEATING) {
		state &= ~WATER_PIPE_HEATING;
		cout << "Water pipe heating is off!" << endl;
	}
}

void lightOutOnOff(int time, int &state, string motion_out) {
	if (time < 5 || time > 16 && !(state & LIGHTS_OUTSIDE)) {
		if (motion_out == "yes") {
			state |= LIGHTS_OUTSIDE;
			cout << "Lights outside is on!" << endl;
		}
		else if (state & LIGHTS_OUTSIDE) {
			state &= ~LIGHTS_OUTSIDE;
			cout << "Lights outside is off!" << endl;
		}
	}
	else if (time >= 5 && time <= 16 && state & LIGHTS_OUTSIDE) {
		state &= ~LIGHTS_OUTSIDE;
		cout << "Lights outside is off!" << endl;
	}
}

void heatersOnOff(int tempr_in, int &state) {
	if (tempr_in < 22 && !(state & HEATERS)) {
		state |= HEATERS;
		cout << "Heaters is on!" << endl;
	}
	else if (tempr_in >= 25 && state & HEATERS) {
		state &= ~HEATERS;
		cout << "Heaters is off!" << endl;
	}
}

void conditionerOnOff(int tempr_in, int &state) {
	if (tempr_in >= 30 && !(state & CONDITIONER)) {
		state |= CONDITIONER;
		cout << "Conditioner is on" << endl;
	}
	else if (tempr_in <= 25 && state & CONDITIONER) {
		state &= ~CONDITIONER;
		cout << "Conditioner is off" << endl;
	}
}

void lightInOnOff(string lights_in, int light_tempr, int &state) {
	if (lights_in == "on" && !(state & LIGHTS_INSIDE)) {
		state |= LIGHTS_INSIDE;
		cout << "Lights inside is on! Colorful temperature: ";
		cout << light_tempr << "K" << endl;
	}
	else if (lights_in == "on" && state & LIGHTS_INSIDE) {
		cout << "Colorful temperature: " << light_tempr << "K" << endl;
	}
	else if (lights_in == "off" && state & LIGHTS_INSIDE) {
		state &= ~LIGHTS_INSIDE;
		cout << "Lights inside is off!" << endl;
	}
}

int changeLightTempr(int light_tempr, int time) {
	if (time >= 16 && time <= 20) {
		light_tempr -= 460;
	}
	if (time == 0)
		light_tempr = 5000;
	return light_tempr;
}

int main() {
	int switches_state = 0, time = 0, day = 1;
	string buffer;
	int tempr_in, tempr_out, light_tempr = 5000;
	string motion_out, lights_in;
	do {
		cout << "---------------------------------------------------" << endl;
		cout << "Day: " << day << ", Time: " << time << ":00" << endl;
		cout << "Enter the status of all major sensors and lights: ";
		getline(cin, buffer);
		stringstream buffer_stream(buffer);
		string prob;
		buffer_stream >> tempr_out >> tempr_in;
		buffer_stream >> motion_out >> lights_in;
		wphOnOff(tempr_out, switches_state);
		lightOutOnOff(time, switches_state, motion_out);
		heatersOnOff(tempr_in, switches_state);
		conditionerOnOff(tempr_in, switches_state);
		light_tempr = changeLightTempr(light_tempr, time);
		lightInOnOff(lights_in, light_tempr, switches_state);
		changeTime(time, day);
	} while (time <= 24 && day == 1);
	return 0;
}