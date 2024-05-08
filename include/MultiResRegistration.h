#pragma once

#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMattesMutualInformationImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkCorrelationImageToImageMetricv4.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include "itkMeanSquaresImageToImageMetricv4.h"

class MultiResRegistration {

public:
	using InputImageType = itk::Image<int16_t, 3>;

	using PixelType = float;
	using ImageType = itk::Image<PixelType, 3>;

	using ConvertType = itk::RescaleIntensityImageFilter<InputImageType, ImageType>;
	using TransformType = itk::TranslationTransform<double, 3>;
	using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
	using CCMetricType = itk::CorrelationImageToImageMetricv4<ImageType, ImageType>;
	using MIMetricType = itk::MattesMutualInformationImageToImageMetricv4<ImageType, ImageType>;
	using RegistrationType = itk::ImageRegistrationMethodv4<ImageType, ImageType, TransformType>;
	using CompositeTransformType = itk::CompositeTransform<double, 3>;

	ConvertType::Pointer fixedImageConv = ConvertType::New();
	ConvertType::Pointer movingImageConv = ConvertType::New();
	
	TransformType::Pointer movingTransform = TransformType::New();
	
	OptimizerType::Pointer optimizerCC = OptimizerType::New();
	CCMetricType::Pointer metricCC = CCMetricType::New();
	RegistrationType::Pointer registrationCC = RegistrationType::New();
	
	OptimizerType::Pointer optimizerMI = OptimizerType::New();
	MIMetricType::Pointer metricMI = MIMetricType::New();
	RegistrationType::Pointer registrationMI = RegistrationType::New();

	using MetricType = itk::MeanSquaresImageToImageMetricv4<ImageType, ImageType>;

	MultiResRegistration() {
		Init();
	};

	void SetMoving(InputImageType* image) {
		movingImageConv->SetInput(image);
		movingImageConv->Update();
	}

	void SetFixed(InputImageType* image) {
		fixedImageConv->SetInput(image);
		fixedImageConv->Update();
	}

	CompositeTransformType::ConstPointer Register() {
		registrationCC->Update();
		auto afterCC = registrationCC->GetModifiableTransform();

		CompositeTransformType::Pointer transformCC = CompositeTransformType::New();
		transformCC->AddTransform(registrationMI->GetModifiableTransform());
		transformCC->AddTransform(afterCC);

		registrationMI->SetMovingInitialTransform(transformCC);
		registrationMI->Update();
		
		CompositeTransformType::Pointer transform = CompositeTransformType::New();
		transform->AddTransform(registrationMI->GetModifiableTransform());
		transform->AddTransform(movingTransform);
		return transform;
	}

	OptimizerType::Pointer GetOptimizerCC() {
		return optimizerCC;
	}

	OptimizerType::Pointer GetOptimizerMI() {
		return optimizerMI;
	}




protected:
	void Init() {
		movingImageConv->SetOutputMaximum(10);
		movingImageConv->SetOutputMinimum(0);
		fixedImageConv->SetOutputMaximum(10);
		fixedImageConv->SetOutputMinimum(0);

		InitPipelineLayout();
		SetCCRegistrationParameters();
		SetMIRegistrationParameters();
	}

private:
	void InitPipelineLayout() {
		registrationCC->SetFixedImage(fixedImageConv->GetOutput());
		registrationMI->SetFixedImage(fixedImageConv->GetOutput());
		registrationCC->SetMovingImage(movingImageConv->GetOutput());
		registrationMI->SetMovingImage(movingImageConv->GetOutput());

		registrationCC->SetOptimizer(optimizerCC);
		registrationMI->SetOptimizer(optimizerMI);

		registrationCC->SetMetric(metricCC);
		registrationCC->SetMetric(metricMI);

		//set identity transform
		TransformType::ParametersType initialParameters(movingTransform->GetNumberOfParameters());
		initialParameters[0] = 0.0;
		initialParameters[1] = 0.0;
		initialParameters[2] = 0.0;
		movingTransform->SetParameters(initialParameters);
		registrationCC->SetMovingInitialTransform(movingTransform);
	}

	void SetCCRegistrationParameters() {
		constexpr unsigned int numberOfLevels = 2;
		RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
		shrinkFactorsPerLevel.SetSize(numberOfLevels);
		shrinkFactorsPerLevel[0] = 4;
		shrinkFactorsPerLevel[1] = 2;
		
		RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
		smoothingSigmasPerLevel.SetSize(numberOfLevels);
		smoothingSigmasPerLevel[0] = 2;
		smoothingSigmasPerLevel[1] = 1;

		registrationCC->SetNumberOfLevels(numberOfLevels);
		registrationCC->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
		registrationCC->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

		optimizerCC->SetNumberOfIterations(50);
		optimizerCC->SetLearningRate(1);
		optimizerCC->SetMinimumStepLength(0.001);
		optimizerCC->SetRelaxationFactor(0.5);
		optimizerCC->SetMinimumConvergenceValue(1e-6);
	}

	void SetMIRegistrationParameters() {
		constexpr unsigned int numberOfLevels = 1;
		RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
		shrinkFactorsPerLevel.SetSize(numberOfLevels);
		shrinkFactorsPerLevel[0] = 1;

		RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
		smoothingSigmasPerLevel.SetSize(numberOfLevels);
		smoothingSigmasPerLevel[0] = 0;

		registrationMI->SetNumberOfLevels(numberOfLevels);
		registrationMI->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
		registrationMI->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

		optimizerMI->SetNumberOfIterations(30);
		optimizerMI->SetLearningRate(1);
		optimizerMI->SetMinimumStepLength(0.001);
		optimizerMI->SetRelaxationFactor(0.5);
		optimizerMI->SetMinimumConvergenceValue(1e-6);
	}
};