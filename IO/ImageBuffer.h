#pragma once
#include "itkImage.h"
#include "itkImageRegionIterator.h"

template<typename PixelType, int dimension>
class ImageBuffer
{
public:
	ImageBuffer(itk::Image<PixelType, dimension> image);

	PixelType* GetData() { return m_buffer; }

	~ImageBuffer();

private:
	PixelType* m_buffer = nullptr;
};


template<typename PixelType, int dimension>
inline ImageBuffer<PixelType, dimension>::ImageBuffer(itk::Image<PixelType, dimension> image)
{
	using IteratorType = itk::ImageRegionIterator<itk::Image<PixelType, dimension>>;

	auto& region = image.GetLargestPossibleRegion();
	const itk::Size<dimension>& size = region.GetSize();

	size_t bufferSize = 1;
	for (size_t i = 0; i < dimension; i++)
	{
		bufferSize *= size[i];
	}
	m_buffer = new PixelType[bufferSize];

	IteratorType iter(&image, region);
	iter.GoToBegin();

	size_t buffIter = 0;

	while (!iter.IsAtEnd())
	{
		m_buffer[buffIter] = iter.Get();
		++iter;
		++buffIter;
	}

}

template<typename PixelType, int dimension>
inline ImageBuffer<PixelType, dimension>::~ImageBuffer()
{
	delete m_buffer;
}
