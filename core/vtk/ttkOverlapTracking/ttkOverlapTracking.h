/// \ingroup vtk
/// \class ttkOverlapTracking
/// \author Jonas Lukasczyk <jl@jluk.de>
/// \date 01.09.2018
///
/// \brief TTK VTK-filter that computes the overlap between labeled vtkPointSets.
///
/// This filter computes the overlap between labeled vtkPointSets stored as blocks of a vtkMultiBlockDataSet. Overlap is characterized by the equality of spatial point coordinates. This filter can be executed iteratively.
///
/// VTK wrapping code for the @OverlapTracking package.
///
/// \param Input vtkMultiBlockDataSet holding the vtkPointSets
/// \param Output vtkUnstructuredGrid Tracking Graph
///
/// sa ttk::OverlapTracking

#pragma once

// VTK includes
#include <vtkUnstructuredGridAlgorithm.h>
#include <vtkInformation.h>

// TTK includes
#include <ttkWrapper.h>
#include <OverlapTracking.h>

#ifndef TTK_PLUGIN
class VTKFILTERSCORE_EXPORT ttkOverlapTracking
#else
class ttkOverlapTracking
#endif
: public vtkUnstructuredGridAlgorithm, public ttk::Wrapper{

    public:
        static ttkOverlapTracking* New();
        vtkTypeMacro(ttkOverlapTracking, vtkUnstructuredGridAlgorithm)

        vtkSetMacro(LabelScalarField, string);
        vtkGetMacro(LabelScalarField, string);

        // default ttk setters
        vtkSetMacro(debugLevel_, int);
        void SetThreads(){
            threadNumber_ = !UseAllCores ? ThreadNumber : ttk::OsCall::getNumberOfCores();
            Modified();
        }
        void SetThreadNumber(int threadNumber){
            ThreadNumber = threadNumber;
            SetThreads();
        }
        void SetUseAllCores(bool onOff){
            UseAllCores = onOff;
            SetThreads();
        }
        // end of default ttk setters

        int FillInputPortInformation(int port, vtkInformation* info) override {
            switch(port)
                case 0: info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMultiBlockDataSet");
            return 1;
        }

        int FillOutputPortInformation(int port, vtkInformation* info) override {
            switch(port)
                case 0: info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkUnstructuredGrid");
            return 1;
        }

    protected:

        ttkOverlapTracking(){
            LabelScalarField = "RegionId";

            UseAllCores = false;

            SetNumberOfInputPorts(1);
            SetNumberOfOutputPorts(1);
        }
        ~ttkOverlapTracking(){};

        bool UseAllCores;
        int ThreadNumber;

        int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector) override;

        int processTimestep(vtkDataObject* dataObject);
        int finalize(vtkUnstructuredGrid* trackingGraph);

    private:

        string LabelScalarField;
        ttk::OverlapTracking overlapTracking;

        bool needsToAbort() override { return GetAbortExecute();};
        int updateProgress(const float &progress) override {
            UpdateProgress(progress);
            return 0;
        };
};
