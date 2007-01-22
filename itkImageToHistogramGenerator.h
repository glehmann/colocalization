/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageToHistogramGenerator.h,v $
  Language:  C++
  Date:      $Date: 2005/08/24 15:16:46 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToHistogramGenerator_h
#define __itkImageToHistogramGenerator_h


#include "itkImageToListAdaptor.h"
#include "itkImageToListGenerator.h"
#include "itkListSampleToHistogramGenerator.h"
#include "itkObject.h"


namespace itk {
namespace Statistics {

template <typename TAdaptor >
struct GetAdaptorMeasurementVectorLength
{
  itkStaticConstMacro( MeasurementVectorLength, unsigned int, TAdaptor::MeasurementVectorSize );
};

/** \class ImageToHistogramGenerator
 *  \brief This class generates an histogram from an image.
 *
 *  The concept of Histogram in ITK is quite generic. It has been designed to
 *  manage multiple components data. This class facilitates the computation of
 *  an histogram from an image. Internally it creates a List that is feed into
 *  the ListSampleToHistogramGenerator.
 *
 */
  
 
template< class TImageType, class TMaskImage = Image< unsigned char, TImageType::ImageDimension > >
class ImageToHistogramGenerator : public Object
{
public:
  /** Standard typedefs */
  typedef ImageToHistogramGenerator  Self ;
  typedef Object Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToHistogramGenerator, Object) ;

  /** standard New() method support */
  itkNewMacro(Self) ;

  typedef TImageType                                      ImageType;
  typedef TMaskImage                                      MaskImageType;
  typedef ImageToListAdaptor< ImageType >                 AdaptorType;
  typedef typename AdaptorType::Pointer                   AdaptorPointer;
  typedef typename ImageType::PixelType                   PixelType;
  typedef typename PixelType::ValueType                   ValueType;
  typedef typename NumericTraits< ValueType >::RealType   ValueRealType;
  typedef DenseFrequencyContainer                         FrequencyContainerType;
  typedef typename MaskImageType::PixelType    MaskPixelType ;

  typedef ImageToListGenerator< ImageType, MaskImageType > ListGeneratorType;
  typedef typename ListGeneratorType::Pointer ListGeneratorPointer;

  typedef ListSampleToHistogramGenerator< 
                         AdaptorType, 
                         ValueRealType,
                         FrequencyContainerType,
                         ::itk::Statistics::GetAdaptorMeasurementVectorLength< 
                                          AdaptorType >::MeasurementVectorLength
                           > GeneratorType;

  typedef ListSampleToHistogramGenerator< 
                         typename ListGeneratorType::ListSampleType, 
                         ValueRealType,
                         FrequencyContainerType,
                         ::itk::Statistics::GetAdaptorMeasurementVectorLength< 
                                          AdaptorType >::MeasurementVectorLength
                           > GeneratorType2;

  typedef typename GeneratorType::Pointer                   GeneratorPointer;
  typedef typename GeneratorType2::Pointer                   GeneratorPointer2;

  typedef typename GeneratorType::HistogramType             HistogramType;
  typedef typename HistogramType::Pointer                   HistogramPointer;
  typedef typename HistogramType::ConstPointer              HistogramConstPointer;
  typedef typename HistogramType::SizeType                  SizeType;
  typedef typename HistogramType::MeasurementVectorType     MeasurementVectorType;

public:

  /** Triggers the Computation of the histogram */
  void Compute( void );

  /** Connects the input image for which the histogram is going to be computed */
  void SetInput( const ImageType * );
  
  /** Return the histogram. o
   \warning This output is only valid after the Compute() method has been invoked 
   \sa Compute */
  const HistogramType * GetOutput() const;
  
  /** Set number of histogram bins */
  void SetNumberOfBins( const SizeType & size );
 
  /** Set marginal scale value to be passed to the histogram generator */
  void SetMarginalScale( double marginalScale );
  void SetHistogramMin(const MeasurementVectorType & histogramMin);
  void SetHistogramMax(const MeasurementVectorType & histogramMax);
  void SetAutoMinMax(bool autoMinMax);

  /** Connects the input image for which the histogram is going to be computed */
  void SetMaskImage( const MaskImageType * );

  /** Set the pixel value treated as on in the mask. If a mask has been 
   * specified, only pixels with this value will be added to the list sample, if
   * no mask has been specified all pixels will be added as measurement vectors
   * to the list sample. */
  itkSetMacro( MaskValue, MaskPixelType );
  itkGetMacro( MaskValue, MaskPixelType );
  
protected:
  ImageToHistogramGenerator();
  virtual ~ImageToHistogramGenerator() {};
  void PrintSelf(std::ostream& os, Indent indent) const;


private:

  AdaptorPointer      m_ImageToListAdaptor;
  ListGeneratorPointer m_ListGenerator;

  GeneratorPointer    m_HistogramGenerator;
  GeneratorPointer2    m_HistogramGenerator2;

  MaskPixelType m_MaskValue;
};


} // end of namespace Statistics 
} // end of namespace itk 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToHistogramGenerator.txx"
#endif

#endif
