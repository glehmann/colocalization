/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkColocalizationImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/03/15 01:57:09 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkColocalizationImageFilter_txx
#define _itkColocalizationImageFilter_txx

#include "itkColocalizationImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template<class TInputImage, class TMaskImage, class TOutputImage>
ColocalizationImageFilter<TInputImage, TMaskImage, TOutputImage>
::ColocalizationImageFilter()
{
  m_MaskValue = NumericTraits<MaskPixelType>::max();
  m_Pearson = 0;
  m_Slope = 0;
  m_Intercept = 0;
  m_Overlap1 = 0;
  m_Overlap2 = 0;
  m_Overlap = 0;
  m_ColocalizedPearson = 0;
  m_ColocalizedSlope = 0;
  m_ColocalizedIntercept = 0;
  m_ColocalizedOverlap1 = 0;
  m_ColocalizedOverlap2 = 0;
  m_ColocalizedOverlap = 0;
  m_Contribution1 = 0;
  m_Contribution2 = 0;
  m_NumberOfBins.Fill( 128 );
  m_Threshold.Fill( NumericTraits< MeasurementType >::Zero );
  m_ComputeThreshold = true;
  this->SetNumberOfRequiredInputs( 2 );
}

template<class TInputImage, class TMaskImage, class TOutputImage>
void
ColocalizationImageFilter<TInputImage, TMaskImage, TOutputImage>
::GenerateData()
{
  typename ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // create the image with the 2 channels
  typename ComposeType::Pointer compose = ComposeType::New();
  compose->SetInput1( this->GetInput( 0 ) );
  compose->SetInput2( this->GetInput( 1 ) );
  progress->RegisterInternalFilter( compose, .4f );
  compose->Update();

  // Create a histogram of the image intensities
  typename HistogramGeneratorType::Pointer histogramGenerator = HistogramGeneratorType::New();
  histogramGenerator->SetInput(  compose->GetOutput()  );
  histogramGenerator->SetMaskImage( this->GetMaskImage()  );
  histogramGenerator->SetMaskValue( m_MaskValue );
  histogramGenerator->SetNumberOfBins( m_NumberOfBins );
  // progress->RegisterInternalFilter(histogramGenerator,.5f);
  histogramGenerator->Compute();

  // Compute the colocalization values for the input image
  typename CalculatorType::Pointer calculator = CalculatorType::New();
  calculator->SetInputHistogram( histogramGenerator->GetOutput() );
  calculator->SetComputeThreshold( m_ComputeThreshold );
  calculator->SetThreshold( m_Threshold );
  calculator->Update();
  m_Threshold = calculator->GetThreshold();
  m_Pearson = calculator->GetPearson();
  m_Slope = calculator->GetSlope();
  m_Intercept = calculator->GetIntercept();
  m_Overlap1 = calculator->GetOverlap1();
  m_Overlap2 = calculator->GetOverlap2();
  m_Overlap = calculator->GetOverlap();
  m_ColocalizedPearson = calculator->GetColocalizedPearson();
  m_ColocalizedSlope = calculator->GetColocalizedSlope();
  m_ColocalizedIntercept = calculator->GetColocalizedIntercept();
  m_ColocalizedOverlap1 = calculator->GetColocalizedOverlap1();
  m_ColocalizedOverlap2 = calculator->GetColocalizedOverlap2();
  m_ColocalizedOverlap = calculator->GetColocalizedOverlap();
  m_Contribution1 = calculator->GetContribution1();
  m_Contribution2 = calculator->GetContribution2();

  typename LogType::Pointer log = LogType::New();
  log->SetInput( histogramGenerator->GetOutput() );
  log->Update(); // fixed in itk 3.2, but required before that
  progress->RegisterInternalFilter( log, .3f );
  
  typename RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( log->GetOutput() );
  progress->RegisterInternalFilter( rescale, .3f );

  rescale->GraftOutput (this->GetOutput());
  rescale->Update();
  this->GraftOutput(rescale->GetOutput());
}


template<class TInputImage, class TMaskImage, class TOutputImage>
void
ColocalizationImageFilter<TInputImage, TMaskImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  const_cast<TInputImage *>(this->GetInput())->SetRequestedRegionToLargestPossibleRegion();
}


template<class TInputImage, class TMaskImage, class TOutputImage>
void 
ColocalizationImageFilter<TInputImage, TMaskImage, TOutputImage>
::GenerateOutputInformation()
{
  OutputImageType     *outputImage    = this->GetOutput();

  // Set the image size to the number of bins along each dimension.
  typename OutputImageType::SizeType size;
  for( unsigned int i=0; i< OutputImageDimension; i++)
    {
    size[i] = m_NumberOfBins[i];
    }

  // Set output image params and Allocate image
  typename OutputImageType::RegionType region;
  region.SetSize( size );
  outputImage->SetRegions( region );
}


template<class TInputImage, class TMaskImage, class TOutputImage>
void 
ColocalizationImageFilter<TInputImage, TMaskImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Threshold: " << m_Threshold << std::endl;
  os << indent << "ComputeThreshold: " << m_ComputeThreshold << std::endl;
  os << indent << "MaskValue: " << static_cast<typename NumericTraits<MaskPixelType>::PrintType>(m_MaskValue) << std::endl;
  os << indent << "Pearson: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Pearson) << std::endl;
  os << indent << "Slope: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Slope) << std::endl;
  os << indent << "Intercept: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Intercept) << std::endl;
  os << indent << "Overlap: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap) << std::endl;
  os << indent << "Overlap1: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap1) << std::endl;
  os << indent << "Overlap2: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap2) << std::endl;
  os << indent << "ColocalizedPearson: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedPearson) << std::endl;
  os << indent << "ColocalizedSlope: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedSlope) << std::endl;
  os << indent << "ColocalizedIntercept: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedIntercept) << std::endl;
  os << indent << "ColocalizedOverlap: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedOverlap) << std::endl;
  os << indent << "ColocalizedOverlap1: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedOverlap1) << std::endl;
  os << indent << "ColocalizedOverlap2: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_ColocalizedOverlap2) << std::endl;
  os << indent << "Contribution1: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Contribution1) << std::endl;
  os << indent << "Contribution2: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Contribution2) << std::endl;
}

}// end namespace itk
#endif
