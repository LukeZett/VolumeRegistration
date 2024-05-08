#pragma once

#include "itkImage.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkImageFileWriter.h"

#include <filesystem>

namespace DCR {
	class PNGExport
	{
		using InputType = itk::Image<int16_t, 3>;
		using RescalerType = itk::RescaleIntensityImageFilter<InputType, itk::Image<uint16_t, 3>>;
		using SlicerType = itk::ExtractImageFilter<itk::Image<uint16_t, 3>, itk::Image<uint16_t, 2>>;
		using WriterType = itk::ImageFileWriter<itk::Image<uint16_t, 2>>;

		itk::SmartPointer<RescalerType> rescaler = RescalerType::New();
		itk::SmartPointer<SlicerType> slicer = SlicerType::New();
		itk::SmartPointer<WriterType> writer = WriterType::New();

	public:
		/**
		* @brief initialize writer
		*/
		PNGExport();

		/**
		* @brief Write 3d image as PNG image series
		* @param[in] path directory for image series
		* @param[in] name prefix for name of PNGs [prefix]NN.png where NN is slice's index
		* @param[in] image pointer to 3D ITK image
		*/
		void WriteTo(const std::filesystem::path& path, const std::string& name, InputType* image);
	};
}