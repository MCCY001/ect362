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

std::string CalculateReynoldsNumber(const PipeSize& actual_pipe, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time) {
    double actual_flow_velocity = CalculateActualFlowVelocity(actual_pipe, max_tank_volume, max_fill_time);
    double re = (actual_flow_velocity * (actual_pipe.nominal_pipe_size_in / 12.0)) / selected_fluid.kinematic_viscosity;
    std::string re_number = std::to_string(re);
    std::string re_result = "";
    if (re <= 2000) {
        re_result = "Re: " + re_number + ", type: " + "Laminar";
    }
    else if (re <= 4000) {
        re_result = "Re: " + re_number + ", type: " + "Transitional";
    }
    else {
        re_result = "Re: " + re_number + ", type: " + "Turbulent";
    }
    return re_result;
}

PipeSelectionResult Calculate(const std::vector<PipeSize>& pipe_size_list, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time) {
    PipeSize actual_pipe = FindActualPipe(pipe_size_list, max_tank_volume, max_fill_time);
    double actual_flow_velocity = CalculateActualFlowVelocity(actual_pipe, max_tank_volume, max_fill_time);
    std::string re = CalculateReynoldsNumber(actual_pipe, selected_fluid, max_tank_volume, max_fill_time);
    return PipeSelectionResult(actual_pipe, actual_flow_velocity, re);
}

