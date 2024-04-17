#pragma once

#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkRescaleIntensityImageFilter.h"

class Registration
{
	using PixelType = float;
	using ImageType = itk::Image<PixelType, 3>;
	using InputImageType = itk::Image<int16_t, 3>;

	using ConvertType = itk::RescaleIntensityImageFilter<InputImageType, ImageType>;
	using TransformType = itk::TranslationTransform<double, 3>;
	using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
	using MetricType = itk::MeanSquaresImageToImageMetricv4<ImageType, ImageType>;
	using RegistrationType = itk::ImageRegistrationMethodv4<ImageType, ImageType, TransformType>;
	using CompositeTransformType = itk::CompositeTransform<double, 3>;

	MetricType::Pointer metric = MetricType::New();
	OptimizerType::Pointer optimizer = OptimizerType::New();
	RegistrationType::Pointer registration = RegistrationType::New();
	TransformType::Pointer movingTransform = TransformType::New();
	ConvertType::Pointer converterFixed = ConvertType::New();
	ConvertType::Pointer converterMoving = ConvertType::New();

public:
	Registration()
	{
		Init();
	}

	void SetMoving(InputImageType* image) {
		converterMoving->SetInput(image);
		converterMoving->Update();
	}
	
	void SetFixed(InputImageType* image) {
		converterFixed->SetInput(image);
		converterFixed->Update();
	}

	CompositeTransformType::ConstPointer Register() {
		registration->Update();

		CompositeTransformType::Pointer transform = CompositeTransformType::New();
		transform->AddTransform(registration->GetModifiableTransform());
		transform->AddTransform(movingTransform);
		return transform;
	}

	OptimizerType::Pointer GetOptimizer() {
		return optimizer;
	}

private:
	void Init();
};