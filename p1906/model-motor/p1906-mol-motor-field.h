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


#ifndef P1906_MOL_MOTOR_FIELD
#define P1906_MOL_MOTOR_FIELD

#include <iostream>
#include <fstream>
using namespace std;

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-mol-field.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906MOL_MOTOR_Field
 *
 * \brief Class extends the molecular Field component of the P1906 framework toward implementing a vector field
 *
 * This class implements persistence length as described in:
 *	  Bush, S. F., & Goel, S. (2013). Persistence Length as a Metric for Modeling and 
 *	    Simulation of Nanoscale Communication Networks, 31(12), 815-824. 
 *		http://dx.doi.org/10.1109/JSAC.2013.SUP2.12130014.
 *	  
 * The Gnu Scientific Library (GSL) is required for these methods.
 *  All points and positions are in three dimensions comprised of a gsl_vector * of length three (x, y, z).
 *  All lines and segments are comprised of two points within a gsl_vector * of length size (x1, y1, z1), (x2, y2, z2).
 *  Lists of points or positions are in a gsl_matrix * of size n x 3.
 *  Each tube is comprised of a list of segments within a gsl_matrix * of size s x 6 -> s x ((x1, y1, z1), (x2, y2, z2)).
 *  A set of tubes is also a gsl_matrix * of size (s * t) x 6, where s is the number of segments and t the number of tubes.
 *  All random number are derived from gsl_rng *.
 */

class P1906MOL_MOTOR_Field : public P1906MOLField
{
public:
  static TypeId GetTypeId (void);
    
  /*
   * Methods related to vector fields
   */
  //! return the location of the vector from vf that is closest to the point pt and put it in result
  static void findClosestPoint(gsl_vector * pt, gsl_matrix * vf, gsl_vector *result);
  //! convert the tube structures to a vector field of the same dimensions as the tubeMatrix
  void tubes2VectorField(gsl_matrix * tubeMatrix, gsl_matrix * vf);
 
  /*
   * Methods implementing unit tests
   */
  //! test tube overlaps
  bool unitTest_getOverlap();
  //! test the findClosestPoint function
  bool unitTest_findClosestPoint();

  /*
   * Methods related to creating and displaying points and lines in 3D
   */
  //! return a pt vector comprised of x, y, z
  static void point(gsl_vector * pt, double x, double y, double z);
  //! return a line comprised of two points
  static void line(gsl_vector * line, gsl_vector * pt1, gsl_vector * pt2);
  //! place a line comprised of pt1 and pt2 into a list of lines at position mp
  void line(gsl_matrix * line, size_t mp, gsl_vector * pt1, gsl_vector * pt2);
  //! retrieve segment mp from tubeMatrix and place it in segment
  static void line(gsl_vector * segment, gsl_matrix * tubeMatrix, size_t mp);
  //! display the segment
  static void displayLine(gsl_vector * line);
  //! set the segment with the given end points
  static void line(gsl_vector * line, P1906MOL_MOTOR_Pos p1, P1906MOL_MOTOR_Pos p2);
  //! print the list of 3D points
  void displayPoints(gsl_matrix * pts);
  //! print only the first 3D numPts
  void displayPoints(gsl_matrix * pts, size_t numPts);
  //! print the value of a single 3D point pt
  void displayPoint(gsl_vector * pt);
  //! true if pt insects segment, false otherwise
  bool isPointOverlap(gsl_vector * pt, gsl_vector * segment);
  //! copy the vector into the vector list v_list at position index
  void insertVector(gsl_matrix * v_list, size_t index, gsl_vector * vector);
   
  /*
   * Methods related to computing structural entropy
   */
  //! return the structural entropy based upon a list of angles
  double sEntropy(gsl_matrix * segAngle);
  
  /*
   * Methods related to tube distance and overlap
   */
  //! return the cross product of u and v in product, all vectors are the same size
  static void cross_product(const gsl_vector * u, const gsl_vector * v, gsl_vector * product);
  //! return the shortest distance in 3D space between the point pt and the segment
  static double distance(gsl_vector * pt, gsl_vector * segment);
  //! return all the points where tubes overlap with one another in pts
  void getAllOverlaps3D(gsl_matrix * tubeMatrix, vector<P1906MOL_MOTOR_Pos> & pts);
  //! return all the points where a segment overlaps with a list of tubes in pts
  int getOverlap3D(gsl_vector * segment, gsl_matrix * tubeMatrix, gsl_matrix * pts, gsl_vector * tubeSegments);
  //! return the nearest segment in tubeMatrix to the point pt that falls within radius from the point, otherwise return -1
  static size_t findNearestTube(gsl_vector * pt, gsl_matrix * tubeMatrix, double radius);  
   
  P1906MOL_MOTOR_Field ();
  virtual ~P1906MOL_MOTOR_Field ();

};

}

#endif /* P1906_MOL_EXTEND_FIELD */
