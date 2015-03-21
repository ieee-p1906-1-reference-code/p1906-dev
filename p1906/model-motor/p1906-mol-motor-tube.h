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


#ifndef P1906_MOL_MOTOR_TUBE
#define P1906_MOL_MOTOR_TUBE

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
 * \class P1906MOL_MOTOR_Tube
 *
 * \brief Class implementing a generic nanotube structure
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

class P1906MOL_MOTOR_Tube : public P1906MOL_MOTOR_Field
{
public:
  static TypeId GetTypeId (void);
  
  //! random number generation structures and initialization
  const gsl_rng_type * T;
  gsl_rng * r;
  
  //! the tube starting point
  //! \todo get actual tube graph properties from a biologist
  gsl_vector * startPt;
  //! hold the values for a tube comprised of many segments: x_start y_start x_start x_end y_end z_end
  gsl_matrix * segMatrix;

  /*
   * Methods related to creating a tube
   */  
  //! the constructor build a tube determined by tubeCharacteristcs_t
  P1906MOL_MOTOR_Tube (struct tubeCharacteristcs_t * ts, gsl_vector * startPt);
  //! return the microtubule in segMatrix of a given persistence length starting at position startPt and its structural entropy in se
  int genTube(struct tubeCharacteristcs_t * ts, gsl_rng * r, gsl_matrix * segMatrix, gsl_vector * startPt);

  /*
   * Methods related to the tube's persistence length
   */
  //! generate tube structure with a given segment and persistence length
  double genPersistenceLength(gsl_rng * r, gsl_matrix * segAngle, double segLength, double persistenceLength); 
  //! compute the persistence length of a set of segments
  double getPersistenceLength();
  
  /*
   * Methods related to accessing and displaying a tube
   */  
  //! return segMatrix, which is comprised of tube segment end points
  void getSegmatrix(gsl_matrix * sm);  
  //! simply print the end points of each segment of a tube
  void displayTube();
     
  virtual ~P1906MOL_MOTOR_Tube ();

};

}

#endif /* P1906_MOL_MOTOR_TUBE */