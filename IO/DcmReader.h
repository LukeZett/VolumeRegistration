#pragma once

#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaDataDictionary.h"

#include <filesystem>
#include <iostream>

namespace DCR
{
	class DcmReader
	{
		using PixelType = int16_t;
		using ImageType = itk::Image<PixelType, 3>;
		using ReaderType = itk::ImageSeriesReader<ImageType>;
		using ImageIOType = itk::GDCMImageIO;
		using NamesGeneratorType = itk::GDCMSeriesFileNames;

		ReaderType::Pointer reader = ReaderType::New();
		ImageIOType::Pointer dicomIO = ImageIOType::New();
		NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
		std::string seriesID;
		std::vector<std::string> filenames;

	public:

		/**
		* @brief Initialize reader and reader's directory
		* @param directory[in] directory containing dicom series
		* @param seriesID[in] id of series to be read (first series will be read if not provided)
		*/
		DcmReader(const std::filesystem::path& directory, const std::string& seriesID = "None");

		/**
		* @brief retrieve IDs for all series in specified reader directory
		* @returns vector with IDs of all series
		*/
		inline const std::vector<std::string>& GetSeriesUIDs() const
		{
			return nameGenerator->GetSeriesUIDs();
		}

		/**
		* @brief set ID of series that will be read, if multiple IDs are present
		* @param[in] seriesID ID of series to be read(must be present in reader's directory)
		*/
		inline const void SpecifySeriesID(const std::string& seriesID)
		{
			this->seriesID = seriesID;
			filenames = nameGenerator->GetFileNames(seriesID);
			reader->SetFileNames(filenames);
		}

		/**
		* @brief update loaded image
		*/
		inline const void Update()
		{
			reader->Update();
		}

		inline ImageType* GetOutput() {
			reader->UpdateOutputInformation();
			return reader->GetOutput();
		}

		/**
		* @brief get ITK reader
		*/
		inline const ReaderType::Pointer& GetReader() const
		{
			return reader;
		}

	private:

	};
}
