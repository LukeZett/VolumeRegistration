#include "Registration.h"

void Registration::Init()
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