#include "Registration.h"

void Registration::Init()
{
	registration->SetOptimizer(optimizer);
	registration->SetMetric(metric);

	TransformType::ParametersType initialParameters(movingTransform->GetNumberOfParameters());
	initialParameters[0] = 0.0;
	initialParameters[1] = 0.0;
	initialParameters[2] = 0.0;
	movingTransform->SetParameters(initialParameters);
	registration->SetMovingInitialTransform(movingTransform);

	converterMoving->SetOutputMaximum(10);
	converterMoving->SetOutputMinimum(0);
	converterFixed->SetOutputMaximum(10);
	converterFixed->SetOutputMinimum(0);
	
	registration->SetMovingImage(converterMoving->GetOutput());

	TransformType::Pointer identity = TransformType::New();
	identity->SetIdentity();
	registration->SetFixedInitialTransform(identity);
	registration->SetFixedImage(converterFixed->GetOutput());

	constexpr unsigned int numberOfLevels = 1; // setup for simple one-level registration
	RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	shrinkFactorsPerLevel.SetSize(1);
	shrinkFactorsPerLevel[0] = 4;
	RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	smoothingSigmasPerLevel.SetSize(1);
	smoothingSigmasPerLevel[0] = 0;
	registration->SetNumberOfLevels(numberOfLevels);
	registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

	using InterpolatorType = itk::LinearInterpolateImageFunction<ImageType, double>;

	metric->SetFixedInterpolator(InterpolatorType::New());
	metric->SetMovingInterpolator(InterpolatorType::New());


	optimizer->SetNumberOfIterations(20);
	optimizer->SetLearningRate(4);
	optimizer->SetMinimumStepLength(0.001);
	optimizer->SetRelaxationFactor(0.5);
	optimizer->SetMinimumConvergenceValue(1e-6);
}