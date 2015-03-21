/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
 *
 *  This source file is an essential part of IEEE Std 1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE Std 1906.1 may also be made and
 *  distributed. All other uses require permission from the IEEE
 *  Standards Department (stds-ipr@ieee.org). All other rights
 *  reserved.
 *
 *  This source file is provided on an AS IS basis.
 *  The IEEE disclaims ANY WARRANTY EXPRESS OR IMPLIED INCLUDING
 *  ANY WARRANTY OF MERCHANTABILITY AND FITNESS FOR USE FOR A
 *  PARTICULAR PURPOSE.
 *  The user of the source file shall indemnify and hold
 *  IEEE harmless from any damages or liability arising out of
 *  the use thereof.
 *
 * Author: Stephen F Bush - GE Global Research
 *                      bushsf@research.ge.com
 *                      http://www.amazon.com/author/stephenbush
 */
 
/* \details This class implements methods that write data in Mathematica format.
 *
 * <pre>
 * Create ASCII files designed for import into Mathematica
 * +---------+                       +-------------+
 * |         |      +----------+     |             |
 * |   NS-3  +--->  |  *.mma   +---> | Mathematica |
 * |         |      +----------+     |             |
 * +---------+                       +-------------+
 * </pre>
 */

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/p1906-mol-motor-MathematicaHelper.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_MathematicaHelper");

TypeId P1906MOL_MOTOR_MathematicaHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_MathematicaHelper")
    .SetParent<Object> ();
  return tid;
}

P1906MOL_MOTOR_MathematicaHelper::P1906MOL_MOTOR_MathematicaHelper ()
{
  /** This class implements persistence length as described in:
	  Bush, S. F., & Goel, S. (2013). Persistence Length as a Metric for Modeling and 
	    Simulation of Nanoscale Communication Networks, 31(12), 815-824. 
		http://dx.doi.org/10.1109/JSAC.2013.SUP2.12130014.
		
    The Gnu Scientific Library (GSL) is required for these methods.
    All points and positions are in three dimensions comprised of a gsl_vector * of length three (x, y, z).
    All lines and segments are comprised of two points within a gsl_vector * of length size (x1, y1, z1), (x2, y2, z2).
    Lists of points or positions are in a gsl_matrix * of size n x 3.
    Each tube is comprised of a list of segments within a gsl_matrix * of size s x 6 -> s x ((x1, y1, z1), (x2, y2, z2)).
    A set of tubes is also a gsl_matrix * of size (s * t) x 6, where s is the number of segments and t the number of tubes.
    All random number are derived from gsl_rng *.
	  
  */

}

//! display the vector field in Mathematica format for VectorPlot3D in file fname
void P1906MOL_MOTOR_MathematicaHelper::vectorFieldPlotMma(gsl_matrix * vf, const char * fname)
{
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  fprintf (pFile, "ListVectorPlot3D[{");
  for (size_t i = 0; i < vf->size1; i++)
  {
    fprintf (pFile, "{{%lf, %lf, %lf}, {%lf, %lf, %lf}}", 
	  gsl_matrix_get (vf, i, 0),
	  gsl_matrix_get (vf, i, 1),
	  gsl_matrix_get (vf, i, 2),
	  gsl_matrix_get (vf, i, 3),
	  gsl_matrix_get (vf, i, 4),
	  gsl_matrix_get (vf, i, 5));
    if (i < (vf->size1 - 1)) fprintf (pFile, ", ");
  }
  //! fprintf (pFile, "}, ");
  //! fprintf (pFile, ", PlotStyle -> {Dashed, Thick, Red}");
  fprintf (pFile, "}]\n");
  //! option to print vertex labels
  //! fprintf (pFile, "}, VertexLabeling -> True]\n");
  
  fclose(pFile);
}

//! write the vector field in Mathematica format using regular spacing between samples in file fname
void P1906MOL_MOTOR_MathematicaHelper::vectorFieldMeshMma(gsl_matrix * vf, const char * fname)
{
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  fprintf (pFile, "ListVectorPlot3D[{");
  for (size_t i = 0; i < vf->size1; i++)
  {
    fprintf (pFile, "{{%f, %f, %f}, {%f, %f, %f}}", 
	  gsl_matrix_get (vf, i, 0),
	  gsl_matrix_get (vf, i, 1),
	  gsl_matrix_get (vf, i, 2),
	  gsl_matrix_get (vf, i, 3),
	  gsl_matrix_get (vf, i, 4),
	  gsl_matrix_get (vf, i, 5));
    if (i < (vf->size1 - 1)) fprintf (pFile, ", ");
  }
  //! fprintf (pFile, "}, ");
  //! fprintf (pFile, ", PlotStyle -> {Dashed, Thick, Red}");
  fprintf (pFile, "}]\n");
  //! option to print vertex labels
  //! fprintf (pFile, "}, VertexLabeling -> True]\n");
  
  fclose(pFile);
}

//! write the vector field in Mathematica format using regular spacing between samples in file fname
void P1906MOL_MOTOR_MathematicaHelper::vectorPlotMma(gsl_matrix * vf, const char * fname)
{
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  fprintf (pFile, "Graphics3D[{");
  for (size_t i = 0; i < vf->size1; i++)
  {
    //Graphics3D[{Arrow[{{0, 0, 1}, {1, 1, 1}}], Arrow[{{0, 0, 0}, {-1, 1, 1}}]}, Axes -> True]
    fprintf (pFile, "Arrow[{{%lf, %lf, %lf}, {%lf, %lf, %lf}}]", 
	  gsl_matrix_get (vf, i, 0),
	  gsl_matrix_get (vf, i, 1),
	  gsl_matrix_get (vf, i, 2),
	  gsl_matrix_get (vf, i, 3),
	  gsl_matrix_get (vf, i, 4),
	  gsl_matrix_get (vf, i, 5));
    if (i < (vf->size1 - 1)) fprintf (pFile, ", ");
  }
  fprintf (pFile, "}, Axes -> True]\n");
  
  fclose(pFile);
}

//! print the points (vertices) in pts in Mathematica format into file fname and include edges between the vertices
void P1906MOL_MOTOR_MathematicaHelper::connectedPoints2Mma(vector<P1906MOL_MOTOR_Pos> pts, const char * fname)
{
  FILE * pFile;
  double x, y, z;
  
  pFile = fopen (fname,"w");
  size_t pt = 1;
  
  fprintf (pFile, "GraphPlot3D[{");
  for (size_t i = 0; i < pts.size(); i++)
  {
    fprintf (pFile, "%ld -> ", pt);
	pt++;
	fprintf (pFile, "%ld", pt);
    if (i < (pts.size() - 1)) fprintf (pFile, ", ");
  }
  fprintf (pFile, "}, ");
  pt = 1;
  fprintf (pFile, "VertexCoordinateRules ->{");
  for (size_t i = 0; i < pts.size(); i++)
  {
	pts.at(i).getPos (&x, &y, &z);
    fprintf (pFile, "%ld -> {%f, %f, %f}", pt, x, y, z);
    pt++;
	if (i < (pts.size() - 1)) fprintf (pFile, ", ");
  }
  fprintf (pFile, "}");
  fprintf (pFile, ", PlotStyle -> {Dashed, Thick, Red}");
  fprintf (pFile, "]\n");
  //! option to print vertex labels
  //! fprintf (pFile, "}, VertexLabeling -> True]\n");
  
  fclose(pFile);
}

//! print the first numPts points pts in Mathematica format in file fname
void P1906MOL_MOTOR_MathematicaHelper::points2Mma(vector<P1906MOL_MOTOR_Pos> & pts, const char * fname)
{
  FILE * pFile;
  P1906MOL_MOTOR_Pos pt;
  double x, y, z;
  
  pFile = fopen (fname,"w");
  
  fprintf (pFile, "Graphics3D[{PointSize[Large], Blue, ");
  for (size_t i = 0; i < pts.size(); i++)
  {
	pt = pts.at(i);
	pt.getPos(&x, &y, &z);
    fprintf (pFile, "Point[{%f, %f, %f}]", x, y, z);
	if (i < pts.size() - 1) fprintf (pFile, ", ");
  }
  fprintf (pFile, "}]\n");
  
  fclose(pFile);
}

//! display the volume surface in Mathematica format into file fname
void P1906MOL_MOTOR_MathematicaHelper::volSurfacePlot(P1906MOL_MOTOR_Pos center, double radius, const char * fname)
{
  FILE * pFile;
  double x, y, z;
  
  pFile = fopen (fname,"w");
  
  center.getPos (&x, &y, &z);
  fprintf (pFile, "Graphics3D[{Opacity[0.5], Sphere[{%lf, %lf, %lf}, %lf]}, Axes -> True]\n", x, y, z, radius);
    
  fclose(pFile);
}

//! print a plot of x,y values in vals in Mathematica format into file fname
void P1906MOL_MOTOR_MathematicaHelper::plot2Mma(gsl_matrix * vals, const char * fname, const char * xlabel, const char * ylabel)
{
  FILE * pFile;
  size_t numVals = vals->size1;
  
  pFile = fopen (fname,"w");
  
  fprintf (pFile, "ListLinePlot[{");
  for (size_t i = 0; i < numVals; i++)
  {
    fprintf (pFile, "{%f, %f}", 
	  gsl_matrix_get(vals, i, 0), 
	  gsl_matrix_get(vals, i, 1));
    if (i < (numVals - 1)) fprintf (pFile, ", ");
  }
  fprintf (pFile, "}");
  fprintf (pFile, ", AxesLabel -> {\"%s\", \"%s\"}, GridLines -> Automatic", xlabel, ylabel);
  fprintf (pFile, "]\n");
  
  fclose(pFile);
}

//! print all tubes in tubeMatrix into a Mathematica file fname with segments per tube of segPerTube
void P1906MOL_MOTOR_MathematicaHelper::tubes2Mma(gsl_matrix * tubeMatrix, size_t segPerTube, const char * fname)
{
  /** save tubes to file in the form of 
    GraphPlot3D[{1 -> 2, 1 -> 4, 1 -> 5, 2 -> 3, 2 -> 6, 3 -> 4, 3 -> 7, 4 -> 8, 5 -> 6, 5 -> 8, 6 -> 7, 7 -> 8}, 
	VertexCoordinateRules -> {1 -> {0, 1, 2}, 2 -> {-1, 0, 2}, 3 -> {0, -1, 2}, 4 -> {1, 0, 2}, 5 -> {0, 2, 0}, 6 -> {-2, 0, 0}, 7 -> {0, -2, 0}, 8 -> {2, 0, 0}}]
  */
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  size_t numSegments = tubeMatrix->size1;
  size_t pt = 1;
  size_t numTubes = numSegments / segPerTube;
  
  //printf ("(tubes2Mma) tubeMatrix: %ld x %ld\n", tubeMatrix->size1, tubeMatrix->size2);
  //printf ("(tubes2Mma) numTubes: %ld segPerTube: %ld\n", numTubes, segPerTube);
  //gsl_matrix_fprintf (stdout, tubeMatrix, "%f");
  
  fprintf (pFile, "GraphPlot3D[{");
  for (size_t i = 0; i < numTubes; i++)
  {
    for (size_t j = 0; j < segPerTube; j++)
	{
      fprintf (pFile, "%ld -> ", pt);
	  pt++;
	  fprintf (pFile, "%ld", pt);
      if ((i < (numTubes - 1)) || (j < (segPerTube - 1))) fprintf (pFile, ", ");
	}
    pt++;
  }
  fprintf (pFile, "}, ");
  pt = 1;
  fprintf (pFile, "VertexCoordinateRules ->{");
    for (size_t i = 0; i < numTubes; i++)
	{
      for (size_t j = 0; j < segPerTube; j++)
	  {
	    if (j == 0) //! only print the ends after the first one
	    {
		  //printf ("(tubes2Mma) i: %ld j: %ld index: %ld pos: %f %f %f\n", 
		  //  i, 
		  //  j, 
		  //  i * segPerTube + j,
		  //  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 0),
		  //  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 1),
		  //  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 2));
			
          fprintf (pFile, "%ld -> {%lf, %lf, %lf}, ",
		    pt,
	        gsl_matrix_get(tubeMatrix, i * segPerTube + j, 0),
		    gsl_matrix_get(tubeMatrix, i * segPerTube + j, 1),
		    gsl_matrix_get(tubeMatrix, i * segPerTube + j, 2));
		  pt++;
        }
		//printf ("(tubes2Mma) i: %ld j: %ld index: %ld pos: %f %f %f\n", 
		//  i, 
		//  j, 
		//  i * segPerTube + j,
		//  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 3),
		//  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 4),
		//  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 5));
		  
		fprintf (pFile, "%ld -> {%lf, %lf, %lf}", 
		  pt,
		  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 3),
		  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 4),
		  gsl_matrix_get(tubeMatrix, i * segPerTube + j, 5));
		pt++;
	    if ((i < (numTubes - 1)) || (j < (segPerTube - 1))) fprintf (pFile, ", ");
	  }
	}
  fprintf (pFile, "}]\n");
  //! option to print vertex labels
  //! fprintf (pFile, "}, VertexLabeling -> True]\n");
  
  fclose (pFile);
}

P1906MOL_MOTOR_MathematicaHelper::~P1906MOL_MOTOR_MathematicaHelper ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
