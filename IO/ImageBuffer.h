#pragma once
#include "itkImage.h"
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"

namespace DCR {
	template<typename InPixelType, int dimension, typename OutPixelType>
	class ImageBuffer
	{
		using RescalerType = itk::RescaleIntensityImageFilter<itk::Image<InPixelType, dimension>, itk::Image<OutPixelType, dimension>>;
	public:
		ImageBuffer(itk::Image<InPixelType, dimension>* image);

		OutPixelType* GetData() { return m_buffer; }

		~ImageBuffer();

		const itk::Size<dimension>& GetSize() { return size; };

	private:

		OutPixelType* m_buffer = nullptr;
		itk::Size<dimension> size;

		RescalerType::Pointer rescaler = RescalerType::New();

	};


	template<typename InPixelType, int dimension, typename OutPixelType>
	inline ImageBuffer<InPixelType, dimension, OutPixelType>::ImageBuffer(itk::Image<InPixelType, dimension>* image)
	{
		rescaler->SetInput(image);
		rescaler->SetOutputMinimum(std::numeric_limits<OutPixelType>::min());
		rescaler->SetOutputMaximum(std::numeric_limits<OutPixelType>::max());
		rescaler->Update();

		using IteratorType = itk::ImageRegionIterator<itk::Image<OutPixelType, dimension>>;

		auto& region = rescaler->GetOutput()->GetLargestPossibleRegion();
		size = region.GetSize();

		size_t bufferSize = 1;
		for (size_t i = 0; i < dimension; i++)
		{
			bufferSize *= size[i];
		}
		m_buffer = new OutPixelType[bufferSize];

		IteratorType iter(rescaler->GetOutput(), region);
		iter.GoToBegin();

		size_t buffIter = 0;

		while (!iter.IsAtEnd())
		{
			m_buffer[buffIter] = iter.Get();
			++iter;
			++buffIter;
		}

	}

	template<typename PixelType, int dimension, typename OutPixelType>
	inline ImageBuffer<PixelType, dimension, OutPixelType>::~ImageBuffer()
	{
		delete m_buffer;
	}
}