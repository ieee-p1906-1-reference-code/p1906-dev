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
 
//! \details This class extends the IEEE 1906 Field component to define a vector field.
//! A vector field defines a vector for every point in three-dimensional space. In this case,
//! we construct a vector field based upon tube-like structures that impact some type of flow.
//! 
//! Note that there two similar structures that should not be confused: line segments and vectors
//!
//! <pre>
//!                     VECTOR
//!                ---------------->
//! 			   ^               ^
//!                |               |
//! gsl_vector * location  gsl_vector * vector 
//!            (x, y, z)      (vx, vy, vz)
//!
//!                           LINE SEGMENT
//!                         -----------------
//! 				        ^               ^
//!                         |               |
//! gsl_vector *segment (x1,y1,z1,       x2,y2,z2)
//!
//! </pre>

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/p1906-mol-motor.h"
#include "ns3/p1906-mol-field.h"
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-MathematicaHelper.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_Field");

TypeId P1906MOL_MOTOR_Field::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_Field")
    .SetParent<P1906MOLField> ();
  return tid;
}

P1906MOL_MOTOR_Field::P1906MOL_MOTOR_Field ()
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
    All random number are derived from gsl_rng *  
  */
  
}

//! set point pt with x, y, and z coordinates
void P1906MOL_MOTOR_Field::point(gsl_vector * pt, double x, double y, double z)
{
  if (pt->size != 3)
    printf ("(point) warning: point vector incorrect length: %ld\n", pt->size);
	
  gsl_vector_set (pt, 0, x);
  gsl_vector_set (pt, 1, y);
  gsl_vector_set (pt, 2, z);
}

//! set line with end points pt1 and pt2
//! <pre>
//! gsl_vector *segment (x1,y1,z1,       x2,y2,z2)
//!                         +               +                       
//!                         v               v                                              
//!                         +---------------+
//! 				           LINE SEGMENT
//! </pre>
void P1906MOL_MOTOR_Field::line(gsl_vector * line, gsl_vector * pt1, gsl_vector * pt2)
{
  if (pt1->size != 3)
    printf ("(line) warning: point vector incorrect length: %ld\n", pt1->size);

  if (pt2->size != 3)
    printf ("(line) warning: point vector incorrect length: %ld\n", pt2->size);

  if (line->size != 6)
    printf ("(line) warning: line vector incorrect length: %ld\n", line->size);
	
  for (int i = 0; i < 3; i++)
    gsl_vector_set (line, i, gsl_vector_get (pt1, i));

  for (int i = 0; i < 3; i++)
    gsl_vector_set (line, i + 3, gsl_vector_get (pt2, i));
}

//! return the segment at location mp from within the tubeMatrix
//! <pre>
//! segments stored sequentially in tubeMatrix
//! segment ---+
//!            | 
//!            v                                 
//!          1 . . .             segPerTube    
//!          +----+ +----+ ... +----+            
//!                                              
//!          +----+ +----+ ... +----+            
//!                                              
//!           ...   ...    ...  ...              
//!                              N * segPerTube
//!          +----+ +----+ ... +----+            
//!                                              
//!         <----------------------->            
//!           tube (rows of segments)
//! </pre>
void P1906MOL_MOTOR_Field::line(gsl_vector * segment, gsl_matrix * tubeMatrix, size_t mp)
{
  if (mp > tubeMatrix->size1)
    printf ("(line) warning: mp %ld is larger than matrix %ld", mp, tubeMatrix->size1);

  for (size_t i = 0; i < 6; i++)
    gsl_vector_set (segment, i, gsl_matrix_get( tubeMatrix, mp, i ));
}

//! set the segment at location mp in the line matrix with pt1 and pt2
void P1906MOL_MOTOR_Field::line(gsl_matrix * line, size_t mp, gsl_vector * pt1, gsl_vector * pt2)
{
  if (mp > line->size1)
    printf ("(line)warning: mp %ld is larger than matrix %ld", mp, line->size1);

  for (int i = 0; i < 3; i++)
    gsl_matrix_set (line, mp, i, gsl_vector_get (pt1, i));

  for (int i = 0; i < 3; i++)
    gsl_matrix_set (line, mp, i + 3, gsl_vector_get (pt2, i));
}

//! set the segment with the given end points
void P1906MOL_MOTOR_Field::line(gsl_vector * line, P1906MOL_MOTOR_Pos p1, P1906MOL_MOTOR_Pos p2)
{
  gsl_vector * p = gsl_vector_alloc (3);
  
  p1.getPos (p);
  for (size_t i = 0; i < 3; i++)
    gsl_vector_set (line, i, gsl_vector_get (p, i));
	
  p2.getPos (p);
  for (size_t i = 3; i < 6; i++)
    gsl_vector_set (line, i, gsl_vector_get (p, i - 3));
}

//! display the segment
void P1906MOL_MOTOR_Field::displayLine(gsl_vector * line)
{
  if (line->size != 6)
  {
    printf ("(displayLine) invalid size: %ld\n", line->size);
  }
  
  printf ("(displayLine) pt1: %lf, %lf, %lf\n", gsl_vector_get (line, 0), gsl_vector_get (line, 1), gsl_vector_get (line, 2));
  printf ("(displayLine) pt2: %lf, %lf, %lf\n", gsl_vector_get (line, 3), gsl_vector_get (line, 4), gsl_vector_get (line, 5));
}

//! copy the vector into the vector list v_list at position index
void P1906MOL_MOTOR_Field::insertVector(gsl_matrix * v_list, size_t index, gsl_vector * vector)
{
  if (index > v_list->size1)
  {
    printf ("(insertVector) index: %ld larger than v_list size: %ld\n", index, v_list->size1);
  }
	
  //! copy the vector into the vector list at position index
  for (size_t i = 0; i < 6; i++)
    gsl_matrix_set (v_list, index, i, gsl_vector_get (vector, i));
}

//! print all the points in the list of pts
void P1906MOL_MOTOR_Field::displayPoints(gsl_matrix *pts)
{
  size_t numPts = pts->size1;
  
  for (size_t i = 0; i < numPts; i++)
    printf ("Point: %g %g %g\n", 
	  gsl_matrix_get(pts, i, 0), 
	  gsl_matrix_get(pts, i, 1),
	  gsl_matrix_get(pts, i, 2));
}

//! return true if point pt lies on segment, false otherwise
bool P1906MOL_MOTOR_Field::isPointOverlap(gsl_vector * pt, gsl_vector * segment)
{
	bool overlap = false;
	/**
	  Determine whether the points A(2,4,2) B(3,7,-3) and C(1,3,3) lie on a straight line.
	  Start by determining a direction vector AB = [a, b, c] for the line connecting points A and B. 
	  This vector gives the slopes of the three parametric equations describing the line between the points. 
	  AB = [\Delta x, \Delta y, \Delta z] = [3 - 2, 7 - 4, -3 - 2] = [1, 3, -5]. 
	  Three parametric equations for a line are: 
        x = x1 + at 
        y = y1 + bt 
        z = z1 + ct, 
      So if we let A = (x1, y1, z1) 
        x = 2 + t 
        y = 4 + 3t 
        z = 2 - 5t 
      These three parametric equations describe the line connecting A and B. 
	  To see if C is on this line, we can substitute its x coordinate into the x parametric equation to find t: 
        1 = 2 + t, 
      so t = -1. Now substitute this t into the other two equations: 
        y = 4 + 3(-1) = 1; 
        z = 2 - 5(-1) = 7. 
      So (1, 1, 7) is a point on the line, but C = (1, 3, 3) is not.
    */
	double px = gsl_vector_get (pt, 0);
	double py = gsl_vector_get (pt, 1);
	double pz = gsl_vector_get (pt, 2);
	
	double sx1 = gsl_vector_get (segment, 0);
	double sy1 = gsl_vector_get (segment, 1);
	double sz1 = gsl_vector_get (segment, 2);
	
	double sx2 = gsl_vector_get (segment, 0);
	double sy2 = gsl_vector_get (segment, 1);
	double sz2 = gsl_vector_get (segment, 2);
	
	//! direction vector
	gsl_vector * dV = gsl_vector_alloc (3);
	
	//! slope of each dimension
	gsl_vector_set (dV, 0, sx2 - sx1);
	gsl_vector_set (dV, 1, sy2 - sy1);
	gsl_vector_set (dV, 2, sz2 - sz1);
	
	if ( ( ((px - sx1) / gsl_vector_get (dV, 0)) == ((py - sy1) / gsl_vector_get (dV, 1)) ) &&  
		 ( ((py - sy1) / gsl_vector_get (dV, 1)) == ((pz - sz1) / gsl_vector_get (dV, 2)) ) )
	  overlap = true;
	
	return overlap;
}

//! print the single 3D point pt
void P1906MOL_MOTOR_Field::displayPoint(gsl_vector *pt)
{
  printf ("Point: %g %g %g\n", 
	gsl_vector_get(pt, 0), 
	gsl_vector_get(pt, 1),
	gsl_vector_get(pt, 2));
}

//! print the first numPts points in the list of pts
void P1906MOL_MOTOR_Field::displayPoints(gsl_matrix *pts, size_t numPts)
{
  for (size_t i = 0; i < numPts; i++)
    printf ("Point: %g %g %g\n", 
	  gsl_matrix_get(pts, i, 0), 
	  gsl_matrix_get(pts, i, 1),
	  gsl_matrix_get(pts, i, 2));
}

//! return vector field vf determined by the tubes in tubeMatrix
void P1906MOL_MOTOR_Field::tubes2VectorField(gsl_matrix * tubeMatrix, gsl_matrix * vf)
{
  gsl_matrix * v = gsl_matrix_alloc (tubeMatrix->size1, 3);
  gsl_matrix * pt = gsl_matrix_alloc (tubeMatrix->size1, 3);
  
  for (size_t i = 0; i < tubeMatrix->size1; i++)
    for (size_t j = 0; j < 3; j++)
    {
      gsl_matrix_set (pt, i, j, gsl_matrix_get (tubeMatrix, i, j));
      gsl_matrix_set (v, i, j, gsl_matrix_get (tubeMatrix, i, j + 3));
    }
        
  // printf ("(tubes2VectorField) set pt and v\n");
  gsl_matrix_sub (v, pt);
  // printf ("(tubes2VectorField) subtracted pt from v\n");
  
  for (size_t i = 0; i < tubeMatrix->size1; i++)
    for (size_t j = 0; j < 3; j++)
    {
          gsl_matrix_set (vf, i, j, gsl_matrix_get (pt, i, j));
      gsl_matrix_set (vf, i, j + 3, gsl_matrix_get (v, i, j));
    }  
  // printf ("(tubes2VectorField) set vf\n");
}

//! return the index of the nearest tube in tubeMatrix within a given radius from pt, otherwise return -1 
size_t P1906MOL_MOTOR_Field::findNearestTube(gsl_vector * pt, gsl_matrix * tubeMatrix, double radius)
{
  double shortestDistance = GSL_POSINF;
  double d = 0;
  size_t closestSegment = -1;
  gsl_vector *segment = gsl_vector_alloc (6);
  
  for (size_t i = 0; i < tubeMatrix->size1; i++)
  {
    line(segment, tubeMatrix, i);
	d = distance(pt, segment);
	if ((d < shortestDistance) && (d <= radius))
	{
	  shortestDistance = d;
	  closestSegment = i;
	}
  }
  
  // printf ("shortestDistance: %f\n", shortestDistance);
  return closestSegment;
}

//! return the shortest distance between point pt and the line or point segment_or_point
//! if segment_or_point is a vector of length 3, then it is a point
//! if segment_or_point is a vector of length 6, then it is a line segment described by two end points
double P1906MOL_MOTOR_Field::distance(gsl_vector *pt, gsl_vector *segment_or_point)
{
  gsl_vector * td = gsl_vector_alloc (3);
  gsl_vector * segment = gsl_vector_alloc (6);
  gsl_vector * pt1 = gsl_vector_alloc (3);
  gsl_vector * pt2 = gsl_vector_alloc (3);
  gsl_vector * res = gsl_vector_alloc (3);
  gsl_vector * res1 = gsl_vector_alloc (3);
  gsl_vector * res2 = gsl_vector_alloc (3);
  double d;
	  
  switch (segment_or_point->size)
  {
    case 3:
      gsl_vector_memcpy (pt1, pt);
      gsl_vector_memcpy (pt2, segment_or_point);
      gsl_vector_memcpy (td, pt1);
      gsl_vector_sub (td, pt2);
      return gsl_blas_dnrm2 (td);
    case 6:
   	  gsl_vector_memcpy (segment, segment_or_point);
	  
      //! distance = |pt - pt1| x |pt - pt2| / |pt2 - pt1| where x is the cross product
      //! \f$|\vec{pt} - \vec{pt1}| \times |\vec{pt} - \vec{pt2}| / |\vec{pt2} - \vec{pt1}|\f$
      point (pt1,
        gsl_vector_get (segment, 0),
	    gsl_vector_get (segment, 1),
	    gsl_vector_get (segment, 2));
      point (pt2,
        gsl_vector_get (segment, 3),
	    gsl_vector_get (segment, 4),
	    gsl_vector_get (segment, 5));
	
      gsl_vector_memcpy (res1, pt);
      gsl_vector_sub (res1, pt1);
  
      // printf ("res1\n");
      // displayPoint (res1);
  
      gsl_vector_memcpy (res2, pt);
      gsl_vector_sub (res2, pt2);
  
      // printf ("res2\n");
      // displayPoint (res2);
  
      cross_product (res1, res2, res);
      // printf ("res\n");
      // displayPoint (res);
  
      gsl_vector_memcpy (res1, pt2);
      gsl_vector_sub (res1, pt1);
      // printf ("res1\n");
      // displayPoint (res1);
  
      d = gsl_blas_dasum (res) / gsl_blas_dasum (res1);
      // printf ("distance: %g\n", d);
  
      return d;
      /* gsl_blas_sdsdot (0, x, y, result); */
    default:
      printf ("(distance) invalid argument to distance\n");
	  return -1;
  }
}

//! the vector cross product is normal to the input vectors and has magnitude equivalent to the 
//! volume of a parallelogram formed by the input vectors, u and v are the input vectors and 
//! product is the output vector
void P1906MOL_MOTOR_Field::cross_product(const gsl_vector *u, const gsl_vector *v, gsl_vector *product)
{
  double p1 = gsl_vector_get(u, 1) * gsl_vector_get(v, 2)
    - gsl_vector_get(u, 2) * gsl_vector_get(v, 1);
 
  double p2 = gsl_vector_get(u, 2) * gsl_vector_get(v, 0)
    - gsl_vector_get(u, 0) * gsl_vector_get(v, 2);
 
  double p3 = gsl_vector_get(u, 0) * gsl_vector_get(v, 1)
    - gsl_vector_get(u, 1) * gsl_vector_get(v, 0);
 
  gsl_vector_set(product, 0, p1);
  gsl_vector_set(product, 1, p2);
  gsl_vector_set(product, 2, p3);
}

//! a unit test for getOverlap
bool P1906MOL_MOTOR_Field::unitTest_getOverlap()
{
  bool passTests = false;
  gsl_vector * segment = gsl_vector_alloc (6);
  gsl_matrix * tubeMatrix = gsl_matrix_alloc (1, 6);
  gsl_matrix * pts = gsl_matrix_alloc (1, 6);
  gsl_matrix_set_zero (pts);
  gsl_vector * pt1 = gsl_vector_alloc (3);
  gsl_vector * pt2 = gsl_vector_alloc (3);
  gsl_vector * pt3 = gsl_vector_alloc (3);
  gsl_vector * pt4 = gsl_vector_alloc (3);
  gsl_vector *tubeSegments = gsl_vector_alloc (1);
  
  //! Test 1: Simple cross
  
  point (pt1, 25, 0, 0);
  point (pt2, 0, 25, 0);
  point (pt3, 0, 25, 0);
  point (pt4, 25, 0, 0);
  
  line (segment, pt1, pt2);
  line (tubeMatrix, 0, pt3, pt4);
 
  int numPts = getOverlap3D(segment, tubeMatrix, pts, tubeSegments);
  printf ("numPts: %d\n", numPts);
  displayPoints (pts);
  
  //! Test 2: Moving cross
  for (int theta = 0; theta < 2 * M_PI; theta += M_PI / 6);
  //! convert theta to x, y values
  
  //! Test 2: Rotating segment
  
  return passTests;  
}

//! return all overlapping points in tubeMatrix in the list of points pts
void P1906MOL_MOTOR_Field::getAllOverlaps3D(gsl_matrix *tubeMatrix, vector<P1906MOL_MOTOR_Pos> & pts)
{
  size_t numSegments = tubeMatrix->size1;
  gsl_vector *segment = gsl_vector_alloc (6);
  gsl_matrix *tmpPts = gsl_matrix_alloc (numSegments * numSegments, 3);
  size_t numPts = 0; // totPts = 0;
  //size_t pp = 0;
  gsl_vector *tubeSegments = gsl_vector_alloc (numSegments * numSegments);
  
  for (size_t i = 0; i < numSegments; i++)
  {
    //! move segment from tubes to segment
	for (size_t k = 0; k < 6; k++)
	  gsl_vector_set (segment, k, gsl_matrix_get (tubeMatrix, i, k));
	//! check for overlap
    numPts = getOverlap3D(segment, tubeMatrix, tmpPts, tubeSegments);
	//! store overlapping points
	for (size_t k = 0; k < numPts; k++)
	{
	  P1906MOL_MOTOR_Pos Pos;
	  Pos.setPos ( gsl_matrix_get (tmpPts, k, 0),
	               gsl_matrix_get (tmpPts, k, 1),
	               gsl_matrix_get (tmpPts, k, 2) );
	  pts.insert(pts.end(), Pos);
	}
  }
}

//! return the number of overlapping points in pts and the index of the tubeMatrix segments overlapped in tubeSegments
int P1906MOL_MOTOR_Field::getOverlap3D(gsl_vector * segment, gsl_matrix * tubeMatrix, gsl_matrix * pts, gsl_vector * tubeSegments)
{
  /** 
    all points defined by x, y, z values
	line A -> B: (a1, a2, a3) -> (b1, b2, b3)
	line C -> D: (c1, c2, c3) -> (d1, d2, d3)
  
    solve: 
    a1 + t * (b1 - a1) == c1 + s * (d1 - c1)
	a2 + t * (b2 - a2) == c2 + s * (d2 - c2)
	a3 + t * (b3 - a3) == c3 + s * (d3 - c3)

    A x =  b where x(0) = t and x(1) = s
	(b1 - a1) * t - (d1 - c1) * s == c1 - a1
	(b2 - a2) * t - (d2 - c2) * s == c2 - a2
	(b3 - a3) * t - (d3 - c3) * s == c3 - a3
  */
  //! total number of segments in tubeMatrix
  size_t numSegments = tubeMatrix->size1;
  //! first end point of segment under test
  double a1 = gsl_vector_get (segment, 0), a2 = gsl_vector_get (segment, 1), a3 = gsl_vector_get (segment, 2);
  //! second end point of segment under test
  double b1 = gsl_vector_get (segment, 3), b2 = gsl_vector_get (segment, 4), b3 = gsl_vector_get (segment, 5);
  //! first end point of tubeMatrix segment 
  double c1, c2, c3;
  //! second end point of tubeMatrix segment
  double d1, d2, d3;
  
  gsl_matrix * A = gsl_matrix_alloc (3, 2);
  gsl_matrix * V = gsl_matrix_alloc (2, 2);
  gsl_vector * b = gsl_vector_alloc (3);
  gsl_vector * x = gsl_vector_alloc (2);
  gsl_vector * S = gsl_vector_alloc (2);
  gsl_vector * work = gsl_vector_alloc (2);
  gsl_vector * pt = gsl_vector_alloc (3);
  size_t numPts = 0;
  size_t ts = 0;
  
  for(size_t i = 0; i < numSegments; i++)
  {
    c1 = gsl_matrix_get (tubeMatrix, i, 0), c2 = gsl_matrix_get (tubeMatrix, i, 1), c3 = gsl_matrix_get (tubeMatrix, i, 2);
    d1 = gsl_matrix_get (tubeMatrix, i, 3), d2 = gsl_matrix_get (tubeMatrix, i, 4), d3 = gsl_matrix_get (tubeMatrix, i, 5);
    
	// printf ("a1 = %g, a2 = %g, a3 = %g\n", a1, a2, a3);
	// printf ("b1 = %g, b2 = %g, b3 = %g\n", b1, b2, b3);
	// printf ("c1 = %g, c2 = %g, c3 = %g\n", c1, c2, c3);
	// printf ("d1 = %g, d2 = %g, d3 = %g\n", d1, d2, d3);
  
    gsl_matrix_set(A, 0, 0, b1 - a1);
    gsl_matrix_set(A, 0, 1, - (d1 - c1));
    gsl_matrix_set(A, 1, 0, b2 - a2);
    gsl_matrix_set(A, 1, 1, - (d2 - c2));
	gsl_matrix_set(A, 2, 0, b3 - a3);
    gsl_matrix_set(A, 2, 1, - (d3 - c3));
    
    gsl_vector_set(b, 0, c1 - a1);
    gsl_vector_set(b, 1, c2 - a2);
	gsl_vector_set(b, 2, c3 - a3);
	
	// printf ("A = %g %g\n    %g %g\n    %g %g\n", 
	//  gsl_matrix_get(A, 0, 0), gsl_matrix_get(A, 0, 1), 
	//  gsl_matrix_get(A, 1, 0), gsl_matrix_get(A, 1, 1),
	//  gsl_matrix_get(A, 2, 0), gsl_matrix_get(A, 2, 1));
	//printf ("b = %g\n    %g\n    %g\n", 
	//  gsl_vector_get(b, 0), 
	//  gsl_vector_get(b, 1),
	//  gsl_vector_get(b, 2));
  
    //! solve A x = b
    //! A is M x N, work is tmp storage of length N, A is replaced with U, V is N x N, S is M x N
    gsl_linalg_SV_decomp (A, V, S, work);
    gsl_linalg_SV_solve (A, V, S, b, x);

    // printf ("x = \n");
    // gsl_vector_fprintf(stdout, x, "%g");
	
	//! if x(0) = t, x(1) = s exist, then substitute and solve for overlapping point
	
	gsl_vector_set (pt, 0, a1 + gsl_vector_get(x, 0) * (b1 - a1));
	gsl_vector_set (pt, 1, a2 + gsl_vector_get(x, 0) * (b2 - a2));
	gsl_vector_set (pt, 2, a3 + gsl_vector_get(x, 0) * (b3 - a3));
    
	// printf ("pt = \n");
    // gsl_vector_fprintf(stdout, pt, "%g");
	
    if(!gsl_isnan (gsl_vector_get(x, 0)) && !gsl_isnan (gsl_vector_get(x, 1)))
	  //! the segments only overlap if their lengths are long enough to do so
	  if(
	    //! A <= x <= B and C <= x <= D
	    min(a1, b1) <= gsl_vector_get(pt, 0) && 
		min(a2, b2) <= gsl_vector_get(pt, 1) && 
		min(a3, b3) <= gsl_vector_get(pt, 2) && 
		
		max(a1, b1) >= gsl_vector_get(pt, 0) && 
		max(a2, b2) >= gsl_vector_get(pt, 1) && 
		max(a3, b3) >= gsl_vector_get(pt, 2) && 
		
		min(c1, d1) <= gsl_vector_get(pt, 0) && 
		min(c2, d2) <= gsl_vector_get(pt, 1) && 
		min(c3, d3) <= gsl_vector_get(pt, 2) && 
		
		max(c1, d1) >= gsl_vector_get(pt, 0) && 
		max(c2, d2) >= gsl_vector_get(pt, 1) && 
		max(c3, d3) >= gsl_vector_get(pt, 2)
	  )
	  {
	    gsl_matrix_set(pts, numPts, 0, gsl_vector_get(pt, 0));
	    gsl_matrix_set(pts, numPts, 1, gsl_vector_get(pt, 1));
		gsl_matrix_set(pts, numPts, 2, gsl_vector_get(pt, 2));
	    // printf ("numPts(%ld) = %g %g %g\n", numPts, 
		//  gsl_matrix_get(pts, numPts, 0), 
		//  gsl_matrix_get(pts, numPts, 1),
		//  gsl_matrix_get(pts, numPts, 2));
		gsl_vector_set (tubeSegments, ts++, i);
	    numPts++;
	  }
	}
  
  return numPts;
}

//! return the location of the vector from vf that is closest to the point pt and put it in result
void P1906MOL_MOTOR_Field::findClosestPoint(gsl_vector * pt, gsl_matrix * vf, gsl_vector * result)
{
  //! the current closest point
  gsl_vector * cpt = gsl_vector_alloc (3);
  //! the current closest vector 
  gsl_vector * cv = gsl_vector_alloc (3);
  //! the test point
  gsl_vector * tpt = gsl_vector_alloc (3);
  
  //! display the vector field
  // for (size_t i = 0; i < vf->size1; i++)
  // {
  //  printf ("vf: %f %f %f %f %f %f\n",
  //	  gsl_matrix_get (vf, i, 0),
  //	  gsl_matrix_get (vf, i, 1),
  //	  gsl_matrix_get (vf, i, 2),
  //	  gsl_matrix_get (vf, i, 3),
  //	  gsl_matrix_get (vf, i, 4),
  //	  gsl_matrix_get (vf, i, 5));
  // }
  
  point (cpt, GSL_POSINF, GSL_POSINF, GSL_POSINF);
  
  for (size_t i = 0; i < vf->size1; i++)
  {
    //! test the next vector field location 
    point (tpt, 
	  gsl_matrix_get (vf, i, 0),
	  gsl_matrix_get (vf, i, 1),
	  gsl_matrix_get (vf, i, 2));
	
	// printf ("pt\n");
	// displayPoint (pt);
	// printf ("tpt\n");
	// displayPoint (tpt);
	// printf ("cpt\n");
	// displayPoint (cpt);
	
	//! tpt is point under test from vector field
	//! cpt is the current shortest distance point
	// printf ("(findClosestPoint) distance to tpt: %f\n", distance (tpt, pt));
	// printf ("(findClosestPoint) distance to cpt: %f\n", distance (cpt, pt));
	
	//! update the location and vector if closer
	if ((distance (tpt, pt) < distance (cpt, pt)) || gsl_isnan(distance (cpt, pt)))
	{
	  // printf ("storing point\n");
	  point (cpt, 
	    gsl_vector_get (tpt, 0),
	    gsl_vector_get (tpt, 1),
	    gsl_vector_get (tpt, 2));
	  // printf ("cpt\n");
	  // displayPoint (cpt);
      point (cv, 
	    gsl_matrix_get (vf, i, 3),
	    gsl_matrix_get (vf, i, 4),
	    gsl_matrix_get (vf, i, 5));
	  // printf ("cv\n");
	  // displayPoint (cv);
	}
  }
  
  // printf ("(findClosestPoint) final cv\n");
  // displayPoint (cv);
  
  //! return the result
  gsl_vector_set (result, 0, gsl_vector_get (cpt, 0));
  gsl_vector_set (result, 1, gsl_vector_get (cpt, 1));
  gsl_vector_set (result, 2, gsl_vector_get (cpt, 2));
  gsl_vector_set (result, 3, gsl_vector_get (cv, 0));
  gsl_vector_set (result, 4, gsl_vector_get (cv, 1));
  gsl_vector_set (result, 5, gsl_vector_get (cv, 2));
  
  // printf ("(findClosestPoint) result: %f %f %f %f %f %f\n",
  //  gsl_vector_get (result, 0),
  //	gsl_vector_get (result, 1),
  //	gsl_vector_get (result, 2),
  //	gsl_vector_get (result, 3),
  //	gsl_vector_get (result, 4),
  //	gsl_vector_get (result, 5));
}

//! return the information entropy \f$ H(x) = - sum( P(x) \log P(x) ) \f$ of a tube segment defined by its list of angles in segAngle
double P1906MOL_MOTOR_Field::sEntropy(gsl_matrix *segAngle)
{
  //! bin the values in order to find P(x)
  //! see gsl_histogram_pdf * gsl_histogram_pdf_alloc (size_t n)
  //! see https://www.gnu.org/software/gsl/manual/html_node/The-histogram-probability-distribution-struct.html
  
  gsl_histogram * h = gsl_histogram_alloc (100); /* need to determine optimal number of bins */
  //! get the max segAngle
  double minAngle = gsl_matrix_min(segAngle);
  double maxAngle = gsl_matrix_max(segAngle);
  gsl_histogram_set_ranges_uniform (h, minAngle, maxAngle);

  for (size_t i = 0; i < segAngle->size1; i++)
    for (size_t j = 0; j < 1; j++)
	{
	  //! printf ("segAngle(%ld,%ld) = %g\t", i, j, gsl_matrix_get (segAngle, i, j)); */
	  gsl_histogram_increment (h, gsl_matrix_get (segAngle, i, j));
    }

  double H = 0;
  int numBins = gsl_histogram_bins (h);
  for(int i = 0; i < numBins; i++)
  {
	double p = gsl_histogram_get (h, i) / gsl_histogram_sum(h);
	//! printf("p = %g\n", p);
	if (p > 0)
	{
	  H = H - p * gsl_sf_log(p);
	  //! printf("H = %g\n", H);
	}
  }
	
  // printf("H = %g\n", H);
  //! gsl_histogram_fprintf (stdout, h, "%g", "%g");
  //gsl_histogram_free (h);
  
  return H;
}

P1906MOL_MOTOR_Field::~P1906MOL_MOTOR_Field ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
