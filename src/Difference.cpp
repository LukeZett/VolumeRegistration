#include "Difference.h"

void Difference::SetFixed(ImageType* image) {
	resampler->SetSize(image->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(image->GetOrigin());
	resampler->SetOutputSpacing(image->GetSpacing());
	resampler->SetOutputDirection(image->GetDirection());
	resampler->SetDefaultPixelValue(0x0);

	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	resampler->SetTransform(identityTransform);

	diff->SetInput1(image);
}