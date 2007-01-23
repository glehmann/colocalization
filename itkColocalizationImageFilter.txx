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
  m_NumberOfBins.Fill( 128 );
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
  calculator->Update();
  m_Pearson = calculator->GetPearson();
  m_Overlap1 = calculator->GetOverlap1();
  m_Overlap2 = calculator->GetOverlap2();
  m_Overlap = calculator->GetOverlap();

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
  for( unsigned int i=0; i< ImageDimension; i++)
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

  os << indent << "MaskValue: " << static_cast<typename NumericTraits<MaskPixelType>::PrintType>(m_MaskValue) << std::endl;
}


}// end namespace itk
#endif
