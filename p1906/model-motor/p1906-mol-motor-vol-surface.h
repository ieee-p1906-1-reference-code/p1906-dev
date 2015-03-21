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


#ifndef  P1906_MOL_MOTOR_VOLSURFACE
#define  P1906_MOL_MOTOR_VOLSURFACE

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>

#include <iostream>
#include <fstream>
using namespace std;

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-mol-motor-field.h"

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906MOL_MOTOR_VolSurface
 *
 * \brief Class implementing a volume surface
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

class P1906MOL_MOTOR_VolSurface : public P1906MOL_MOTOR_Field
{
public:
  static TypeId GetTypeId (void);
  
  //! random number generation structures and initialization
  const gsl_rng_type * T;
  gsl_rng * r;
  
  //! define the volume surface assuming a sphere for now
  P1906MOL_MOTOR_Pos center; //! the center point of the sphere
  double radius; //! the radius of the sphere
  
  //! FluxMeter - measure flux through the volume surface
  //! ReflectiveBarrier - act as reflective bounding surface
  //! Receiver - act as a motor destination volume
  enum typeOfVolume { FluxMeter, ReflectiveBarrier, Receiver };
  typeOfVolume volType;
  
  /*
   * Methods related to creating the volume surface
   */  
  //! the constructor to build a sphere surface
  P1906MOL_MOTOR_VolSurface ();
  //! set enum FluxMeter, ReflectiveBarrier, Receiver
  void setType (typeOfVolume st);
  //! return enum FluxMeter, ReflectiveBarrier, Receiver
  typeOfVolume getType ();
  //! set the location and size of the volume sphere
  void setVolume(P1906MOL_MOTOR_Pos v_center, double v_radius);
  
  /*
   * Methods related to accessing and displaying a volume surface
   */  
  //! simply print the center and radius of the sphere
  void displayVolSurface();
  
  /*
   * Methods related to querying location relative to volume
   */
  //! return true if the point is inside the volume surface
  bool isInsideVolSurf(P1906MOL_MOTOR_Pos pt);
  //! return the angle between two vectors
  double vectorAngle(gsl_vector * seg1, gsl_vector * seg2);
  //! return radius line segment from center to a point on the surface
  //! v_radius must be allocated for a line segment
  void getRadiusLine(gsl_vector * v_radius, P1906MOL_MOTOR_Pos pt);
  
  /*
   * Methods related to intersection with and reflection from the volume surface
   */
  //! reflect a particle from the surface given the last and current positions
  //! adjust the current position given a reflection
  void reflect(P1906MOL_MOTOR_Pos last_pos, P1906MOL_MOTOR_Pos & current_pos);
  //! find the intersecting point(s) ipt that intersect the sphere:
  //! (1) if no intersection, then ipt has zero values
  //! (2) if one intersection, then ipt has one intersection point
  //! (3) if two intersections, then ipt has two intersection points
  void sphereIntersections(gsl_vector * segment, vector<P1906MOL_MOTOR_Pos> & ipt);
  
  /*
   * Methods related to flow through the volume surface
   */ 
  //! compute the flux through the FluxMeter volume surface based upon the microtubules in tubeMatrix
  double fluxMeter(gsl_matrix * tubeMatrix);

  virtual ~P1906MOL_MOTOR_VolSurface ();

};

}

#endif /*  P1906_MOL_MOTOR_VOLSURFACE */