#pragma once
#include "itkImage.h"
#include "itkTransform.h"
#include "itkRescaleIntensityImageFilter.h"

namespace DCR {
	/**
	* @brief base class for registration, implements setting the images and
	* converts them to floating point format, which is needed for registration in ITK registration framework
	*/
	class Registration
	{
		using PixelType = uint16_t;
		static const int dimension = 3;
		using InputImageType = itk::Image<PixelType, dimension>;
		using RegistrationImageType = itk::Image<double, dimension>;

	private:
		InputImageType::Pointer m_movingImage;
		InputImageType::Pointer m_fixedImage;

	protected:
		itk::RescaleIntensityImageFilter<InputImageType, RegistrationImageType>::Pointer m_fixedRescaler = itk::RescaleIntensityImageFilter<InputImageType, RegistrationImageType>::New();
		itk::RescaleIntensityImageFilter<InputImageType, RegistrationImageType>::Pointer m_movingRescaler = itk::RescaleIntensityImageFilter<InputImageType, RegistrationImageType>::New();;

	public:

		virtual itk::Transform<double, dimension, dimension>::ConstPointer GetMovingShift() = 0;

		void SetMoving(itk::Image<PixelType, dimension>::Pointer image) {
			image->UpdateOutputInformation();
			m_movingRescaler->SetInput(image);
			m_movingRescaler->UpdateOutputInformation();
			m_movingImage = image;
		}

		void SetFixed(itk::Image<PixelType, dimension>::Pointer image) {
			image->UpdateOutputInformation();
			m_fixedRescaler->SetInput(image);
			m_fixedRescaler->UpdateOutputInformation();
			m_fixedImage = image;
		}

	};
}