#include "DcmReader.h"

using namespace DCR;

DcmReader::DcmReader(const std::filesystem::path& directory, const std::string& seriesID)
{
	std::string directoryString = directory.string();
	reader->SetImageIO(dicomIO);
	nameGenerator->SetDirectory(directoryString);

	if (seriesID == "None")
	{
		auto& serUIDs = GetSeriesUIDs();
		this->seriesID = *serUIDs.begin();
	}
	else this->seriesID = seriesID;

	filenames = nameGenerator->GetFileNames(this->seriesID);
	reader->SetFileNames(filenames);
}
