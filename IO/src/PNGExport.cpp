#include "PNGExport.h"
#include <sstream>
#include <string>

using namespace DCR;

PNGExport::PNGExport() {
	rescaler->SetOutputMinimum(0); // configure rescaling to exportable range
	rescaler->SetOutputMaximum(0xFFFF);

	slicer->InPlaceOn(); // configure slicing of 3D volume 
	slicer->SetDirectionCollapseToSubmatrix();

	slicer->SetInput(rescaler->GetOutput()); // connect export pipeline
	writer->SetInput(slicer->GetOutput());
}

void PNGExport::WriteTo(const std::filesystem::path& path, const std::string& name, InputType *image)
{
	if (!std::filesystem::exists(path)) {
		std::filesystem::create_directory(path);
	}


	rescaler->SetInput(image);
	rescaler->UpdateOutputInformation();

	auto& region = rescaler->GetOutput()->GetLargestPossibleRegion();
	auto size = region.GetSize();
	auto start = region.GetIndex();

	size[2] = 0;

	itk::Image<uint16_t, 3>::RegionType targetRegion;
	targetRegion.SetSize(size);
	targetRegion.SetIndex(start);

	for (size_t i = 0; i < region.GetSize()[2]; i++)
	{
		start[2] = i;
		targetRegion.SetIndex(start);
		slicer->SetExtractionRegion(targetRegion);

		writer->SetFileName((path / name).string() + std::to_string(i) + ".png");
		writer->Update();
	}

	writer->SetFileName((path / name).string());	
}
