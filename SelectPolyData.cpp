/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSelectPolyData.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "SelectPolyData.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkCharArray.h"
#include "vtkExecutive.h"
#include "vtkFloatArray.h"
#include "vtkGarbageCollector.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkLine.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkTriangleFilter.h"
#include "vtkTriangleStrip.h"
#include "vtkSmartPointer.h"
#include <queue>
#include <map>
#include <vector>
#include <time.h>
#include <Queue.h>
#include "vtkCellLocator.h"


vtkStandardNewMacro(SelectPolyData);
vtkCxxSetObjectMacro(SelectPolyData,Loop,vtkPoints);


double dist(double a1[], double a2[])
{
	return sqrt((a1[0]-a2[0])*(a1[0]-a2[0]) + (a1[1]-a2[1])*(a1[1]-a2[1]) + (a1[2]-a2[2])*(a1[2]-a2[2]));
}

double angleCos(double a1[],double a2[])
{
	return (a1[0]*a2[0]+a1[1]*a2[1]+a1[2]*a2[2])/sqrt((a1[0]*a1[0]+a1[1]*a1[1]+a1[2]*a1[2])*(a2[0]*a2[0]+a2[1]*a2[1]+a2[2]*a2[2]));
}

// Description:
// Instantiate object with InsideOut turned off.
SelectPolyData::SelectPolyData()
{
  this->GenerateSelectionScalars = 0;
  this->InsideOut = 0;
  this->Loop = NULL;
  this->SelectionMode = VTK_INSIDE_SMALLEST_REGION;
  this->ClosestPoint[0] = this->ClosestPoint[1] = this->ClosestPoint[2] = 0.0;
  this->GenerateUnselectedOutput = 0;

  this->SetNumberOfOutputPorts(3);

  vtkPolyData *output2 = vtkPolyData::New();
  this->GetExecutive()->SetOutputData(1, output2);
  output2->Delete();

  vtkPolyData *output3 = vtkPolyData::New();
  this->GetExecutive()->SetOutputData(2, output3);
  output3->Delete();
}

//----------------------------------------------------------------------------
SelectPolyData::~SelectPolyData()
{
  if (this->Loop)
    {
    this->Loop->Delete();
    }
}

//----------------------------------------------------------------------------
vtkPolyData *SelectPolyData::GetUnselectedOutput()
{
  if (this->GetNumberOfOutputPorts() < 2)
    {
    return NULL;
    }
  
  return vtkPolyData::SafeDownCast(
    this->GetExecutive()->GetOutputData(1));
}

//----------------------------------------------------------------------------
vtkPolyData *SelectPolyData::GetSelectionEdges()
{
  if (this->GetNumberOfOutputPorts() < 3)
    {
    return NULL;
    }
  
  return vtkPolyData::SafeDownCast(
    this->GetExecutive()->GetOutputData(2));
}

//----------------------------------------------------------------------------
int SelectPolyData::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkIdType numPts, numLoopPts;
  vtkPolyData *triMesh;
  vtkPointData *inPD, *outPD=output->GetPointData();
  vtkCellData *inCD, *outCD=output->GetCellData();
  vtkIdType closest=0, numPolys, i, j;
  int k;
  vtkIdList *loopIds, *edgeIds, *neighbors;//*tmp_edgeIds
  double x[3], xLoop[3], closestDist2, dist2, t;
  double x0[3], x1[3], vec[3], dir[3], neiX[3];
  vtkCellArray *inPolys;
  vtkPoints *inPts;
  int numNei;
  vtkIdType id, currentId = 0, nextId, pt1, pt2, numCells, neiId;
  vtkIdType *cells, *pts, npts, numMeshLoopPts, prevId;
  unsigned short int ncells;
  int mark, s1, s2, val;

  // Initialize and check data
  vtkDebugMacro(<<"Selecting data...");

  this->GetUnselectedOutput()->Initialize();
  this->GetSelectionEdges()->Initialize();

  if ( (numPts = input->GetNumberOfPoints()) < 1 )
    {
    vtkErrorMacro("Input contains no points");
    return 1;
    }

  if ( this->Loop == NULL || 
  (numLoopPts=this->Loop->GetNumberOfPoints()) < 3 )
    {
    vtkErrorMacro("Please define a loop with at least three points");
    return 1;
    }

  // Okay, now we build unstructured representation. Make sure we're
  // working with triangles.
  vtkTriangleFilter *tf=vtkTriangleFilter::New();
  tf->SetInput(input);
  tf->PassLinesOff();
  tf->PassVertsOff();
  tf->Update();
  triMesh = tf->GetOutput();
  triMesh->Register(this);
  tf->Delete();
  inPD = triMesh->GetPointData();
  inCD = triMesh->GetCellData();

  numPts = triMesh->GetNumberOfPoints();
  inPts = triMesh->GetPoints();
  inPolys = triMesh->GetPolys();
  numPolys = inPolys->GetNumberOfCells();
  if ( numPolys < 1 )
    {
    vtkErrorMacro("This filter operates on surface primitives");
    tf->Delete();
    return 1;
    }

  this->Mesh = vtkPolyData::New();
  this->Mesh->SetPoints(inPts); //inPts
  this->Mesh->SetPolys(inPolys);
  this->Mesh->BuildLinks(); //to do neighborhood searching
  numCells = this->Mesh->GetNumberOfCells();

  // First thing to do is find the closest mesh points to the loop
  // points. This creates a list of point ids.
  loopIds = vtkIdList::New();
  loopIds->SetNumberOfIds(numLoopPts);

	//for(int i=0; i<numLoopPts; i++)
	//{
	//	this->Loop->GetPoint(i,xLoop);	
	//	closest=FindClosestVertex(this->Mesh,xLoop);
	//	if(closest)
	//	{
	//		loopIds->SetId(i,closest);
	//	}
	//	else
	//	{
	//		vtkErrorMacro(<<"Can't find closest vertex.");
	//	}	
	//}

  for ( i=0; i < numLoopPts; i++)
    {
    this->Loop->GetPoint(i,xLoop);
    closest = -1;
    closestDist2 = VTK_DOUBLE_MAX;

    for ( j=0; j < numPts; j++)
      {
      inPts->GetPoint(j,x);

      dist2 = vtkMath::Distance2BetweenPoints(x, xLoop);
      if ( dist2 < closestDist2 )
        {
        closest = j;
        closestDist2 = dist2;
        }
      } //for all input points

    loopIds->SetId(i,closest);
    } //for all loop points

  // Now that we've got point ids, we build the loop. Start with the
  // first two points in the loop (which define a line), and find the
  // mesh edge that is directed along the line, and whose
  // end point is closest to the line. Continue until loop closes in on
  // itself.
  /*
      vtkErrorMacro("Enter0");
	tmp_edgeIds = vtkIdList::New();
	tmp_edgeIds->Allocate(numLoopPts*100000);
	neighbors = vtkIdList::New();
	neighbors->Allocate(10000);
	tmp_edgeIds->InsertNextId(loopIds->GetId(0));
	for ( i=0; i < numLoopPts; i++ )
	{
		currentId = loopIds->GetId(i);
		nextId = loopIds->GetId((i+1)%numLoopPts);
		inPts->GetPoint(currentId, x);
		inPts->GetPoint(currentId, x0);
		std::map<int, MyData> SPmap;
		MyData MDtmp = MyData(currentId, 0.0);
		SPmap[currentId] = MDtmp;
		Queue myqueue = Queue();
		myqueue.push(currentId);
		bool abc = false;
		while(!myqueue.empty())
		{ 
			int curp = myqueue.pop();
			this->GetPointNeighbors(curp, neighbors); //points connected by edge
			numNei = neighbors->GetNumberOfIds();
			inPts->GetPoint(curp, x);
			for (j=0; j<numNei; j++)
			{
				neiId = neighbors->GetId(j);
				inPts->GetPoint(neiId, neiX);
				double distTmp = dist(neiX,x) + SPmap[curp].Distance;
				if(distTmp < SPmap[neiId].Distance){
					myqueue.push(neiId);
					MDtmp = MyData(curp, distTmp);
					SPmap[neiId] = MDtmp;
				}
			}
		}
		std::vector<int> vecTmp;
		int idPtr = nextId;
		while(idPtr != currentId)
		{
			vecTmp.push_back(idPtr);
			idPtr = SPmap[idPtr].PointId;
		}
		for(int i = vecTmp.size()-1; i>= 0; i--)
		{
			tmp_edgeIds->InsertNextId(vecTmp[i]);
		}
	}
			    vtkErrorMacro("Enter1");

	#define SAMPLE_STEP 4
	int newSize = tmp_edgeIds->GetNumberOfIds();
	//Loop->Initialize();
	//Loop->Allocate(newSize / SAMPLE_STEP);
	//Loop->SetNumberOfPoints(newSize / SAMPLE_STEP);
	for(int i = 0; i < newSize; i+= SAMPLE_STEP)
	{
		double xTmp[3];
		inPts->GetPoint(tmp_edgeIds->GetId(i),xTmp);
		Loop->SetPoint(i / SAMPLE_STEP,xTmp);
	}
	vtkErrorMacro("Enter2");
	loopIds->SetNumberOfIds(newSize / SAMPLE_STEP);
	for(int i = 0; i < newSize; i+= SAMPLE_STEP)
	{
		loopIds->SetId(i / SAMPLE_STEP, tmp_edgeIds->GetId(i));
	}
	*/
	edgeIds = vtkIdList::New();
	edgeIds->Allocate(numLoopPts*10000);
	neighbors = vtkIdList::New();
	neighbors->Allocate(10000);
	edgeIds->InsertNextId(loopIds->GetId(0));
	//numLoopPts = newSize / SAMPLE_STEP;
	for ( i=0; i < numLoopPts; i++ )
    {
		currentId = loopIds->GetId(i);
		nextId = loopIds->GetId((i+1)%numLoopPts);
		inPts->GetPoint(currentId, x);
		inPts->GetPoint(currentId, x0);
		inPts->GetPoint(nextId, x1);
		prevId = (-1);
		std::vector<int> myPath;
		bool flag = true;
		for (j=0; j<3; j++)
		{
			vec[j] = x1[j] - x0[j];
		}
		// track edge
		for (id=currentId; id != nextId; )
		{
			if(flag == false)
				break;
			this->GetPointNeighbors(id,neighbors); //points connected by edge
			numNei = neighbors->GetNumberOfIds();
			closest = -1;
			closestDist2 = VTK_DOUBLE_MAX;
			for (j=0; j<numNei; j++)
			{
				neiId = neighbors->GetId(j);
				inPts->GetPoint(neiId, neiX);
				for (k=0; k<3; k++)
				{
					dir[k] = neiX[k] - x[k];
				}
				if ( neiId != prevId && vtkMath::Dot(dir,vec) > 0.0 ) //candidate
				{
					dist2 = vtkLine::DistanceToLine(neiX, x0, x1);
					if ( dist2 < closestDist2 )
					{
						closest = neiId;
						closestDist2 = dist2;
					}
				}//in direction of line
			}//for all neighbors
			if ( closest < 0 )
			{
				flag = false;
			}
			else
			{
				myPath.push_back(closest);
				prevId = id;
				id = closest;
				inPts->GetPoint(id, x);
			}
		}//for tracking edge
		if(flag == true){
			for(unsigned int i = 0; i < myPath.size(); i++){
				edgeIds->InsertNextId(myPath[i]);
			}
		}
		else{
		    vtkErrorMacro("Enter");
			std::map<int, MyData> SPmap;
			MyData MDtmp = MyData(currentId, 0.0);
			SPmap[currentId] = MDtmp;
			Queue myqueue = Queue();
			myqueue.push(currentId);

			//clock_t time0 = std::clock();
			while(!myqueue.empty())
			{
				//clock_t timeCur = std::clock();
				//if(((timeCur - time0)/double(1000) > (double(5) / numLoopPts)) && flag)
				//if(flag)
				//	break;
				int curp = myqueue.pop();
				this->GetPointNeighbors(curp,neighbors); //points connected by edge
				numNei = neighbors->GetNumberOfIds();
				inPts->GetPoint(curp, x);
				for (j=0; j<numNei; j++)
				{
					neiId = neighbors->GetId(j);
					inPts->GetPoint(neiId, neiX);
					double distTmp = dist(neiX,x) + SPmap[curp].Distance;
					if(distTmp < SPmap[neiId].Distance){
						myqueue.push(neiId);
						MDtmp = MyData(curp, distTmp);
						SPmap[neiId] = MDtmp;

					}
				}
			}
			std::vector<int> vecTmp;
			int idPtr = nextId;
			while(idPtr != currentId)
			{
				vecTmp.push_back(idPtr);
				idPtr = SPmap[idPtr].PointId;
			}
			for(int i = vecTmp.size()-1; i>= 0; i--)
			{
				edgeIds->InsertNextId(vecTmp[i]);
			}
		}
  }

  /*
  int szz = tmp_edgeIds->GetNumberOfIds() /SAMPLE_STEP;
  Loop->Allocate(szz +1);
  Loop->Initialize();
  Loop->SetNumberOfPoints(szz+1);
  double tmp_points[3];
  for(int i = 0; i < szz * SAMPLE_STEP; i+=SAMPLE_STEP){
	  inPts->GetPoint(tmp_edgeIds->GetId(i),tmp_points);
	  Loop->InsertNextPoint(tmp_points);
  }
  inPts->GetPoint(tmp_edgeIds->GetId(tmp_edgeIds->GetNumberOfIds()-1),tmp_points);
  Loop->InsertNextPoint(tmp_points);
  numLoopPts = Loop->GetNumberOfPoints();
  */
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/*
  int SimpSize = tmp_edgeIds->GetNumberOfIds() / SAMPLE_STEP;
   edgeIds = vtkIdList::New();
  edgeIds->Allocate(numLoopPts*10,1000);
  neighbors = vtkIdList::New();
  neighbors->Allocate(10000);
  edgeIds->InsertNextId(tmp_edgeIds->GetId(0));

  for ( i=0; i < SimpSize; i++)
    {
    currentId = tmp_edgeIds->GetId(i * SAMPLE_STEP);
	if(i == SimpSize - 1)
		nextId = tmp_edgeIds->GetId(0);
	else
		nextId = tmp_edgeIds->GetId((i+1)*SAMPLE_STEP);
    prevId = (-1);
    inPts->GetPoint(currentId, x);
    inPts->GetPoint(currentId, x0);
    inPts->GetPoint(nextId, x1);
    for (j=0; j<3; j++)
      {
      vec[j] = x1[j] - x0[j];
      }

    // track edge
    for (id=currentId; id != nextId; )
      {
      this->GetPointNeighbors(id,neighbors); //points connected by edge
      numNei = neighbors->GetNumberOfIds();
      closest = -1;
      closestDist2 = VTK_DOUBLE_MAX;
      for (j=0; j<numNei; j++)
        {
        neiId = neighbors->GetId(j);
        inPts->GetPoint(neiId, neiX);
        for (k=0; k<3; k++)
          {
          dir[k] = neiX[k] - x[k];
          }
        if ( neiId != prevId && vtkMath::Dot(dir,vec) > 0.0 ) //candidate
          {
          dist2 = vtkLine::DistanceToLine(neiX, x0, x1);
          if ( dist2 < closestDist2 )
            {
            closest = neiId;
            closestDist2 = dist2;
            }
          }//in direction of line
        }//for all neighbors

      if ( closest < 0 )
        {



        }
      else
        {
        edgeIds->InsertNextId(closest);
        prevId = id;
        id = closest;
        inPts->GetPoint(id, x);
        }
      }//for tracking edge
    }//for all edges of loop


	*/
    /*
  int SimpSize = tmp_edgeIds->GetNumberOfIds() / SAMPLE_STEP;
  
  edgeIds = vtkIdList::New();
  edgeIds->Allocate(SimpSize*SAMPLE_STEP*5);
  neighbors = vtkIdList::New();
  neighbors->Allocate(10000);
  edgeIds->InsertNextId(tmp_edgeIds->GetId(0));

  for ( i=0; i < SimpSize; i++ )
    {
    currentId = tmp_edgeIds->GetId(i * SAMPLE_STEP);
	if(i == (SimpSize - 1))
		nextId = tmp_edgeIds->GetId(0);
	else
		nextId = tmp_edgeIds->GetId((i+1) * SAMPLE_STEP);

    inPts->GetPoint(currentId, x);
    inPts->GetPoint(nextId, x1);

	std::map<int, MyData> SPmap;
	MyData MDtmp = MyData(currentId, 0.0);
	SPmap[currentId] = MDtmp;
	Queue myqueue = Queue();
	myqueue.push(currentId,1.05 * dist(x1,x));
	bool flag = false;
	//clock_t time0 = std::clock();
	
	while(!myqueue.empty())
	{
		//clock_t timeCur = std::clock();
		//if(((timeCur - time0)/double(1000) > (double(5) / numLoopPts)) && flag)
		if(flag)
			break;
		int curp = myqueue.pop();
		this->GetPointNeighbors(curp,neighbors); //points connected by edge
		numNei = neighbors->GetNumberOfIds();
		inPts->GetPoint(curp, x);
		for (j=0; j<numNei; j++)
		{
			neiId = neighbors->GetId(j);
			inPts->GetPoint(neiId, neiX);
			double distTmp = dist(neiX,x) + SPmap[curp].Distance;
			if(distTmp < SPmap[neiId].Distance){
				double distToEnd = dist(x1,neiX);
				myqueue.push(neiId,distTmp + 1.05 * distToEnd );
				MDtmp = MyData(curp, distTmp);
				SPmap[neiId] = MDtmp;
				if(neiId == nextId)
					flag = true;
			}
		}
	}
	std::vector<int> vecTmp;
	int idPtr = nextId;
	while(idPtr != currentId)
	{
		vecTmp.push_back(idPtr);
		idPtr = SPmap[idPtr].PointId;
	}
	for(int i = vecTmp.size()-1; i>= 0; i--)
	{
		edgeIds->InsertNextId(vecTmp[i]);
	}
  }

  */
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //vtkErrorMacro("OK");
  
  // mainly for debugging
  numMeshLoopPts = edgeIds->GetNumberOfIds();
  vtkCellArray *selectionEdges=vtkCellArray::New();
  selectionEdges->Allocate(selectionEdges->EstimateSize(1,numMeshLoopPts),100);
  selectionEdges->InsertNextCell(numMeshLoopPts);
  for (i=0; i<numMeshLoopPts; i++)
    {
    selectionEdges->InsertCellPoint(edgeIds->GetId(i));
    }
  this->GetSelectionEdges()->SetPoints(inPts);
  this->GetSelectionEdges()->SetLines(selectionEdges);
  selectionEdges->Delete();
  // Phew...we've defined loop, now want to do a fill so we can extract the
  // inside from the outside. Depending on GenerateSelectionScalars flag,
  // we either set the distance of the points to the selection loop as
  // zero (GenerateSelectionScalarsOff) or we evaluate the distance of these
  // points to the lines. (Later we'll use a connected traversal to compute
  // the distances to the remaining points.)

  // Next, prepare to mark off inside/outside and on boundary of loop.
  // Mark the boundary of the loop using point marks. Also initialize
  // the advancing front (used to mark traversal/compute scalars).
  // Prepare to compute the advancing front
  vtkIntArray *cellMarks = vtkIntArray::New();
  cellMarks->SetNumberOfValues(numCells);
  for (i=0; i<numCells; i++)  //mark unvisited
    {
    cellMarks->SetValue(i,VTK_LARGE_INTEGER);
    }
  vtkIntArray *pointMarks = vtkIntArray::New();
  pointMarks->SetNumberOfValues(numPts);
  for (i=0; i<numPts; i++)  //mark unvisited
    {
    pointMarks->SetValue(i,VTK_LARGE_INTEGER);
    }

  vtkIdList *currentFront = vtkIdList::New(), *tmpFront;
  vtkIdList *nextFront = vtkIdList::New();
  for (i=0; i<numMeshLoopPts; i++)
    {
    id = edgeIds->GetId(i);
    pointMarks->SetValue(id, 0); //marks the start of the front
    currentFront->InsertNextId(id);
    }
  // Traverse the front as long as we can. We're basically computing a 
  // topological distance.
  int maxFront=0;
  vtkIdType maxFrontCell=(-1);
  int currentFrontNumber=1, numPtsInFront;
  while ( (numPtsInFront = currentFront->GetNumberOfIds()) )
	{
    for (i=0; i < numPtsInFront; i++)
      {
      id = currentFront->GetId(i);
      this->Mesh->GetPointCells(id, ncells, cells);
      for (j=0; j<ncells; j++)
        {
        id = cells[j];
        if ( cellMarks->GetValue(id) == VTK_LARGE_INTEGER )
          {
          if ( currentFrontNumber > maxFront )
            {
            maxFrontCell = id;
            }
          cellMarks->SetValue(id, currentFrontNumber);
          this->Mesh->GetCellPoints(id,npts,pts);
          for (k=0; k<npts; k++)
            {
            if ( pointMarks->GetValue(pts[k]) == VTK_LARGE_INTEGER )
              {
              pointMarks->SetValue(pts[k], 1);
              nextFront->InsertNextId(pts[k]);
              }
            }
          }
        } //for cells surrounding point
      } //all points in front
    currentFrontNumber++;
    tmpFront = currentFront;
    currentFront = nextFront;
    nextFront = tmpFront;
    nextFront->Reset();
    } //while still advancing

  // Okay, now one of the regions is filled with negative values. This fill
  // operation assumes that everthing is connected.
  if ( this->SelectionMode == VTK_INSIDE_CLOSEST_POINT_REGION )
    {// find closest point and use as a seed
    for (closestDist2=VTK_DOUBLE_MAX, j=0; j < numPts; j++)
      {
      inPts->GetPoint(j,x);

      dist2 = vtkMath::Distance2BetweenPoints(x, this->ClosestPoint);
      // get closest point not on the boundary
      if ( dist2 < closestDist2 && pointMarks->GetValue(j) != 0 )
        {
        closest = j;
        closestDist2 = dist2;
        }
      } //for all input points
    this->Mesh->GetPointCells(closest, ncells, cells);
    }
  // We do the fill as a moving front. This is an alternative to recursion. The
  // fill negates one region of the mesh on one side of the loop.
  currentFront->Reset(); nextFront->Reset();
  currentFront->InsertNextId(maxFrontCell);
  vtkIdType numCellsInFront;
  
  cellMarks->SetValue(maxFrontCell,-1);

  while ( (numCellsInFront = currentFront->GetNumberOfIds()) > 0 )
    {
    for (i=0; i < numCellsInFront; i++)
      {
      id = currentFront->GetId(i);

      this->Mesh->GetCellPoints(id, npts, pts);
      for (j=0; j<3; j++)
        {
        pt1 = pts[j];
        pt2 = pts[(j+1)%3];
        s1 = pointMarks->GetValue(pt1);
        s2 = pointMarks->GetValue(pt2);

        if ( s1 != 0 )
          {
          pointMarks->SetValue(pt1, -1);
          }

        if ( ! (s1 == 0 && s2 == 0) )
          {
          this->Mesh->GetCellEdgeNeighbors(id, pt1, pt2, neighbors);
          numNei = neighbors->GetNumberOfIds();
          for (k=0; k<numNei; k++)
            {
            neiId = neighbors->GetId(k);
            val = cellMarks->GetValue(neiId);
            if ( val != -1 ) //-1 is what we're filling with
              {
              cellMarks->SetValue(neiId,-1);
              nextFront->InsertNextId(neiId);
              }
            }
          }//if can cross boundary
        }//for all edges of cell
      } //all cells in front

    tmpFront = currentFront;
    currentFront = nextFront;
    nextFront = tmpFront;
    nextFront->Reset();
    } //while still advancing
 
  // Now may have to invert fill value depending on what we wan to extract
  if ( this->SelectionMode == VTK_INSIDE_SMALLEST_REGION )
    {
    for (i=0; i < numCells; i++)
      {
      mark = cellMarks->GetValue(i);
      cellMarks->SetValue(i, -mark);
      }
    for (i=0; i < numPts; i++)
      {
      mark = pointMarks->GetValue(i);
      pointMarks->SetValue(i, -mark);
      }
    }

  // If generating selection scalars, we now have to modify the scalars to
  // approximate a distance function. Otherwise, we can create the output.
  if ( ! this->GenerateSelectionScalars )
    {//spit out all the negative cells
    vtkCellArray *newPolys=vtkCellArray::New();
    newPolys->Allocate(numCells/2, numCells/2);
    for (i=0; i< numCells; i++)
      {
      if ( (cellMarks->GetValue(i) < 0) || 
      (cellMarks->GetValue(i) > 0 && this->InsideOut) )
        {
        this->Mesh->GetCellPoints(i, npts, pts);
        newPolys->InsertNextCell(npts,pts);
        }
      }
    output->SetPoints(inPts);
    output->SetPolys(newPolys);
    outPD->PassData(inPD);
    newPolys->Delete();

    if ( this->GenerateUnselectedOutput )
      {
      vtkCellArray *unPolys=vtkCellArray::New();
      unPolys->Allocate(numCells/2, numCells/2);
      for (i=0; i< numCells; i++)
        {
        if ( (cellMarks->GetValue(i) >= 0) || 
        (cellMarks->GetValue(i) < 0 && this->InsideOut) )
          {
          this->Mesh->GetCellPoints(i, npts, pts);
          unPolys->InsertNextCell(npts,pts);
          }
        }
      this->GetUnselectedOutput()->SetPoints(inPts);
      this->GetUnselectedOutput()->SetPolys(unPolys);
      this->GetUnselectedOutput()->GetPointData()->PassData(inPD);
      unPolys->Delete();
      }

    }
  else //modify scalars to generate selection scalars
    {
    vtkFloatArray *selectionScalars=vtkFloatArray::New();
    selectionScalars->SetNumberOfTuples(numPts);
    
    // compute distance to lines. Really this should be computed based on
    // the connected fill distance.
    for (j=0; j < numPts; j++) //compute minimum distance to loop
      {
      if ( pointMarks->GetValue(j) != 0 )
        {
        inPts->GetPoint(j,x);
        for ( closestDist2=VTK_DOUBLE_MAX, i=0; i < numLoopPts; i++ )
          {
          Loop->GetPoint(i, x0);
          Loop->GetPoint((i+1)%numLoopPts, x1);
          dist2 = vtkLine::DistanceToLine(x, x0, x1, t, xLoop);
          if ( dist2 < closestDist2 )
            {
            closestDist2 = dist2;
            }
          }//for all loop edges
          closestDist2 = sqrt((double)closestDist2);
          selectionScalars->SetComponent(j,0,
                                         closestDist2*pointMarks->GetValue(j));
        }
      }

    // Now, determine the sign of those points "on the boundary" to give a 
    // better approximation to the scalar field.
    for (j=0; j < numMeshLoopPts; j++)
      {
      id = edgeIds->GetId(j);
      inPts->GetPoint(id, x);
      for ( closestDist2=VTK_DOUBLE_MAX, i=0; i < numLoopPts; i++ )
        {
        Loop->GetPoint(i, x0);
        Loop->GetPoint((i+1)%numLoopPts, x1);
        dist2 = vtkLine::DistanceToLine(x, x0, x1, t, xLoop);
        if ( dist2 < closestDist2 )
          {
          closestDist2 = dist2;
          neiX[0] = xLoop[0]; neiX[1] = xLoop[1]; neiX[2] = xLoop[2];
          }
        }//for all loop edges
      closestDist2 = sqrt((double)closestDist2);

      // find neighbor not on boundary and compare negative/positive values
      // to see what makes the most sense
      this->GetPointNeighbors(id, neighbors);
      numNei = neighbors->GetNumberOfIds();
      for (dist2=0.0, i=0; i<numNei; i++)
        {
        neiId = neighbors->GetId(i);
        if ( pointMarks->GetValue(neiId) != 0 ) //find the furthest away
          {
          if ( fabs(selectionScalars->GetComponent(neiId,0)) > dist2 )
            {
            currentId = neiId;
            dist2 = fabs(selectionScalars->GetComponent(neiId,0));
            }
          }
        }

      inPts->GetPoint(currentId, x0);
      if ( vtkMath::Distance2BetweenPoints(x0,x) < 
           vtkMath::Distance2BetweenPoints(x0,neiX) )
        {
        closestDist2 = closestDist2 * pointMarks->GetValue(currentId);
        }
      else
        {
        closestDist2 = -closestDist2 * pointMarks->GetValue(currentId);
        }

      selectionScalars->SetComponent(id,0,closestDist2);
      }//for all boundary points

    output->CopyStructure(this->Mesh); //pass geometry/topology unchanged
    int idx = outPD->AddArray(selectionScalars);
    outPD->SetActiveAttribute(idx, vtkDataSetAttributes::SCALARS);
    outPD->CopyScalarsOff();
    outPD->PassData(inPD);
    outCD->PassData(inCD);
    selectionScalars->Delete();
    }

  // Clean up and update output
  triMesh->UnRegister(this);

  this->Mesh->Delete();

  neighbors->Delete();

 // tmp_edgeIds->Delete();
  edgeIds->Delete();

  loopIds->Delete();

  cellMarks->Delete();

  pointMarks->Delete();

  currentFront->Delete();

  //nextFront->Delete();

  return 1;
}

//
//void SelectPolyData::InputPic(Pic & pic)
//{
//	unsigned short ncells;
//	int i, j, k;
//	vtkIdType *cells, *pts, npts;
//
//	int n = this->Mesh->GetPoints()->GetNumberOfPoints();
//	pic.neibours.resize(2*n);
//	pic.position.resize(2*n);
//
//	this->Mesh->GetPointCells(ptId, ncells, cells);
//	for (i=0; i<ncells; i++)
//    {
//		this->Mesh->GetCellPoints(cells[i], npts, pts);
//		for (j=0; j<3; j++)
//		{
//			for(k = 0; k < 3; k ++)
//			{
//				if(j != k){
//					pic.neibours[pts[j]].push_back(pts[k]);
//				}
//			}
//		}
//	}
//
//	for (i=0 ; i < n; i++)
//	{
//		pic.postion[i] = this->Mesh->GetPoint(i);
//	}
//
//}


//----------------------------------------------------------------------------
void SelectPolyData::GetPointNeighbors (vtkIdType ptId, vtkIdList *nei)
{
  unsigned short ncells;
  int i, j;
  vtkIdType *cells, *pts, npts;
  
  nei->Reset();
  this->Mesh->GetPointCells(ptId, ncells, cells);
  for (i=0; i<ncells; i++)
    {
    this->Mesh->GetCellPoints(cells[i], npts, pts);
    for (j=0; j<3; j++)
      {
      if ( pts[j] != ptId )
        {
        nei->InsertUniqueId(pts[j]);
        }
      }
    }
}

//----------------------------------------------------------------------------
vtkIdType SelectPolyData::FindClosestVertex(vtkPolyData*inputMesh, double point[3])
{

	//Project the point onto inputMesh
		//vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	    vtkSmartPointer<vtkCellLocator> cellLocator=vtkSmartPointer<vtkCellLocator>::New();
		cellLocator->SetDataSet(inputMesh);
		cellLocator->BuildLocator();

		double closestPoint[3];				//the coordinates of the closest point will be returned here
		double closestPointDist2;			//the squared distance to the closest point will be returned here
		vtkIdType cellId;							//the cell id of the cell containing the closest point will be returned here
		int subId;										//this is rarely used (in triangle strips only, I believe)
		cellLocator->FindClosestPoint(point, closestPoint, cellId, subId, closestPointDist2);
	
		//vtkSmartPointer<vtkIdList>ptIds = vtkSmartPointer<vtkIdList>::New();
		vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
		inputMesh->GetCellPoints(cellId,ptIds);
		int npts=ptIds->GetNumberOfIds();
		double CellPt[3], dist2;
		vtkIdType ClosestPtId = -1;
		double closestDist2 = VTK_DOUBLE_MAX;
		for(int i=0;i<npts;i++)
		{
			inputMesh->GetPoint(ptIds->GetId(i),CellPt);
			dist2 = vtkMath::Distance2BetweenPoints(point, CellPt);
			if ( dist2 < closestDist2 )
			{
				ClosestPtId = ptIds->GetId(i);
				closestDist2 = dist2;
			}
		}
		if(closestPointDist2==-1)
		{
			return 0;
		}
		else
		{
			return ClosestPtId;
		}

		//cellLocator->Delete();
		//ptIds->Delete();
		
}
//----------------------------------------------------------------------------
unsigned long int SelectPolyData::GetMTime()
{
  unsigned long mTime=this->Superclass::GetMTime();
  unsigned long time;

  if ( this->Loop != NULL )
    {
    time = this->Loop->GetMTime();
    mTime = ( time > mTime ? time : mTime );
    }

  return mTime;
}

//----------------------------------------------------------------------------
void SelectPolyData::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Generate Unselected Output: " 
     << (this->GenerateUnselectedOutput ? "On\n" : "Off\n");

  os << indent << "Inside Mode: ";
  os << this->GetSelectionModeAsString() << "\n";

  os << indent << "Closest Point: (" << this->ClosestPoint[0] << ", " 
     << this->ClosestPoint[1] << ", " << this->ClosestPoint[2] << ")\n";

  os << indent << "Generate Selection Scalars: " 
     << (this->GenerateSelectionScalars ? "On\n" : "Off\n");

  os << indent << "Inside Out: " << (this->InsideOut ? "On\n" : "Off\n");

  if ( this->Loop )
    {
    os << indent << "Loop of " << this->Loop->GetNumberOfPoints()
       << "points defined\n";
    }
  else
    {
    os << indent << "Loop not defined\n";
    }
}

