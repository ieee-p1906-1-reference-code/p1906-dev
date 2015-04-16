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


#ifndef P1906_MOL_MOTOR_POS
#define P1906_MOL_MOTOR_POS

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

#include "ns3/attribute-helper.h"
#include "ns3/double.h"
#include "ns3/object-vector.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/traced-callback.h"
#include "ns3/trace-source-accessor.h"

namespace ns3 {

/**
 * \ingroup IEEE P1906 framework
 *
 * \class P1906MOL_MOTOR_Pos
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

class P1906MOL_MOTOR_Pos : public Object
{
public:
  static TypeId GetTypeId (void);
  P1906MOL_MOTOR_Pos (double x = 0, double y = 0, double z = 0);

  gsl_vector * pos;
  //! the pos_* values are intended for ns-3 attributes to be used for tracing
  TracedValue<double_t> pos_x;
  TracedValue<double_t> pos_y;
  TracedValue<double_t> pos_z;
  
  /**
   * TracedCallback signature for course change notifications.
   *
   * If the callback is connected using Config::ConnectWithoutContext()
   * omit the \c context argument from the signature.
   *
   * \param [in] context The context string, supplied by the Trace source.
   * \param [in] model The P1906MOL_MOTOR_Pos which is changing course.
   */
  typedef void (* posTracedCallback)
    (const std::string context, const Ptr<const P1906MOL_MOTOR_Pos> p);
  
  /**
   * Used to alert subscribers that a change in direction, velocity,
   * or position has occurred.
   */
  ns3::TracedCallback<Ptr<const P1906MOL_MOTOR_Pos> > pos_xTrace;
  ns3::TracedCallback<Ptr<const P1906MOL_MOTOR_Pos> > pos_yTrace;
  ns3::TracedCallback<Ptr<const P1906MOL_MOTOR_Pos> > pos_zTrace;
  
  /*
   * Methods related to recording position
   */
  //! record the position
  void setPos (double x, double y, double z);  
  //! record the object's position from the vector [x y z] 
  void setPos (gsl_vector * in_pos);
  //! record the object's position from the given position 
  void setPos (P1906MOL_MOTOR_Pos& p);

  /*
   * Methods related to retrieving the position
   */
  //! retrieve the position
  void getPos (double * x, double * y, double * z);
  //! retrieve the position into out_pos vector [x y z]
  void getPos (gsl_vector * out_pos);
    
  /*
   * Operations on position
   */  
  //! shift point by a scaled vector: new_pos = pos + d v_in
  void shiftPos (P1906MOL_MOTOR_Pos v_in, double d);
 
  /*
   * Methods related to displaying position
   */
  //! print the position
  void displayPos();

  virtual ~P1906MOL_MOTOR_Pos ();

};

std::ostream& operator<<(std::ostream& out, const P1906MOL_MOTOR_Pos& p);
std::istream& operator>>(std::istream& is, P1906MOL_MOTOR_Pos& p);

ATTRIBUTE_HELPER_HEADER (P1906MOL_MOTOR_Pos);
}

#endif /* P1906_MOL_MOTOR_POS */