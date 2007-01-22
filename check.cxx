#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkVector.h"
#include "itkImageToHistogramGenerator.h"
#include "itkCompose2DVectorImageFilter.h"
#include <itkHistogramToLogProbabilityImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include "itkColocalizationImageFilter.h"

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

  typedef itk::ImageFileReader< IType > ReaderType;

  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName( argv[1] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );

  typedef itk::ColocalizationImageFilter< IType > ColocType;
  ColocType::Pointer coloc = ColocType::New();
  coloc->SetInput( 0, reader1->GetOutput() );
  coloc->SetInput( 1, reader2->GetOutput() );
  coloc->Update();
  std::cout << "Pearson: " << coloc->GetPearson() << std::endl;
  std::cout << "Overlap: " << coloc->GetOverlap() << std::endl;
  std::cout << "Overlap1: " << coloc->GetOverlap1() << std::endl;
  std::cout << "Overlap2: " << coloc->GetOverlap2() << std::endl;

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( coloc->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  

  return 0;
}

