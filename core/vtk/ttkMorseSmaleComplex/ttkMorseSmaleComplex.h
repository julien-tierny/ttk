/// \ingroup vtk
/// \class ttkMorseSmaleComplex
/// \author Guillaume Favelier <guillaume.favelier@lip6.fr>
/// \author Julien Tierny <julien.tierny@lip6.fr>
/// \date February 2017.
///
/// \brief TTK VTK-filter that wraps the morseSmaleComplex processing package.
///
/// TTK module for the computation of Morse-Smale complexes.
/// Morse-Smale complexes are useful topological abstractions of scalar
/// fields for data segmentation, feature extraction, etc.
///
/// \b Related \b publication \n
/// "Parallel Computation of 3D Morse-Smale Complexes" \n
/// Nithin Shivashankar, Vijay Natarajan \n
/// Proc. of EuroVis 2012. \n
/// Computer Graphics Forum, 2012.
///
/// \param Input Input scalar field, defined as a point data scalar field
/// attached to a geometry, either 2D or 3D, either regular grid or
/// triangulation (vtkDataSet)
/// \param Output0 Output critical points (vtkUnstructuredGrid)
/// \param Output1 Output 1-separatrices (vtkUnstructuredGrid)
/// \param Output2 Output 2-separatrices (vtkUnstructuredGrid)
/// \param Output3 Output data segmentation (vtkDataSet)
///
/// This filter can be used as any other VTK filter (for instance, by using the
/// sequence of calls SetInputData(), Update(), GetOutput()).
///
/// See the related ParaView example state files for usage examples within a
/// VTK pipeline.
///
/// \sa ttk::MorseSmaleComplex
///

#pragma once

// VTK Module
#include <ttkMorseSmaleComplexModule.h>

// ttk code includes
#include <MorseSmaleComplex.h>
#include <ttkAlgorithm.h>

class vtkUnstructuredGrid;

class TTKMORSESMALECOMPLEX_EXPORT ttkMorseSmaleComplex
  : public ttkAlgorithm,
    protected ttk::MorseSmaleComplex {

public:
  static ttkMorseSmaleComplex *New();

  vtkTypeMacro(ttkMorseSmaleComplex, ttkAlgorithm);

  vtkSetMacro(ScalarField, std::string);
  vtkGetMacro(ScalarField, std::string);

  vtkSetMacro(ScalarFieldId, int);
  vtkGetMacro(ScalarFieldId, int);

  vtkSetMacro(OffsetFieldId, int);
  vtkGetMacro(OffsetFieldId, int);

  vtkSetMacro(ForceInputOffsetScalarField, bool);
  vtkGetMacro(ForceInputOffsetScalarField, bool);

  vtkSetMacro(InputOffsetScalarFieldName, std::string);
  vtkGetMacro(InputOffsetScalarFieldName, std::string);

  vtkSetMacro(IterationThreshold, int);
  vtkGetMacro(IterationThreshold, int);

  vtkSetMacro(ComputeCriticalPoints, bool);
  vtkGetMacro(ComputeCriticalPoints, bool);

  vtkSetMacro(ComputeAscendingSeparatrices1, bool);
  vtkGetMacro(ComputeAscendingSeparatrices1, bool);

  vtkSetMacro(ComputeDescendingSeparatrices1, bool);
  vtkGetMacro(ComputeDescendingSeparatrices1, bool);

  vtkSetMacro(ComputeSaddleConnectors, bool);
  vtkGetMacro(ComputeSaddleConnectors, bool);

  vtkSetMacro(ComputeAscendingSeparatrices2, bool);
  vtkGetMacro(ComputeAscendingSeparatrices2, bool);

  vtkSetMacro(ComputeDescendingSeparatrices2, bool);
  vtkGetMacro(ComputeDescendingSeparatrices2, bool);

  vtkSetMacro(ComputeAscendingSegmentation, bool);
  vtkGetMacro(ComputeAscendingSegmentation, bool);

  vtkSetMacro(ComputeDescendingSegmentation, bool);
  vtkGetMacro(ComputeDescendingSegmentation, bool);

  vtkSetMacro(ComputeFinalSegmentation, bool);
  vtkGetMacro(ComputeFinalSegmentation, bool);

  vtkSetMacro(ReturnSaddleConnectors, int);
  vtkGetMacro(ReturnSaddleConnectors, int);

  vtkSetMacro(SaddleConnectorsPersistenceThreshold, double);
  vtkGetMacro(SaddleConnectorsPersistenceThreshold, double);

  int setupTriangulation(vtkDataSet *input);
  vtkDataArray *getScalars(vtkDataSet *input);
  vtkDataArray *getOffsets(vtkDataSet *input);

protected:
  template <typename VTK_TT>
  int dispatch(vtkDataArray *inputScalars,
               vtkDataArray *inputOffsets,
               vtkUnstructuredGrid *outputCriticalPoints,
               vtkUnstructuredGrid *outputSeparatrices1,
               vtkUnstructuredGrid *outputSeparatrices2,
               ttk::Triangulation &triangulation);

  ttkMorseSmaleComplex();
  ~ttkMorseSmaleComplex() override;

  int FillInputPortInformation(int port, vtkInformation *info) override;
  int FillOutputPortInformation(int port, vtkInformation *info) override;
  int RequestData(vtkInformation *request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector) override;

private:
  std::string ScalarField{};
  std::string InputOffsetScalarFieldName{ttk::OffsetScalarFieldName};
  bool ForceInputOffsetScalarField{};
  int IterationThreshold{-1};
  bool ComputeCriticalPoints{true};
  bool ComputeAscendingSeparatrices1{true};
  bool ComputeDescendingSeparatrices1{true};
  bool ComputeSaddleConnectors{true};
  bool ComputeAscendingSeparatrices2{false};
  bool ComputeDescendingSeparatrices2{false};
  bool ComputeAscendingSegmentation{true};
  bool ComputeDescendingSegmentation{true};
  bool ComputeFinalSegmentation{true};
  int ScalarFieldId{};
  int OffsetFieldId{-1};
  int ReturnSaddleConnectors{false};
  double SaddleConnectorsPersistenceThreshold{0.0};

  ttk::Triangulation *triangulation_{};
  vtkDataArray *defaultOffsets_{};
  bool hasUpdatedMesh_{};
};
