/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkColocalizationImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/04/05 13:59:37 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkColocalizationImageFilter_h
#define __itkColocalizationImageFilter_h

#include "itkVector.h"
#include "itkCompose2DVectorImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkColocalizationCalculator.h"
#include "itkImageToHistogramGenerator.h"
#include "itkImageToHistogramGenerator.h"
#include "itkHistogramToLogProbabilityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace itk {

/** \class ColocalizationImageFilter 
 *
 */

template<class TInputImage, class TMaskImage=Image<unsigned char, TInputImage::ImageDimension>, class TOutputImage=TInputImage>
class ITK_EXPORT ColocalizationImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard Self typedef */
  typedef ColocalizationImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Image related typedefs. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension ) ;
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension ) ;
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension ) ;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ColocalizationImageFilter, ImageToImageFilter);
  
  /** Standard image type within this class. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TMaskImage MaskImageType;

  /** Image pixel value typedef. */
  typedef typename TInputImage::PixelType   InputPixelType;
  typedef typename TOutputImage::PixelType   OutputPixelType;
  typedef typename TMaskImage::PixelType   MaskPixelType;
  
  /** Image related typedefs. */
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TOutputImage::Pointer OutputImagePointer;
  typedef typename TMaskImage::Pointer MaskImagePointer;

  typedef typename TInputImage::SizeType  InputSizeType;
  typedef typename TInputImage::IndexType  InputIndexType;
  typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TOutputImage::SizeType  OutputSizeType;
  typedef typename TOutputImage::IndexType  OutputIndexType;
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  typedef itk::Vector< unsigned char, 2 > VectorType;
  typedef itk::Image< VectorType, ImageDimension > VectorImageType;
  typedef itk::Compose2DVectorImageFilter< InputImageType, VectorImageType > ComposeType;
  typedef itk::Statistics::ImageToHistogramGenerator< VectorImageType, MaskImageType > HistogramGeneratorType;
  typedef typename HistogramGeneratorType::HistogramType HistogramType;
  typedef ColocalizationCalculator< HistogramType > CalculatorType;
  typedef itk::HistogramToLogProbabilityImageFilter< HistogramType > LogType;
  typedef itk::RescaleIntensityImageFilter< typename LogType::OutputImageType, OutputImageType > RescaleType;

  typedef typename HistogramType::MeasurementType MeasurementType;
  typedef typename HistogramType::SizeType HistogramSizeType;

  itkSetMacro(MaskValue, MaskPixelType);
  itkGetMacro(MaskValue, MaskPixelType);

  /** Set/Get the number of histogram bins. Default is 128. */
  itkSetMacro( NumberOfBins, HistogramSizeType );
  itkGetConstMacro( NumberOfBins, HistogramSizeType );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputComparableCheck,
    (Concept::Comparable<OutputPixelType>));
  itkConceptMacro(OutputOStreamWritableCheck,
    (Concept::OStreamWritable<OutputPixelType>));
  /** End concept checking */
#endif

   /** Set the mask image */
  void SetMaskImage(MaskImageType *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 2, const_cast<MaskImageType *>(input) );
     }

  /** Get the mask image */
  MaskImageType * GetMaskImage()
    {
    return static_cast<MaskImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(2)));
    }

   /** Set the mask image */
  void SetInput3(MaskImageType *input)
     {
     this->SetMaskImage( input );
     }

  itkSetMacro(Threshold, MeasurementVectorType);
  itkGetConstMacro(Threshold, MeasurementVectorType);

  itkGetConstMacro(Pearson, MeasurementType);
  itkGetConstMacro(Overlap1, MeasurementType);
  itkGetConstMacro(Overlap2, MeasurementType);
  itkGetConstMacro(Overlap, MeasurementType);
  itkGetConstMacro(Contribution1, MeasurementType);
  itkGetConstMacro(Contribution2, MeasurementType);

protected:
  ColocalizationImageFilter();
  ~ColocalizationImageFilter(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateInputRequestedRegion();
  void GenerateData ();
  virtual void GenerateOutputInformation();

private:
  ColocalizationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  MeasurementVectorType m_Threshold ;

  MaskPixelType m_MaskValue;
  HistogramSizeType m_NumberOfBins;

  MeasurementType m_Pearson;
  MeasurementType m_Overlap1;
  MeasurementType m_Overlap2;
  MeasurementType m_Overlap;
  MeasurementType m_Contribution1;
  MeasurementType m_Contribution2;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkColocalizationImageFilter.txx"
#endif

#endif
