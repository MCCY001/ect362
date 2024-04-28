#include "display.h"


void DrawFluidData(HDC hdc, const std::vector<FluidType>& fluid_type_list) {
	int x = 10;
	int y = 0;
	int yStep = 20;

	std::string header = "Fluid Name:";
	TextOut(hdc, x, y, header.c_str(), header.length());
	y += yStep;

	for (const auto& fluid : fluid_type_list) {
		char text[512];
		sprintf_s(text, sizeof(text), "%-15s",
			fluid.name.c_str());

		TextOut(hdc, x, y, text, strlen(text));
		y += yStep;
	}
}

void DrawPipeData(HDC hdc, const std::vector<PipeSize>& pipe_size_list) {
	int x = 300;
	int y = 0;
	int yStep = 20;
	std::string header = "Pipe data:";
	TextOut(hdc, x, y, header.c_str(), header.length());
	y += yStep;
	header = "ID    Nominal Pipe Size (in)    Nominal Pipe Size (mm)    Outside Diameter (in)    Outside Diameter (mm)    Wall Thickness (in)    Wall Thickness (mm)    Inside Diameter (in)    Inside Diameter (ft)    Flow Area (mm)    Flow Area (mm2)    Flow Area (m2)";
	TextOut(hdc, x, y, header.c_str(), header.length());
	y += yStep;

	for (const auto& pipeSize : pipe_size_list) {
		char text[512];
		sprintf_s(text, sizeof(text), "%-5u %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f",
			pipeSize.id,
			pipeSize.nominal_pipe_size_in,
			pipeSize.nominal_pipe_size_mm,
			pipeSize.outside_diameter_in,
			pipeSize.outside_diameter_mm,
			pipeSize.wall_thickness_in,
			pipeSize.wall_thickness_mm,
			pipeSize.inside_diameter_in,
			pipeSize.inside_diameter_ft,
			pipeSize.flow_area_mm,
			pipeSize.flow_area_mm2,
			pipeSize.flow_area_m2);
		TextOut(hdc, x, y, text, strlen(text));
		y += yStep;
	}
}

void DisplaySelectedData(HDC hdc, PipeSelectionResult result) {
	int x = 250;
	int y = 250;
	int yStep = 20;
	std::string header = "Pipe data:";
	TextOut(hdc, x, y, header.c_str(), header.length());
	y += yStep;
	header = "ID    Nominal Pipe Size (in)    Nominal Pipe Size (mm)    Outside Diameter (in)    Outside Diameter (mm)    Wall Thickness (in)    Wall Thickness (mm)    Inside Diameter (in)    Inside Diameter (ft)    Flow Area (mm)    Flow Area (mm2)    Flow Area (m2)";
	TextOut(hdc, x, y, header.c_str(), header.length());
	y += yStep;
	char text[512];
	sprintf_s(text, sizeof(text), "%-5u %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %-15.2f",
		result.actual_pipe.id,
		result.actual_pipe.nominal_pipe_size_in,
		result.actual_pipe.nominal_pipe_size_mm,
		result.actual_pipe.outside_diameter_in,
		result.actual_pipe.outside_diameter_mm,
		result.actual_pipe.wall_thickness_in,
		result.actual_pipe.wall_thickness_mm,
		result.actual_pipe.inside_diameter_in,
		result.actual_pipe.inside_diameter_ft,
		result.actual_pipe.flow_area_mm,
		result.actual_pipe.flow_area_mm2,
		result.actual_pipe.flow_area_m2);
	TextOut(hdc, x, y, text, strlen(text));
	y += yStep;
	TextOut(hdc, x, y, result.re.c_str(), result.re.size());
}
