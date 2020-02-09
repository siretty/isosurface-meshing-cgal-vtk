#include <iostream>

#include <cstddef>

#include <vtkContourFilter.h>
#include <vtkSampleFunction.h>
#include <vtkSmartPointer.h>

#include <vtkActor.h>
#include <vtkImageData.h>
#include <vtkImplicitFunction.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkNamedColors.h>
#include <vtkSuperquadric.h>

#include <vtkOBJWriter.h>

class CustomSphere : public vtkImplicitFunction {
public:
  static CustomSphere *New() { return new CustomSphere(); }

  double EvaluateFunction(double x[3]) override {
    ++function_evaluations;
    return x[0] * x[0] + x[1] * x[1] + x[2] * x[2];
  }

  void EvaluateGradient(double x[3], double g[3]) override {
    ++gradient_evaluations;
    g[0] = 2 * x[0];
    g[1] = 2 * x[1];
    g[2] = 2 * x[2];
  }

  size_t function_evaluations = 0;
  size_t gradient_evaluations = 0;
};

int main(int, char *[]) {
  vtkSmartPointer<vtkNamedColors> colors =
      vtkSmartPointer<vtkNamedColors>::New();

  // vtkSmartPointer<vtkSuperquadric> implicitFunction =
  //    vtkSmartPointer<vtkSuperquadric>::New();
  // implicitFunction->SetPhiRoundness(2.5);
  // implicitFunction->SetThetaRoundness(.5);

  vtkSmartPointer<CustomSphere> implicitFunction =
      vtkSmartPointer<CustomSphere>::New();

  // Sample the function
  vtkSmartPointer<vtkSampleFunction> sample =
      vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(50, 50, 50);
  sample->SetImplicitFunction(implicitFunction);
  //double value = 1.0;
  //double xmin = -value, xmax = value, ymin = -value, ymax = value,
  //       zmin = -value, zmax = value;
  //sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);
  sample->SetModelBounds(-2, 2, -2, 2, -2, 2);

  // Create the 0 isosurface
  vtkSmartPointer<vtkContourFilter> contours =
      vtkSmartPointer<vtkContourFilter>::New();
  contours->SetInputConnection(sample->GetOutputPort());
  contours->GenerateValues(1, 1.0, 1.0);

  vtkSmartPointer<vtkOBJWriter> writer =
    vtkSmartPointer<vtkOBJWriter>::New();
  writer->SetFileName("vtk.obj");
  writer->SetInputConnection(contours->GetOutputPort());
  writer->Write();

  /*
  // Map the contours to graphical primitives
  vtkSmartPointer<vtkPolyDataMapper> contourMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  contourMapper->SetInputConnection(contours->GetOutputPort());
  contourMapper->SetScalarRange(0.0, 1.2);

  // Create an actor for the contours
  vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();
  contourActor->SetMapper(contourMapper);

  // -- create a box around the function to indicate the sampling volume --

  // Create outline
  vtkSmartPointer<vtkOutlineFilter> outline =
      vtkSmartPointer<vtkOutlineFilter>::New();
  outline->SetInputConnection(sample->GetOutputPort());

  // Map it to graphics primitives
  vtkSmartPointer<vtkPolyDataMapper> outlineMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineMapper->SetInputConnection(outline->GetOutputPort());

  // Create an actor for it
  vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
  outlineActor->SetMapper(outlineMapper);
  outlineActor->GetProperty()->SetColor(0, 0, 0);

  // Visualize
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> interactor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  renderer->AddActor(contourActor);
  renderer->AddActor(outlineActor);
  renderer->SetBackground(colors->GetColor3d("Tan").GetData());

  renderWindow->Render();
  interactor->Start();
  */

  std::cerr << "function evaluations: " << implicitFunction->function_evaluations << '\n';
  std::cerr << "gradient evaluations: " << implicitFunction->gradient_evaluations << '\n';

  return EXIT_SUCCESS;
}
