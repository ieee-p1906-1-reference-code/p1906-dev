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

#ifndef P1906_MOL_EXTENDEDMOTION
#define P1906_MOL_EXTENDEDMOTION

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
#include "ns3/p1906-motion.h"
#include "ns3/p1906-mol-motion.h"
#include "ns3/p1906-mol-motor-pos.h"
#include "ns3/p1906-mol-motor-vol-surface.h"

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906MOL_MOTOR_Motion
 *
 * \brief Class extends the capability of the P1906 molecular Motion component towards molecular motor motion
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

class P1906MOL_MOTOR_Motion : public P1906MOLMotion
{
public:
  static TypeId GetTypeId (void);
 
  /*
   * Methods related to tracking motor position
   */ 
  //! print a single position
  void displayPos(gsl_vector * pts);
    
  /*
   * Methods related to motor motion
   */
  //! motor is driven by Brownian motion until the destination is reached, returning the propagation time
  void float2Destination(Ptr<P1906MessageCarrier> carrier, double timePeriod);
  //! motor binds to microtubule and walks and is driven by Brownian motion when unbound to microtubule, returning propagation time
  void move2Destination(Ptr<P1906MessageCarrier> carrier, gsl_matrix * tubeMatrix, size_t segPerTube, double timePeriod, vector<P1906MOL_MOTOR_Pos> & pts);
  //! display all the volume surfaces recognizing the motor
  void displayVolSurfaces();
  //! newPos is Brownian motion from currentPos over timePeriod 
  void brownianMotion(gsl_rng * r, gsl_vector * currentPos, gsl_vector * newPos, double timePeriod, double D, vector<P1906MOL_MOTOR_VolSurface> & vsl);
  //! Brownian motion from startPt for length time in timePeriod units; results returned in pts
  int freeFloat(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> & pts, int time, double timePeriod, vector<P1906MOL_MOTOR_VolSurface> & vsl);
  //! free float until intersection with any tube
  size_t float2Tube(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> & pts, gsl_matrix * tubeMatrix, double timePeriod,vector<P1906MOL_MOTOR_VolSurface> & vsl);
  //! walk along a specific tube identified by startPt and place result in pts
  void motorWalk(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> & pts, gsl_matrix * tubeMatrix, size_t segPerTube, vector<P1906MOL_MOTOR_VolSurface> & vsl);
  
  /*
   * These methods are required to utilize the core IEEE 1906 reference model
   */
  //! return the propagation delay by simulating motor motion from transmitter to receiver.
  double ComputePropagationDelay (Ptr<P1906CommunicationInterface> src,
  		                                  Ptr<P1906CommunicationInterface> dst,
  		                                  Ptr<P1906MessageCarrier> message,
  		                                  Ptr<P1906Field> field);
  //! return the proper Message Carrier
  Ptr<P1906MessageCarrier> CalculateReceivedMessageCarrier(Ptr<P1906CommunicationInterface> src,
  		                                                           Ptr<P1906CommunicationInterface> dst,
  		                                                           Ptr<P1906MessageCarrier> message,
  		                                                           Ptr<P1906Field> field);
  
  P1906MOL_MOTOR_Motion ();
  virtual ~P1906MOL_MOTOR_Motion ();

};

}

#endif /* P1906_MOL_EXTENDEDMOTION */
