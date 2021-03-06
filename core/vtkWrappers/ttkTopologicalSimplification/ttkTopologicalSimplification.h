/// \ingroup vtkWrappers
/// \class ttkTopologicalSimplification
/// \author Guillaume Favelier <guillaume.favelier@lip6.fr>
/// \date February 2016
///
/// \brief TTK VTK-filter for the topological simplification of scalar 
/// data.
/// 
/// Given an input scalar field and a list of critical points to remove, this 
/// filter minimally edits the scalar field such that the listed critical points
/// disappear. This procedure is useful to speedup subsequent topological data 
/// analysis when outlier critical points can be easily identified. It is 
/// also useful for data simplification.
///
/// The list of critical points to remove must be associated with a point data
/// scalar field that represent the vertex global identifiers in the input 
/// geometry.
/// 
/// Note that this filter will also produce an output vertex offset scalar field
/// that can be used for further topological data analysis tasks to disambiguate
/// vertices on flat plateaus. For instance, this output vertex offset field 
/// can specified to the vtkContourForests, vtkIntegralLines, or 
/// vtkScalarFieldCriticalPoints filters.
/// 
/// Also, this filter can be given a specific input vertex offset.
///
/// \param Input0 Input scalar field, either 2D or 3D, either regular grid or 
/// triangulation (vtkDataSet)
/// \param Input1 List of critical point constraints (vtkPointSet)
/// \param Output Output simplified scalar field (vtkDataSet)
///
/// This filter can be used as any other VTK filter (for instance, by using the 
/// sequence of calls SetInputData(), Update(), GetOutput()).
///
/// See the related ParaView example state files for usage examples within a 
/// VTK pipeline.
///
/// \b Related \b publication \n
/// "Generalized Topological Simplification of Scalar Fields on Surfaces" \n
/// Julien Tierny, Valerio Pascucci \n
/// Proc. of IEEE VIS 2012.\n
/// IEEE Transactions on Visualization and Computer Graphics, 2012.
///
/// \sa vtkScalarFieldCriticalPoints
/// \sa vtkIntegralLines
/// \sa vtkContourForests
/// \sa vtkIdentifiers
/// \sa ttk::TopologicalSimplification
#ifndef _TTK_TOPOLOGICALSIMPLIFICATION_H
#define _TTK_TOPOLOGICALSIMPLIFICATION_H

// ttk code includes
#include                  <TopologicalSimplification.h>
#include                  <ttkWrapper.h>

// VTK includes -- to adapt
#include                  <vtkCharArray.h>
#include                  <vtkUnsignedCharArray.h>
#include                  <vtkShortArray.h>
#include                  <vtkUnsignedShortArray.h>
#include                  <vtkIntArray.h>
#include                  <vtkFloatArray.h>
#include                  <vtkDoubleArray.h>
#include                  <vtkDataArray.h>
#include                  <vtkDataSet.h>
#include                  <vtkDataSetAlgorithm.h>
#include                  <vtkFiltersCoreModule.h>
#include                  <vtkInformation.h>
#include                  <vtkObjectFactory.h>
#include                  <vtkPointData.h>
#include                  <vtkSmartPointer.h>

#include<ttkTriangulation.h>

class VTKFILTERSCORE_EXPORT ttkTopologicalSimplification
: public vtkDataSetAlgorithm, public Wrapper{

  public:
    
    static ttkTopologicalSimplification* New();
    vtkTypeMacro(ttkTopologicalSimplification, vtkDataSetAlgorithm);

    vtkSetMacro(debugLevel_, int);

    void SetThreadNumber(int threadNumber){
      ThreadNumber = threadNumber;
      SetThreads();
    }

    void SetUseAllCores(bool onOff){
      UseAllCores = onOff;
      SetThreads();
    }

    vtkSetMacro(ScalarField, string);
    vtkGetMacro(ScalarField, string);

    vtkSetMacro(UseInputOffsetScalarField, int);
    vtkGetMacro(UseInputOffsetScalarField, int);

    vtkSetMacro(ConsiderIdentifierAsBlackList, int);
    vtkGetMacro(ConsiderIdentifierAsBlackList, int);

    vtkSetMacro(AddPerturbation, int);
    vtkGetMacro(AddPerturbation, int);

    vtkSetMacro(InputOffsetScalarFieldName, string);
    vtkGetMacro(InputOffsetScalarFieldName, string);

    vtkSetMacro(OutputOffsetScalarFieldName, string);
    vtkGetMacro(OutputOffsetScalarFieldName, string);

    vtkSetMacro(VertexIdentifierScalarField, string);
    vtkGetMacro(VertexIdentifierScalarField, string);

    int getTriangulation(vtkDataSet* input);
    int getScalars(vtkDataSet* input);
    int getIdentifiers(vtkPointSet* input);
    int getOffsets(vtkDataSet* input);

    
  protected:

    ttkTopologicalSimplification();
    
    ~ttkTopologicalSimplification();
    
    TTK_SETUP();

    int FillInputPortInformation(int port, vtkInformation* info);

    
  private:
   
    int ScalarFieldId;
    string ScalarField;
    string InputOffsetScalarFieldName;
    string OutputOffsetScalarFieldName;
    string VertexIdentifierScalarField;
    bool UseInputOffsetScalarField;
    bool ConsiderIdentifierAsBlackList;
    bool AddPerturbation;
    bool hasUpdatedMesh_;

    TopologicalSimplification topologicalSimplification_;
    Triangulation *triangulation_;
    vtkDataArray* identifiers_;
    vtkDataArray* inputScalars_;
    vtkIntArray* offsets_;
    vtkDataArray* inputOffsets_;

};

#endif // _TTK_TOPOLOGICALSIMPLIFICATION_H
