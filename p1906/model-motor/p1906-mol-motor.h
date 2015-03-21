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


#ifndef P1906_MOL_MOTOR
#define P1906_MOL_MOTOR

#include <vector>

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
#include "ns3/p1906-mol-motor-motion.h"
#include "ns3/p1906-mol-message-carrier.h"
#include "ns3/p1906-mol-motor-pos.h"
#include "ns3/p1906-mol-motor-vol-surface.h"

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906MOL_Motor
 *
 * \brief Class implementing a molecular motor
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

class P1906MOL_Motor : public P1906MOLMessageCarrier
{
public:
  static TypeId GetTypeId (void);
  
  //! the current location of the motor in 3D space
  gsl_vector * current_location;

  //! a record of the position of the motor throughout its lifetime
  vector<P1906MOL_MOTOR_Pos> pos_history;
  
  //! simulated time structure anticipating other fields that may be required for time management
  struct simtime_t
  {
    //! simulated time
    double time;
  } t;
  
  /*
   * Methods related to simulation time
   */
  //! \todo return the propagation delay for a motor
  double getTime();  
  //! reset the simulation time
  void initTime();
  //! update the simulation given an event duration
  void updateTime(double event_time);
  //! return the elapsed time since the motor was created
  double propagationDelay();

  //! randomness for the motor  
  const gsl_rng_type * T;
  gsl_rng * r;
  
  //! currently, only brownianMotion checks if in the Receiver volume
  vector<P1906MOL_MOTOR_VolSurface> vsl;
  
  P1906MOL_Motor ();

  /*
   * Methods related to volume surfaces
   */  
  //! create and store a new volume surface
  void addVolumeSurface(P1906MOL_MOTOR_Pos v_c, double v_radius, P1906MOL_MOTOR_VolSurface::typeOfVolume v_type);
  //! display all the volume surfaces recognizing the motor
  void displayVolSurfaces();
  
  /*
   * Methods related to motor positioning and tracking
   */
  //! print the current location
  void displayLocation();
  //! set the current location
  void setLocation(P1906MOL_MOTOR_Pos pt);
  //! set the current location
  void setLocation(double x, double y, double z);
  //! return true if motor is in the destination volume, false otherwise
  bool inDestination();
  //! this is where the motor starts, for example, location of the transmitter
  void setStartingPoint(gsl_vector * pt);
  
  virtual ~P1906MOL_Motor ();

};

}

#endif /* P1906_MOL_MOTOR */
