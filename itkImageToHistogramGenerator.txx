/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToHistogramGenerator.txx,v $
  Language:  C++
  Date:      $Date: 2004/02/25 16:10:47 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageToHistogramGenerator_txx
#define _itkImageToHistogramGenerator_txx

#include "itkImageToHistogramGenerator.h"


namespace itk { 
namespace Statistics {


template < class TImage, class TMaskImage >
ImageToHistogramGenerator< TImage, TMaskImage >
::ImageToHistogramGenerator() 
{
  m_ImageToListAdaptor = AdaptorType::New();
  m_ListGenerator = ListGeneratorType::New();
  m_HistogramGenerator = GeneratorType::New();
  m_HistogramGenerator->SetListSample( m_ImageToListAdaptor );
  m_HistogramGenerator2 = GeneratorType2::New();
  m_HistogramGenerator2->SetListSample( m_ListGenerator->GetListSample() );
  m_MaskValue = NumericTraits<MaskPixelType>::max();

  // set a usable default value
  SizeType size;
  size.Fill( 128 );
  this->SetNumberOfBins( size );

}



template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetInput( const ImageType * image ) 
{
  m_ImageToListAdaptor->SetImage( image );
  m_ListGenerator->SetInput( image );
}


template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetMaskImage( const MaskImageType * image ) 
{
  m_ListGenerator->SetMaskImage( image );
}


template < class TImage, class TMaskImage >
const typename ImageToHistogramGenerator< TImage, TMaskImage >::HistogramType *
ImageToHistogramGenerator< TImage, TMaskImage >
::GetOutput() const
{
  if( m_ListGenerator->GetMaskImage() )
    {
    return m_HistogramGenerator2->GetOutput();
    }
  else
    {
    return m_HistogramGenerator->GetOutput();
    }
}



template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::Compute() 
{
  if( m_ListGenerator->GetMaskImage() )
    {
    m_ListGenerator->SetMaskValue( m_MaskValue );
    m_ListGenerator->Update();
    m_HistogramGenerator2->Update();
    // m_HistogramGenerator->SetListSample( m_ListGenerator->GetListSample() );
    }
  else
    {
    // m_HistogramGenerator->SetListSample( m_ImageToListAdaptor );
    m_HistogramGenerator->Update();
    }
  // m_HistogramGenerator->Update();
}



template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetNumberOfBins( const SizeType & size ) 
{
  m_HistogramGenerator->SetNumberOfBins( size );
  m_HistogramGenerator2->SetNumberOfBins( size );
}



template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetMarginalScale( double marginalScale )
{
  m_HistogramGenerator->SetMarginalScale( marginalScale );
  m_HistogramGenerator2->SetMarginalScale( marginalScale );
}


template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetHistogramMin(const MeasurementVectorType & histogramMin)
{
  m_HistogramGenerator->SetHistogramMin(histogramMin);
  m_HistogramGenerator2->SetHistogramMin(histogramMin);
}


template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetHistogramMax(const MeasurementVectorType & histogramMax)
{
  m_HistogramGenerator->SetHistogramMax(histogramMax);
  m_HistogramGenerator2->SetHistogramMax(histogramMax);
}


template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::SetAutoMinMax(bool autoMinMax)
{
  m_HistogramGenerator->SetAutoMinMax(autoMinMax);
  m_HistogramGenerator2->SetAutoMinMax(autoMinMax);
}


template < class TImage, class TMaskImage >
void
ImageToHistogramGenerator< TImage, TMaskImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << "ImageToListSample adaptor = " << m_ImageToListAdaptor << std::endl;
  os << "HistogramGenerator = " << m_HistogramGenerator << std::endl;
}



} // end of namespace Statistics 
} // end of namespace itk

#endif


