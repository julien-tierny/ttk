#include <ttkSurfaceQuadrangulation.h>

vtkStandardNewMacro(ttkSurfaceQuadrangulation);

ttkSurfaceQuadrangulation::ttkSurfaceQuadrangulation()
  : InputOffsetIdentifiersFieldName{}, ForceInputIdentifiersField{false},
    ForceInputOffsetIdentifiersField{false}, SubdivisionLevel{5},
    RelaxationIterations{100}, surfaceQuadrangulation_{}, triangulation_{},
    scalarField_{}, identifiersField_{}, offsetIdentifiersField_{} {

  InputIdentifiersFieldName
    = std::string(static_cast<const char *>(ttk::VertexScalarFieldName));

  SetNumberOfInputPorts(1);
}

int ttkSurfaceQuadrangulation::FillInputPortInformation(int port,
                                                        vtkInformation *info) {

  if(port == 0) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkDataSet");
  }
  return 0;
}

int ttkSurfaceQuadrangulation::getTriangulation(vtkDataSet *input) {

  triangulation_ = ttkTriangulation::getTriangulation(input);

#ifndef TTK_ENABLE_KAMIKAZE
  if(triangulation_ == nullptr) {
    cerr << "[ttkSurfaceQuadrangulation] Error: input triangulation pointer "
            "is NULL."
         << endl;
    return -1;
  }
#endif

  triangulation_->setWrapper(this);
  surfaceQuadrangulation_.setWrapper(this);
  surfaceQuadrangulation_.setupTriangulation(triangulation_);
  Modified();

#ifndef TTK_ENABLE_KAMIKAZE
  if(triangulation_->isEmpty()) {
    cerr << "[ttkSurfaceQuadrangulation] Error: ttkTriangulation allocation "
            "problem."
         << endl;
    return -1;
  }
#endif

  return 0;
}

int ttkSurfaceQuadrangulation::getScalarField(vtkDataSet *input) {

#ifndef TTK_ENABLE_KAMIKAZE
  if(input == nullptr) {
    cerr << "[ttkSurfaceQuadrangulation] Error: input pointer is NULL." << endl;
    return -1;
  }

  if(input->GetNumberOfPoints() == 0) {
    cerr << "[ttkSurfaceQuadrangulation] Error: input has no point." << endl;
    return -1;
  }
#endif

  vtkPointData *pointData = input->GetPointData();

#ifndef TTK_ENABLE_KAMIKAZE
  if(pointData == nullptr) {
    cerr << "[ttkSurfaceQuadrangulation] Error: input has no point data."
         << endl;
    return -1;
  }
#endif

  if(InputScalarFieldName.length() != 0) {
    scalarField_ = pointData->GetArray(InputScalarFieldName.data());
  } else {
    scalarField_ = pointData->GetArray(0);
  }

#ifndef TTK_ENABLE_KAMIKAZE
  if(scalarField_ == nullptr) {
    cerr
      << "[ttkSurfaceQuadrangulation] Error: NULL input scalar field pointer."
      << endl;
    return -1;
  }
#endif

  return 0;
}

int ttkSurfaceQuadrangulation::getIdentifiersField(vtkDataSet *input) {
  vtkPointData *pointData = input->GetPointData();
  auto vsfn = static_cast<const char *>(ttk::VertexScalarFieldName);

  if(ForceInputIdentifiersField && InputIdentifiersFieldName.length() != 0) {
    identifiersField_ = pointData->GetArray(InputIdentifiersFieldName.data());
  } else if(pointData->GetArray(vsfn) != nullptr) {
    identifiersField_ = pointData->GetArray(vsfn);
  }

#ifndef TTK_ENABLE_KAMIKAZE
  if(identifiersField_ == nullptr) {
    cerr << "[ttkSurfaceQuadrangulation] Error: wrong identifiers field."
         << endl;
    return -1;
  }
#endif

  return 0;
}

int ttkSurfaceQuadrangulation::getOffsetIdentifiersField(vtkDataSet *input) {
  vtkPointData *pointData = input->GetPointData();
  auto osfn = static_cast<const char *>(ttk::OffsetScalarFieldName);
  auto vsfn = static_cast<const char *>(ttk::VertexScalarFieldName);

  if(ForceInputOffsetIdentifiersField
     && InputOffsetIdentifiersFieldName.length() != 0) {
    offsetIdentifiersField_
      = pointData->GetArray(InputOffsetIdentifiersFieldName.data());
  } else if(pointData->GetArray(osfn) != nullptr) {
    offsetIdentifiersField_ = pointData->GetArray(osfn);
  } else if(pointData->GetArray(vsfn) != nullptr) {
    // take the identifiers scalar field
    offsetIdentifiersField_ = pointData->GetArray(vsfn);
  } else {
    // take the first array in the dataset
    offsetIdentifiersField_ = pointData->GetArray(0);
  }

#ifndef TTK_ENABLE_KAMIKAZE
  if(offsetIdentifiersField_ == nullptr) {
    cerr << "[ttkSurfaceQuadrangulation] Error: wrong offset identifiers field."
         << endl;
    return -1;
  }
#endif

  return 0;
}

int ttkSurfaceQuadrangulation::doIt(std::vector<vtkDataSet *> &inputs,
                                    std::vector<vtkDataSet *> &outputs) {

  ttk::Memory m;

  vtkDataSet *domain = vtkDataSet::SafeDownCast(inputs[0]);
  vtkDataSet *output = vtkDataSet::SafeDownCast(outputs[0]);

  int res = 0;

  res += getTriangulation(domain);

#ifndef TTK_ENABLE_KAMIKAZE
  if(res != 0) {
    cerr << "[ttkSurfaceQuadrangulation] Error: wrong triangulation." << endl;
    return -1;
  }
#endif

  const ttk::SimplexId numberOfPointsInDomain = domain->GetNumberOfPoints();

#ifndef TTK_ENABLE_KAMIKAZE
  if(numberOfPointsInDomain == 0) {
    cerr << "[ttkSurfaceQuadrangulation] Error: domain has no points." << endl;
    return -3;
  }
#endif

  res += getScalarField(domain);
  res += getIdentifiersField(domain);
  res += getOffsetIdentifiersField(domain);

#ifndef TTK_ENABLE_KAMIKAZE
  // something wrong when getting scalar fields from VTK
  if(res != 0) {
    cerr << "[ttkSurfaceQuadrangulation] Error: Wrong scalar field selected."
         << endl;
    return -4;
  }
#endif

  surfaceQuadrangulation_.setVertexNumber(numberOfPointsInDomain);
  surfaceQuadrangulation_.setSubdivisionLevel(SubdivisionLevel);
  surfaceQuadrangulation_.setRelaxationIterations(RelaxationIterations);
  surfaceQuadrangulation_.setInputScalarFieldPointer(scalarField_);
  surfaceQuadrangulation_.setInputOffsetIdentifiersFieldPointer(
    offsetIdentifiersField_);

  std::vector<ttk::SimplexId> outputVertices;
  std::vector<std::vector<ttk::SimplexId>> outputEdges;

  res += surfaceQuadrangulation_.execute();

#ifndef TTK_ENABLE_KAMIKAZE
  // something wrong in base/SurfaceQuadrangulation
  if(res != 0) {
    cerr << "[ttkSurfaceQuadrangulation] SurfaceQuadrangulation.execute() "
            "error code: "
         << res << endl;
    return -9;
  }
#endif

  // update result
  auto outputQuadrangulation = vtkSmartPointer<vtkIntArray>::New();
  outputQuadrangulation->SetNumberOfComponents(1);
  outputQuadrangulation->SetVoidArray(
    static_cast<void *>(outputVertices.data()), outputVertices.size(), 1);

  output->ShallowCopy(domain);
  // output->GetPointData()->AddArray(outputQuadrangulation);

  std::stringstream msg;
  msg << "[ttkSurfaceQuadrangulation] Memory usage: " << m.getElapsedUsage()
      << " MB." << endl;
  dMsg(cout, msg.str(), memoryMsg);

  return 0;
}
