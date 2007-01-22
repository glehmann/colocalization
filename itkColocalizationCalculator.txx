/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkColocalizationCalculator.txx,v $
  Language:  C++
  Date:      $Date: 2005/01/13 15:36:46 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkColocalizationCalculator_txx
#define _itkColocalizationCalculator_txx

#include "itkColocalizationCalculator.h"

namespace itk
{


template<class TInputHistogram>
ColocalizationCalculator<TInputHistogram>
::ColocalizationCalculator()
{
  m_Threshold.Fill( NumericTraits< MeasurementType >::Zero );
}


template<class TInputHistogram>
typename ColocalizationCalculator<TInputHistogram>::MeasurementType
ColocalizationCalculator<TInputHistogram>
::Mean( unsigned int dim ) const
{
  typename TInputHistogram::ConstPointer histogram = this->GetInputHistogram();
  MeasurementType mean = NumericTraits<MeasurementType>::Zero;

  for (unsigned int i = 0; i < histogram->GetSize( dim ); i++)
    {
    MeasurementType val = histogram->GetMeasurement( i, dim );
    FrequencyType freq = histogram->GetFrequency( i, dim );
    mean += val * freq;
    }

  mean /= histogram->GetTotalFrequency();

  return mean;
}


template<class TInputHistogram>
void
ColocalizationCalculator<TInputHistogram>
::GenerateData()
{

  typename TInputHistogram::ConstPointer histogram = this->GetInputHistogram();

  if (histogram->GetSize().GetSizeDimension() != 2)
    {
    itkExceptionMacro(<<"Histogram must be 2-dimensional.");
    }

  MeasurementType mean0 = Mean( 0 );
  MeasurementType mean1 = Mean( 1 );

  MeasurementType pearsonNum = 0;
  MeasurementType pearsonDen0 = 0;
  MeasurementType pearsonDen1 = 0;

  MeasurementType s0s1 = 0;
  MeasurementType s0_2 = 0;
  MeasurementType s1_2 = 0;

  for (unsigned int i = 0; i < histogram->GetSize( 0 ); i++)
    {

    const MeasurementType & s0 = histogram->GetMeasurement( i, 0 );
    MeasurementType dev0 = s0 - mean0;

    for (unsigned int j = 0; j < histogram->GetSize( 1 ); j++)
      {
      const MeasurementType & s1 = histogram->GetMeasurement( j, 1 );

      typename HistogramType::IndexType index;
      index[0] = i;
      index[1] = j;
      const FrequencyType & freq = histogram->GetFrequency( index );

      MeasurementType dev1 = s1 - mean1;

      pearsonNum += freq * dev0 * dev1;
      pearsonDen0 += freq * vcl_pow( dev0, 2 );
      pearsonDen1 += freq * vcl_pow( dev1, 2 );

      s0s1 += freq * s0 * s1;
      s0_2 += freq * vcl_pow( s0, 2 );
      s1_2 += freq * vcl_pow( s1, 2 );
      }
    }

  m_Pearson = pearsonNum / vcl_sqrt( pearsonDen0 * pearsonDen1 );

  m_Overlap1 = s0s1 / s0_2;
  m_Overlap2 = s0s1 / s1_2;
  m_Overlap = s0s1 / vcl_sqrt( s0_2 * s1_2 );
  // overlap can also be computed that way:
  //  std::cout << "Overlap': " << vcl_sqrt( m_Overlap1 * m_Overlap2 ) <<std::endl;

}

template<class TInputHistogram>
void
ColocalizationCalculator<TInputHistogram>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Pearson: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Pearson) << std::endl;
  os << indent << "Overlap1: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap1) << std::endl;
  os << indent << "Overlap2: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap2) << std::endl;
  os << indent << "Overlap: " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_Overlap) << std::endl;
//   os << indent << ": " << static_cast<typename NumericTraits<MeasurementType>::PrintType>(m_) << std::endl;

}

} // end namespace itk

#endif
