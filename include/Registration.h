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
		registration->SetOptimizer(optimizer);
		registration->SetMetric(metric);
		movingTransform->SetIdentity();
		registration->SetMovingInitialTransform(movingTransform);
		registration->SetMovingImage(converterMoving->GetOutput());
		registration->SetFixedImage(converterFixed->GetOutput());

		constexpr unsigned int numberOfLevels = 1; // setup for simple one-level registration
		RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
		shrinkFactorsPerLevel.SetSize(1);
		shrinkFactorsPerLevel[0] = 1;
		RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
		smoothingSigmasPerLevel.SetSize(1);
		smoothingSigmasPerLevel[0] = 0;
		registration->SetNumberOfLevels(numberOfLevels);
		registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
		registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

		optimizer->SetNumberOfIterations(10);
		optimizer->SetLearningRate(4);
		optimizer->SetMinimumStepLength(0.001);
		optimizer->SetRelaxationFactor(0.5);
	}

	void SetMoving(InputImageType* image) {
		converterMoving->SetInput(image);
	}
	
	void SetFixed(InputImageType* image) {
		converterFixed->SetInput(image);
	}

	CompositeTransformType::ConstPointer Register() {
		registration->Update();

		CompositeTransformType::Pointer transform = CompositeTransformType::New();
		transform->AddTransform(registration->GetModifiableTransform());
		transform->AddTransform(movingTransform);
		return transform;
	}
};