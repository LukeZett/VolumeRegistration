#include "itkCommand.h"

template<typename TRegistration>
class RegistrationInterfaceCommand : public itk::Command
{
public:
	using Self = RegistrationInterfaceCommand;
	using Pointer = itk::SmartPointer<Self>;
	using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
	using OptimizerPointer = OptimizerType*;
	itkNewMacro(Self);


protected:
	RegistrationInterfaceCommand() = default;
};