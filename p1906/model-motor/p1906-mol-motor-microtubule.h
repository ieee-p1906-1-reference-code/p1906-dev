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


#ifndef P1906_MOL_MOTOR_FIELD_MICROTUBULES
#define P1906_MOL_MOTOR_FIELD_MICROTUBULES

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
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-motion.h"

#include "ns3/p1906-mol-motor-tube-characteristics.h"

namespace ns3 {

/**
 * \ingroup P1906 framework
 *
 * \class P1906MOL_MOTOR_MicrotubulesField
 *
 * \brief Class implements molecular motor transport using microtubules in the P1906 framework
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
 *  
 * See http://www.gnu.org/software/gsl/manual/html_node/Level-3-GSL-BLAS-Interface.html#Level-3-GSL-BLAS-Interface.
 *
 */

class P1906MOL_MOTOR_MicrotubulesField : public P1906MOL_MOTOR_Field
{
public:
  static TypeId GetTypeId (void);
    
  //! create a given density of tubes of numSegments in given volume; default volume starts at origin with length volume^(1/4) in each dimension  
  gsl_matrix * tubeMatrix;
  //! properties of the microtubule network
  tubeCharacteristcs_t ts;
  //! holds the vector field
  gsl_matrix * vf;

  //! random number generation structures and initialization
  const gsl_rng_type * T;
  gsl_rng * r;
  
  P1906MOL_MOTOR_MicrotubulesField ();
  
  /*
   * Methods related to tube properties
   */
  //! set the volume in which tubes will be generated
  void setTubeVolume(double volume = 25);
  //! set the mean tube length
  void setTubeLength(double mean_tube_length = 100);
  //! set the mean angle between tube segments
  void setTubeIntraAngle(double mean_intra_tube_angle = 30);
  //! set the mean angle between entire tubes
  void setTubeInterAngle(double mean_inter_tube_angle = 10);
  //! set the density of tube segments
  void setTubeDensity(double mean_tube_density = 10);
  //! set the persistence length of the tubes
  void setTubePersistenceLength(double persistenceLength = 50);
  //! set the number of segments per tube
  void setTubeSegments(size_t segPerTube = 10);
  //! display all the microtubule network properties
  void displayTubeChars();
  
  /*
   * Methods related to creating microtubules and analyzing molecular motor transport
   */
  //! return the size of the matrix to allocate
  void getTubesSize(double * rows, double * cols);
  //! export tubes; copy the object's tubeMatrix into tm for use outside the object
  void getTubes(gsl_matrix * tm);
  //! import tubes; create a local copy of the tube structure tm in tubeMatrix
  void setTubes(gsl_matrix * tm);
  //! fill tubeMatrix with random tubes in area with a given number of total segments and persistence length
  void genTubes();
  //! plot persistence length versus structural entropy
  void persistenceVersusEntropy(gsl_vector * persistenceLengths);

  /*
   * Methods implementing unit tests
   */
   
  //! test plot2mma
  bool unitTest_Plot2Mma(vector<P1906MOL_MOTOR_Pos> & pts);
  //! test persistence length versus entropy plot
  bool unitTest_PersistenceLengthsVsEntropy();
  //! test computation of all segment overlaps
  bool unitTest_AllOverlaps();
  //! test computation of a single overlap
  bool unitTest_Overlap();
  //! test the computation of distance
  bool unitTest_Distance();
  //! test the computation of a vector field
  bool unitTest_VectorField();
  //! test motor movement using Brownian motion until destination reached
  bool unitTest_NoTubeMotion();
  //! test the movement of a motor floating to a tube and walking on the tube
  bool unitTest_MotorMovement(vector<P1906MOL_MOTOR_Pos> & pts);
  //! test motor movement to destination
  bool unitTest_MotorMove2Destination(vector<P1906MOL_MOTOR_Pos> & pts);
  //! test the volume surface as a flux meter and later as a compartmentalization volume
  bool unitTest_VolSurface();
  //! test the volume surface as a compartmentalization volume
  bool unitTest_ReflectiveBarrier();
  //! test the FluxMeter
  bool unitTest_FluxMeter();
  
  virtual ~P1906MOL_MOTOR_MicrotubulesField ();

};

}

#endif /* P1906_MOL_MOTOR_FIELD_MICROTUBULES */
