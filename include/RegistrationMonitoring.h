#pragma once

#include "itkCommand.h"
#include "itkMacro.h"
#include "Logging.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"
#include <string>
	
class RegistrationMonitoring : public itk::Command
{
public:
	using Self = RegistrationMonitoring;
	using Parent = itk::Command;
	using itkPointer = itk::SmartPointer<Self>;

	using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;

	itkNewMacro(Self);

	void Execute(itk::Object* caller, const itk::EventObject& event) override
	{
		Execute((const itk::Object*)caller, event);
	}

	void Execute(const itk::Object* caller, const itk::EventObject& event) override 
	{
		auto optimizer = static_cast<const OptimizerType*>(caller);

		if (!itk::IterationEvent().CheckEvent(&event))
		{
			return;
		}
		LOG_INFO("Iteration: " + std::to_string(optimizer->GetCurrentIteration()));
		LOG_INFO("Cost: " + std::to_string(optimizer->GetValue()));
		std::cout << optimizer->GetCurrentPosition() << std::endl;
	}

protected:
	RegistrationMonitoring() = default;

};