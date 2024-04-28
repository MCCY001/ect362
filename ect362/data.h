#pragma once
#include <string>
#include <vector>

struct FluidType {
	std::string name = "";
	double specific_gravity = 0;
	double specific_weight = 0;
	double density = 0;
	double dynamic_viscosity = 0;
	double kinematic_viscosity = 0;
};

struct PipeSize {
	unsigned int id = 0;
	double nominal_pipe_size_in = 0;
	double nominal_pipe_size_mm = 0;
	double outside_diameter_in = 0;
	double outside_diameter_mm = 0;
	double wall_thickness_in = 0;
	double wall_thickness_mm = 0;
	double inside_diameter_in = 0;
	double inside_diameter_ft = 0;
	double flow_area_mm = 0;
	double flow_area_mm2 = 0;
	double flow_area_m2 = 0;
};

struct CustomerOrder {
	unsigned int customer_id = 0;
	double max_tank_volume;
	double max_fill_time;
	PipeSize selected_pipe;
};

const std::vector<FluidType> ReadFluidData(const std::string& file_path);
const std::vector<PipeSize> ReadPipeData(const std::string& file_path);
void WriteCustomerToFile(const CustomerOrder& customer, const std::string& file_path);

#define ID_CONFIRM_BUTTON 1000

#define FLUID_TYPE_PREFIX 2000

#define MAXIMUM_TANKER_TRUCK_VOLUME_EDIT 3000
#define MAXIMUM_TANKER_FILL_TIME_EDIT 3001
#define CUSTOMER_ID_EDIT 3002
