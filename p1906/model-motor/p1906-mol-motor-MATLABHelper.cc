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

/* \details This class writes data in MATLAB format
 *
 * <pre>
 * Creates ASCII files designed for import into MATLAB
 * +-----------+                      +--------+
 * |           |     +---------+      |        |
 * |   NS-3    | +-> | *.dat   |  +-> | MATLAB |
 * |           |     +---------+      |        |
 * +-----------+                      +--------+
 * </pre>
 */
 
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/p1906-mol-motor-MATLABHelper.h"
#include "ns3/p1906-mol-motor-tube.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_MATLABHelper");

TypeId P1906MOL_MOTOR_MATLABHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_MATLABHelper")
    .SetParent<Object> ();
  return tid;
}

P1906MOL_MOTOR_MATLABHelper::P1906MOL_MOTOR_MATLABHelper ()
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

//! write a list of vectors into file fname in MATLAB loadable format
void P1906MOL_MOTOR_MATLABHelper::vectorFieldPlotMATLAB(gsl_matrix * vf, const char * fname)
{
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  for (size_t i = 0; i < vf->size1; i++)
  {
    fprintf (pFile, "%f %f %f %f %f %f\n", 
	  gsl_matrix_get (vf, i, 0),
	  gsl_matrix_get (vf, i, 1),
	  gsl_matrix_get (vf, i, 2),
	  gsl_matrix_get (vf, i, 3),
	  gsl_matrix_get (vf, i, 4),
	  gsl_matrix_get (vf, i, 5));
  }
  
  fclose(pFile);
}

//! write the vector field in MATLAB format using regular spacing between samples in the file fname
//! The result of this file is loaded into Mathematica and the vector field is reconstructed from the samples via interpolation
//! Then the vector field operators are applied - see bushsf@research.ge.com for results
void P1906MOL_MOTOR_MATLABHelper::vectorFieldMeshMATLAB(gsl_matrix * vf, const char * fname)
{
  FILE * pFile;

  pFile = fopen (fname,"w");
  
  // find the mesh volume limits
  double xMin, xMax;
  double yMin, yMax;
  double zMin, zMax;
  double uMin, uMax;
  double vMin, vMax;
  double wMin, wMax;
  
  gsl_vector * x = gsl_vector_alloc(vf->size1);
  gsl_vector * y = gsl_vector_alloc(vf->size1);
  gsl_vector * z = gsl_vector_alloc(vf->size1);
  gsl_vector * u = gsl_vector_alloc(vf->size1);
  gsl_vector * v = gsl_vector_alloc(vf->size1);
  gsl_vector * w = gsl_vector_alloc(vf->size1);
  
  //! extract the location and vector components from the field
  gsl_matrix_get_col (x, vf, 0);
  gsl_matrix_get_col (y, vf, 1);
  gsl_matrix_get_col (z, vf, 2);
  gsl_matrix_get_col (u, vf, 3);
  gsl_matrix_get_col (v, vf, 4);
  gsl_matrix_get_col (w, vf, 5);
  
  //! find the min and max values for each component
  gsl_vector_minmax (x, &xMin, &xMax);
  gsl_vector_minmax (y, &yMin, &yMax);
  gsl_vector_minmax (z, &zMin, &zMax);
  gsl_vector_minmax (u, &uMin, &uMax);
  gsl_vector_minmax (v, &vMin, &vMax);
  gsl_vector_minmax (w, &wMin, &wMax);
  
  //printf ("xMin: %f yMin: %f zMin: %f uMin: %f vMin: %f wMin: %f\n", xMin, yMin, zMin, vMin, uMin, wMin);
  //printf ("xMax: %f yMax: %f zMax: %f uMax: %f vMax: %f wMax: %f\n", xMax, yMax, zMax, vMax, uMax, wMax);
	
  gsl_vector * pt1 = gsl_vector_alloc (3);
  gsl_vector * pt2 = gsl_vector_alloc (3);
  gsl_vector * vec = gsl_vector_alloc (3);
  gsl_vector * closest = gsl_vector_alloc (6);
  
  double xStepsize = (xMax - xMin) / 10.0;
  double yStepsize = (yMax - yMin) / 10.0;
  double zStepsize = (zMax - zMin) / 10.0;
  
  //printf ("xStepsize: %f yStepsize: %f zStepsize: %f\n", xStepsize, yStepsize, zStepsize);
  
  //! step through equidistant points in a volume and store the vector values at each point
  for (double i = xMin; i < xMax; i += xStepsize)
    for (double j = yMin; j < yMax; j += yStepsize)
	  for (double k = zMin; k < zMax; k += zStepsize)
      {
	    //! find the closest point to the current location
	    P1906MOL_MOTOR_Field::point (pt1, i, j, k);
		// printf ("pt1\n");
		// displayPoint (pt1);
	    P1906MOL_MOTOR_Field::findClosestPoint (pt1, vf, closest);
		//printf ("(findClosestPoint) closest vector: %f %f %f %f %f %f\n",
        //  gsl_vector_get (closest, 0),
	    //  gsl_vector_get (closest, 1),
	    //  gsl_vector_get (closest, 2),
	    //  gsl_vector_get (closest, 3),
	    //  gsl_vector_get (closest, 4),
	    //  gsl_vector_get (closest, 5));
		P1906MOL_MOTOR_Field::point (pt2, 
		  gsl_vector_get (closest, 0),
		  gsl_vector_get (closest, 1),
		  gsl_vector_get (closest, 2));
		  
		// printf ("(vectorFieldMeshMATLAB) distance between pt and vector location: %f\n", distance (pt1, pt2));
		//! check if distance within range
		if (P1906MOL_MOTOR_Field::distance(pt1, pt2) > 2.0 * xStepsize)
		{
		  //! if not, store the null vector
		  // printf ("(vectorFieldMeshMATLAB) using null vector\n");
          P1906MOL_MOTOR_Field::point (vec, 0.0, 0.0, 0.0);
		  // displayPoint (vec);
	    } 
		else 
		{
		  // printf ("(vectorFieldMeshMATLAB) using vector\n");
		  //! otherwise, store the vector value
		  P1906MOL_MOTOR_Field::point (vec,
	        gsl_vector_get (closest, 3),
	        gsl_vector_get (closest, 4),
	        gsl_vector_get (closest, 5));
		  // displayPoint (vec);
		}
		//! print current location and stored vector value
        fprintf (pFile, "%f %f %f %f %f %f\n",
	      i, 
		  j,
		  k, 
		  gsl_vector_get (vec, 0),
          gsl_vector_get (vec, 1),
          gsl_vector_get (vec, 2));
      }
	  
  fclose(pFile);
}

P1906MOL_MOTOR_MATLABHelper::~P1906MOL_MOTOR_MATLABHelper ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
