#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkVector.h"
#include "itkImageToHistogramGenerator.h"
#include "itkImageToHistogramGenerator.h"
#include "itkCompose2DVectorImageFilter.h"
#include <itkHistogramToLogProbabilityImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include "itkColocalizationCalculator.h"

int main(int argc, char * argv[])
{

  if( argc != 4 )
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::Vector< unsigned char, 2 > VType;
  typedef itk::Image< VType, dim > VIType;

  typedef itk::ImageFileReader< IType > ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName( argv[1] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );

  typedef itk::Compose2DVectorImageFilter< IType, VIType > ComposeType;
  ComposeType::Pointer compose = ComposeType::New();
  compose->SetInput1( reader1->GetOutput() );
  compose->SetInput2( reader2->GetOutput() );
  compose->Update();

  typedef itk::Statistics::ImageToHistogramGenerator< VIType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( compose->GetOutput() );
/*  FilterType::SizeType s;
  s.Fill( 256 );
  filter->SetNumberOfBins( s );*/
  filter->Compute();

  typedef itk::ColocalizationCalculator< FilterType::HistogramType > ColocType;
  ColocType::Pointer coloc = ColocType::New();
  coloc->SetInputHistogram( filter->GetOutput() );
  coloc->Update();
  std::cout << "Pearson: " << coloc->GetPearson() << std::endl;
  std::cout << "Overlap: " << coloc->GetOverlap() << std::endl;
  std::cout << "Overlap1: " << coloc->GetOverlap1() << std::endl;
  std::cout << "Overlap2: " << coloc->GetOverlap2() << std::endl;

  typedef itk::HistogramToLogProbabilityImageFilter< FilterType::HistogramType > LogType;
  LogType::Pointer log = LogType::New();
  log->SetInput( filter->GetOutput() );
  
  typedef itk::RescaleIntensityImageFilter< LogType::OutputImageType, IType> RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( log->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  

  return 0;
}

