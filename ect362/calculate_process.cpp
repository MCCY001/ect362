#include "calculate_process.h"

double CalculateVolumeFlowRate(double max_tank_volume, double max_fill_time) {
	if (max_fill_time == 0) {
		return -1;
	}
	return max_tank_volume / max_fill_time;
}

PipeSize FindActualPipe(const std::vector<PipeSize>& pipe_size_list, double max_tank_volume, double max_fill_time) {
	double volume_flow_rate = CalculateVolumeFlowRate(max_tank_volume, max_fill_time);
	const double flow_velocity = 9.0;
	double closest = DBL_MAX;
	PipeSize actual_pipe{};
	double ideal_pipe_diameter_in = ((2 * sqrt(volume_flow_rate / (PI * flow_velocity))) / PI) * 12;

	for (const PipeSize& pipe : pipe_size_list) {
		if (pipe.nominal_pipe_size_in > ideal_pipe_diameter_in && pipe.nominal_pipe_size_in < closest) {
			actual_pipe = pipe;
			closest = pipe.nominal_pipe_size_in;
		}
	}
	return actual_pipe;
}

double CalculateActualFlowVelocity(const PipeSize& actual_pipe, double max_tank_volume, double max_fill_time) {
	double volume_flow_rate = CalculateVolumeFlowRate(max_tank_volume, max_fill_time);
	return volume_flow_rate / (PI * std::pow(actual_pipe.nominal_pipe_size_in / 2, 2));
}

char* CalculateReynoldsNumber(const PipeSize& actual_pipe, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time) {
	double actual_flow_velocity = CalculateActualFlowVelocity(actual_pipe, max_tank_volume, max_fill_time);
	double re = (actual_flow_velocity * (actual_pipe.nominal_pipe_size_in / 12.0)) / selected_fluid.kinematic_viscosity;
	char* re_result = (char*)malloc(100);
	if (re_result == NULL) {
		return NULL;
	}
	if (re <= 2000) {
		sprintf_s(re_result, 100, "Re: %.2f, type: Laminar", re);
	}
	else if (re <= 4000) {
		sprintf_s(re_result, 100, "Re: %.2f, type: Transitional", re);
	}
	else {
		sprintf_s(re_result, 100, "Re: %.2f, type: Turbulent", re);
	}
	return re_result;
}

PipeSelectionResult Calculate(const std::vector<PipeSize>& pipe_size_list, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time) {
	PipeSize actual_pipe = FindActualPipe(pipe_size_list, max_tank_volume, max_fill_time);
	double actual_flow_velocity = CalculateActualFlowVelocity(actual_pipe, max_tank_volume, max_fill_time);
	char* re = CalculateReynoldsNumber(actual_pipe, selected_fluid, max_tank_volume, max_fill_time);
	return PipeSelectionResult(actual_pipe, actual_flow_velocity, re);
}

