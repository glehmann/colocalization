/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkColocalizationCalculator.h,v $
  Language:  C++
  Date:      $Date: 2005/11/25 15:50:35 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkColocalizationCalculator_h
#define __itkColocalizationCalculator_h

#include "itkHistogramAlgorithmBase.h"
#include "itkHistogram.h"

namespace itk
{

/** \class ColocalizationCalculator
 * \brief Computes colocalization coefficients
 * 
 * \ingroup Calculators
 */

template< class TInputHistogram >
class ColocalizationCalculator :
      public HistogramAlgorithmBase< TInputHistogram >
{
public:
  /**Standard class typedefs. */
  typedef ColocalizationCalculator Self;
  typedef HistogramAlgorithmBase<TInputHistogram> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  typedef TInputHistogram HistogramType;
  typedef typename TInputHistogram::MeasurementType MeasurementType;
  typedef typename TInputHistogram::MeasurementVectorType MeasurementVectorType;
  typedef typename TInputHistogram::FrequencyType FrequencyType;

  typedef typename NumericTraits<MeasurementType>::RealType MeanType;
  typedef typename NumericTraits<MeasurementType>::RealType VarianceType;

  typedef std::vector<MeanType> MeanVectorType;
  typedef std::vector<FrequencyType> FrequencyVectorType;

  typedef typename TInputHistogram::InstanceIdentifier InstanceIdentifierType;
  typedef std::vector<InstanceIdentifierType> InstanceIdentifierVectorType;

  /**Standard Macros */
  itkTypeMacro(ColocalizationCalculator, HistogramAlgorithmsBase);
  itkNewMacro(Self) ;

  itkSetMacro(Threshold, MeasurementVectorType);
  itkGetConstMacro(Threshold, MeasurementVectorType);

  itkGetConstMacro(Pearson, MeasurementType);
  itkGetConstMacro(Overlap1, MeasurementType);
  itkGetConstMacro(Overlap2, MeasurementType);
  itkGetConstMacro(Overlap, MeasurementType);
  itkGetConstMacro(Contribution1, MeasurementType);
  itkGetConstMacro(Contribution2, MeasurementType);

  MeasurementType Mean( unsigned int dim ) const;

protected:
  ColocalizationCalculator();
  virtual ~ColocalizationCalculator() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Calculates the thresholds and save them */
  void GenerateData() ;

private:
  /** Internal thresholds storage */
  MeasurementVectorType m_Threshold ;

  MeasurementType m_Pearson;
  MeasurementType m_Overlap1;
  MeasurementType m_Overlap2;
  MeasurementType m_Overlap;
  MeasurementType m_Contribution1;
  MeasurementType m_Contribution2;

} ; // end of class

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkColocalizationCalculator.txx"
#endif

#endif
