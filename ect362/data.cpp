#include "data.h"

#include <fstream>
#include <sstream>
#include <iostream>


const std::vector<FluidType> ReadFluidData(const std::string& file_path) {
    std::vector<FluidType> fluids;
    std::ifstream file(file_path);

    if (!file) {
        std::cerr << "Failed in open file: " << file_path << std::endl;
        return fluids;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        FluidType fluid;
        std::string token;
        std::getline(ss, fluid.name, ',');

        std::getline(ss, token, ',');
        fluid.specific_gravity = std::stod(token);
        std::getline(ss, token, ',');
        fluid.specific_weight = std::stod(token);
        std::getline(ss, token, ',');
        fluid.density = std::stod(token);
        std::getline(ss, token, ',');
        fluid.dynamic_viscosity = std::stod(token);
        std::getline(ss, token, ',');
        fluid.kinematic_viscosity = std::stod(token);

        fluids.push_back(fluid);
    }

    file.close();
    return fluids;
}

const std::vector<PipeSize> ReadPipeData(const std::string& file_path) {
    std::vector<PipeSize> pipes;
    std::ifstream file(file_path);
    int id = 0;
    if (!file) {
        std::cerr << "Failed in open file: " << file_path << std::endl;
        return pipes;
    }

    std::string line;
    int pipe_id = 0;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        PipeSize pipeSize;
        std::string token;

        pipeSize.id = pipe_id++;


        std::getline(ss, token, ',');
        pipeSize.nominal_pipe_size_in = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.nominal_pipe_size_mm = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.outside_diameter_in = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.outside_diameter_mm = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.wall_thickness_in = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.wall_thickness_mm = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.inside_diameter_in = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.inside_diameter_ft = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.flow_area_mm = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.flow_area_mm2 = std::stod(token);

        std::getline(ss, token, ',');
        pipeSize.flow_area_m2 = std::stod(token);

        pipes.push_back(pipeSize);
    }

    file.close();
    return pipes;
}

void WriteCustomerToFile(const CustomerOrder& customer, const std::string& file_path) {
    std::ofstream file(file_path, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    file << "Customer ID: " << customer.customer_id << "\n"
        << "Max Tank Volume: " << customer.max_tank_volume << " cubic feet\n"
        << "Max Fill Time: " << customer.max_fill_time << " seconds\n"
        << "Selected Pipe Diameter: " << customer.selected_pipe.nominal_pipe_size_in << " inches\n";

    file << "\n";

    file.close();
}