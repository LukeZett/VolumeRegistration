#pragma once

#include "itkSubtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkTranslationTransform.h"

class Difference
{
	using ImageType = itk::Image<int16_t, 3>;
	using ResamplerType = itk::ResampleImageFilter<ImageType, ImageType>;
	using DifferenceType = itk::SubtractImageFilter<ImageType, ImageType>;

	ResamplerType::Pointer resampler = ResamplerType::New();
	DifferenceType::Pointer diff = DifferenceType::New();

public:
	/**
	* @brief Initialize difference filter
	*/
	Difference() {
		diff->SetInput2(resampler->GetOutput());
	}

	/**
	* @brief Set reference image
	* @param[in] image pointer to ITK image
	*/
	void SetFixed(ImageType* image);

	/**
	* @brief Set moving image
	* @param[in] image pointer to ITK image
	*/
	void SetMoving(ImageType* image) {
		resampler->SetInput(image);
	}

	/**
	* @brief Set transform for moving image
	* @param[in] transform transform applied to moving image before difference
	*/
	void SetTransform(const itk::Transform<double, 3, 3>* transform) {
		resampler->SetTransform(transform);
	}

	/**
	* @brief Update filter content
	*/
	void Update() {
		diff->Update();
	}

	/**
	* @brief get output of filter as ITK image
	* @returns (reference - moving) as ITK image
	*/
	ImageType* GetOutput() {
		return diff->GetOutput();
	}
};