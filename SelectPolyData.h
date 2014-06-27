#ifndef SELECTPOLYDATA_H
#define SELECTPOLYDATA_H
#include <vtkPolyDataAlgorithm.h>
#include <vector>
#include <QtGui>

#define VTK_INSIDE_SMALLEST_REGION 0
#define VTK_INSIDE_LARGEST_REGION 1
#define VTK_INSIDE_CLOSEST_POINT_REGION 2

class vtkCharArray;
class vtkPoints;
class vtkIdList;

/*
struct Pic
{
	Pic(){};
	std::vector<double*> position;
	std::vector<std::vector<int>> neibours;

	std::vector<int> getNeibour(int id)
	{
		return neibours[id];
	}

	double* getPosition(int id)
	{
		return position[id];
	}

};

*/

class SelectPolyData : public vtkPolyDataAlgorithm
{
public:
  // Description:
  // Instantiate object with InsideOut turned off, and 
  // GenerateSelectionScalars turned off. The unselected output
  // is not generated, and the inside mode is the smallest region.
  static SelectPolyData *New();

  vtkTypeMacro(SelectPolyData,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get the flag to control behavior of the filter. If
  // GenerateSelectionScalars is on, then the output of the filter
  // is the same as the input, except that scalars are generated.
  // If off, the filter outputs the cells laying inside the loop, and
  // does not generate scalars.
  vtkSetMacro(GenerateSelectionScalars,int);
  vtkGetMacro(GenerateSelectionScalars,int);
  vtkBooleanMacro(GenerateSelectionScalars,int);

  // Description:
  // Set/Get the InsideOut flag. When off, the mesh within the loop is
  // extracted. When on, the mesh outside the loop is extracted.
  vtkSetMacro(InsideOut,int);
  vtkGetMacro(InsideOut,int);
  vtkBooleanMacro(InsideOut,int);

  // Description:
  // Set/Get the array of point coordinates defining the loop. There must
  // be at least three points used to define a loop.
  virtual void SetLoop(vtkPoints*);
  vtkGetObjectMacro(Loop,vtkPoints);

  // Description:
  // Control how inside/outside of loop is defined.
  vtkSetClampMacro(SelectionMode,int,
             VTK_INSIDE_SMALLEST_REGION,VTK_INSIDE_CLOSEST_POINT_REGION);
  vtkGetMacro(SelectionMode,int);
  void SetSelectionModeToSmallestRegion()
    {this->SetSelectionMode(VTK_INSIDE_SMALLEST_REGION);};
  void SetSelectionModeToLargestRegion()
    {this->SetSelectionMode(VTK_INSIDE_LARGEST_REGION);};
  void SetSelectionModeToClosestPointRegion()
    {this->SetSelectionMode(VTK_INSIDE_CLOSEST_POINT_REGION);};
  const char *GetSelectionModeAsString();

  // Description:
  // Control whether a second output is generated. The second output
  // contains the polygonal data that's not been selected.
  vtkSetMacro(GenerateUnselectedOutput,int);
  vtkGetMacro(GenerateUnselectedOutput,int);
  vtkBooleanMacro(GenerateUnselectedOutput,int);

  // Description:
  // Return output that hasn't been selected (if GenreateUnselectedOutput is
  // enabled).
  vtkPolyData *GetUnselectedOutput();

  // Description:
  // Return the (mesh) edges of the selection region.
  vtkPolyData *GetSelectionEdges();

  // Overload GetMTime() because we depend on Loop
  unsigned long int GetMTime();

 protected:
	SelectPolyData();
  ~SelectPolyData();
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  int GenerateSelectionScalars;
  int InsideOut;
  vtkPoints *Loop;
  int SelectionMode;
  double ClosestPoint[3];
  int GenerateUnselectedOutput;

private:
  vtkPolyData *Mesh;
  void GetPointNeighbors (vtkIdType ptId, vtkIdList *nei);
  vtkIdType FindClosestVertex(vtkPolyData*inputMesh, double point[3]);
  //void InputPic(Pic &);
private:
  SelectPolyData(const SelectPolyData&);  // Not implemented.
  void operator=(const SelectPolyData&);  // Not implemented.
};

// Description:
// Return the method of determining in/out of loop as a string.
inline const char *SelectPolyData::GetSelectionModeAsString(void)
{
  if ( this->SelectionMode == VTK_INSIDE_SMALLEST_REGION ) 
    {
    return "InsideSmallestRegion";
    }
  else if ( this->SelectionMode == VTK_INSIDE_LARGEST_REGION ) 
    {
    return "InsideLargestRegion";
    }
  else 
    {
    return "InsideClosestPointRegion";
    }
}

#endif