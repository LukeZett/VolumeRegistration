#pragma once
#include "itkImage.h"
#include "itkResampleImageFilter.h"
#include "itkImportImageFilter.h"
#include "itkTranslationTransform.h"


class SphereImage {

	using Pixel = int16_t;
	using ImageType = itk::Image<Pixel, 3>;
	using ImportFilterType = itk::ImportImageFilter<Pixel, 3>;
	using ResamplerType = itk::ResampleImageFilter<ImageType, ImageType>;

	ImportFilterType::Pointer m_importFilter = ImportFilterType::New();
	ResamplerType::Pointer m_resampler = ResamplerType::New();



public:
	ImageType::Pointer SetTransform(const itk::Transform<double, 3, 3>::Pointer transform) {
		m_resampler->SetTransform(transform);
		return m_resampler->GetOutput();
	}

	SphereImage(uint64_t xSize, uint64_t ySize, uint64_t zSize, const Pixel& minVal, const Pixel& maxVal) {
		Pixel* buffer = new Pixel[xSize * ySize * zSize];
		memset(buffer, 0, xSize * ySize * zSize * sizeof(Pixel));

		Pixel* it = buffer;

		for (unsigned int z = 0; z < zSize; z++)
		{
			const double dz =
				static_cast<double>(z) - static_cast<double>(zSize) / 2.0;
			for (unsigned int y = 0; y < ySize; y++)
			{
				const double dy =
					static_cast<double>(y) - static_cast<double>(ySize) / 2.0;
				for (unsigned int x = 0; x < xSize; x++)
				{
					const double dx =
						static_cast<double>(x) - static_cast<double>(xSize) / 2.0;
					const double d2 = dx * dx + dy * dy + dz * dz;
					*it++ = (d2 < 10 * 10) ? maxVal : minVal;
				}
			}
		}

		ImportFilterType::IndexType start;
		start.Fill(0);

		ImportFilterType::SizeType size;
		size[0] = xSize;
		size[1] = ySize;
		size[2] = zSize;

		ImportFilterType::RegionType region;
		region.SetSize(size);
		region.SetIndex(start);

		m_importFilter->SetRegion(region);

		const itk::SpacePrecisionType origin[3] = { 0.0, 0.0, 0.0 };
		m_importFilter->SetOrigin(origin);

		const itk::SpacePrecisionType spacing[3] = { 1.0, 1.0, 1.0 };
		m_importFilter->SetSpacing(spacing);
		m_importFilter->SetImportPointer(buffer, xSize * ySize * zSize, true);

		m_importFilter->UpdateOutputInformation();
		m_resampler->SetInput(m_importFilter->GetOutput());
		auto image = m_importFilter->GetOutput();

		m_resampler->SetSize(image->GetLargestPossibleRegion().GetSize());
		m_resampler->SetOutputOrigin(image->GetOrigin());
		m_resampler->SetOutputSpacing(image->GetSpacing());
		m_resampler->SetOutputDirection(image->GetDirection());
		m_resampler->SetDefaultPixelValue(0x0);

	}

	ImageType::Pointer GetOutput() {
		
		m_resampler->UpdateOutputInformation();

		return m_resampler->GetOutput();
	}

	

};