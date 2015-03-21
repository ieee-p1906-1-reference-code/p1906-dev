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


#ifndef P1906_METRICS
#define P1906_METRICS

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

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906_Metrics
 *
 * \brief Base class implementing P1906 metrics
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

class P1906_Metrics : public Object
{
public:
  static TypeId GetTypeId (void);
  
  P1906_Metrics ();
  
/*
 * Message Component: These metrics deal with the information encoded within a Message and how the 
 * Message is impacted by the channel and intended target. Metrics (a) thru (d) shall be implemented. 
 * Metric (e) may be implemented.
 * a)	Message Deliverability
 * b)	Message Lifetime
 * c)	Information Density
 * d)	Bandwidth-Delay Product
 * e)	Information and Communication Energy
 */
 
/* 
 * Motion Component: These metrics are strongly related to the Motion Component, which describes Message Carrier motion. 
 * Either both (a) and (b) shall be implemented or (c) shall be implemented. 
 * a)	Collision Behavior
 * b)	Mass Displacement
 * c)	Positioning Accuracy of Message Carriers
 */
 
/*
 * Field Component: These metrics relate to the degree to which Message Carrier motion can be controlled such that it 
 * follows an intended gradient. Diffusive Flux is used in Brownian motion and can be modeled by Levy or Weiner processes 
 * and can also be described by the Langevin Noise. At least one of (a), (b), or (c) shall be implemented in order to 
 * describe Message Carrier motion.
 * a)	Persistence Length
 * b)	Diffusive Flux
 * c)	Langevin Noise
 */
 
/*
 * Specificity Component: These metrics are related to the ability of a Message Carrier to deliver a Message to its 
 * intended target. Metrics (a), (c), and (e) shall be implemented; metrics (b) and/oror (d) may be implemented.
 * a)	Specificity
 * b)	Affinity
 * c)	Sensitivity
 * d)	Angular Spectrum 
 * e)	Delay Spectrum
 */

/* System: These metrics relate to and impact all components. All of the metrics in this category shall be implemented. 
 * a)	Active Network Programmability
 * b)	Perturbation Rate
 * c)	Supersystem Degradation
 * d)	Bandwidth-Volume Ratio
 */

  void Message_Deliverability();
  void Message_Lifetime();
  void Information_Density();
  void Bandwidth_Delay_Product();
  void Information_and_Communication_Energy();
  void Collision_Behavior();
  void Mass_Displacement();
  void Positioning_Accuracy_of_Message_Carriers();
  void Persistence_Length();
  void Diffusive_Flux();
  void Langevin_Noise();
  void Specificity();
  void Affinity();
  void Sensitivity();
  void Angular_Spectrum();
  void Delay_Spectrum();
  void Active_Network_Programmability(gsl_matrix * vf, gsl_vector * pt);
  void Perturbation_Rate();
  void Supersystem_Degradation();
  void Bandwidth_Volume_Ratio();
  
  virtual ~P1906_Metrics ();

};

}

#endif /* P1906_METRICS */
