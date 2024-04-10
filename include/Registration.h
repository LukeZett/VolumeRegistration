#pragma once

#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

class Registration
{
	using PixelType = int16_t;
	using ImageType = itk::Image<PixelType, 3>;
	using TransformType = itk::TranslationTransform<double, 3>;
	using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
	using MetricType = itk::MeanSquaresImageToImageMetricv4<ImageType, ImageType>;
	using RegistrationType = itk::ImageRegistrationMethodv4<ImageType, ImageType, TransformType>;
	using CompositeTransformType = itk::CompositeTransform<double, 3>;

	MetricType::Pointer metric = MetricType::New();
	OptimizerType::Pointer optimizer = OptimizerType::New();
	RegistrationType::Pointer registration = RegistrationType::New();
	TransformType::Pointer movingTransform = TransformType::New();

public:
	Registration()
	{
		registration->SetOptimizer(optimizer);
		registration->SetMetric(metric);
		movingTransform->SetIdentity();
		registration->SetMovingInitialTransform(movingTransform);
	}

	void SetMoving(ImageType* image) {
		registration->SetMovingImage(image);
	}
	
	void SetFixed(ImageType* image) {
		registration->SetFixedImage(image);
	}

	CompositeTransformType::ConstPointer Register() {
		registration->Update();

		CompositeTransformType::Pointer transform = CompositeTransformType::New();
		transform->AddTransform(registration->GetModifiableTransform());
		transform->AddTransform(movingTransform);
		return transform;
	}
};