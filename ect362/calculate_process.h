#pragma once
#include <cmath>
#include <cfloat>
#include "data.h"
#define PI 3.14159265358979323846

struct PipeSelectionResult {
    PipeSize actual_pipe;
    double actual_flow_velocity;
    std::string re = "";
    PipeSelectionResult(PipeSize actual_pipe = {}, double actual_flow_velocity = 0.0, std::string re = "") {
        this->actual_pipe = actual_pipe;
        this->actual_flow_velocity = actual_flow_velocity;
        this->re = re;
    }
};

double CalculateVolumeFlowRate(double max_tank_volume, double max_fill_time);

PipeSize FindActualPipe(const std::vector<PipeSize>& pipe_size_list, double max_tank_volume, double max_fill_time);

double CalculateActualFlowVelocity(const PipeSize& actual_pipe, double max_tank_volume, double max_fill_time);

std::string CalculateReynoldsNumber(const PipeSize& actual_pipe, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time);

PipeSelectionResult Calculate(const std::vector<PipeSize>& pipe_size_list, const FluidType& selected_fluid, double max_tank_volume, double max_fill_time);

