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

/* \details This class implements location related methods
 *
 * <pre>
 * Currently very simple:
 * +--------------------+
 * |                    |
 * | X, Y, Z Coordinate |
 * |                    |
 * +--------------------+
 * \todo add timestamp
 * \todo add resolution (uncertainty related to Clause 6.8 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and 
 *       Molecular Communication Framework -> void P1906_Metrics::Positioning_Accuracy_of_Message_Carriers())
 * </pre>
 */
 
#include "ns3/log.h"
#include "ns3/p1906-mol-motor-pos.h"

#include <string>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_Pos");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_MOTOR_Pos);

TypeId P1906MOL_MOTOR_Pos::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_Pos")
    .SetParent<Object> ()
	.AddConstructor<P1906MOL_MOTOR_Pos> ()
	.AddTraceSource ("XLocation",
	                 "The current Euclidean X position.",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_Pos::pos_x),
					 "ns3:posTracedCallback")
	.AddTraceSource ("YLocation",
	                 "The current Euclidean Y position.",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_Pos::pos_x),
					 "ns3:posTracedCallback")
	.AddTraceSource ("ZLocation",
	                 "The current Euclidean Z position.",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_Pos::pos_z),
					 "ns3:posTracedCallback")
  ;
  return tid;
}

//! initial location values are optional
P1906MOL_MOTOR_Pos::P1906MOL_MOTOR_Pos (double x, double y, double z)
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

  NS_LOG_FUNCTION(this);
  pos = gsl_vector_alloc (3);
  gsl_vector_set (pos, 0, x);
  gsl_vector_set (pos, 1, y);
  gsl_vector_set (pos, 2, z);
  pos_x = x;
  pos_y = y;
  pos_z = z;
}

ATTRIBUTE_HELPER_CPP (P1906MOL_MOTOR_Pos);

std::ostream& operator<<(std::ostream& out, const P1906MOL_MOTOR_Pos& p)
{
  return out << gsl_vector_get(p.pos, 0) << " " << gsl_vector_get(p.pos, 1) << " " << gsl_vector_get(p.pos, 2);
}

std::istream& operator>>(std::istream& is, P1906MOL_MOTOR_Pos& p)
{
  double x, y, z; 
  
  is >> x >> y >> z;
  gsl_vector_set(p.pos, 0, x);
  gsl_vector_set(p.pos, 1, y);
  gsl_vector_set(p.pos, 2, z);
  
  p.pos_x = x;
  p.pos_y = y;
  p.pos_z = z;
  
  return is;
}

//! set the object's position from the vector [x y z] 
void P1906MOL_MOTOR_Pos::setPos (gsl_vector * in_pos)
{
  gsl_vector_memcpy (pos, in_pos);
  pos_x = gsl_vector_get (pos, 0);
  pos_y = gsl_vector_get (pos, 1);
  pos_z = gsl_vector_get (pos, 2);
  NS_LOG_INFO(this << " " << pos_x << " " << pos_y << " " << pos_z);
}

//! set the object's position
void P1906MOL_MOTOR_Pos::setPos (double x, double y, double z)
{
  gsl_vector_set (pos, 0, x);
  gsl_vector_set (pos, 1, y);
  gsl_vector_set (pos, 2, z);
  pos_x = x;
  pos_y = y;
  pos_z = z;
  NS_LOG_INFO(this << " " << pos_x << " " << pos_y << " " << pos_z);
}

void P1906MOL_MOTOR_Pos::setPos (P1906MOL_MOTOR_Pos& p)
{
  gsl_vector_set (pos, 0, p.pos_x);
  gsl_vector_set (pos, 1, p.pos_y);
  gsl_vector_set (pos, 2, p.pos_z);
  pos_x = p.pos_x;
  pos_y = p.pos_y;
  pos_z = p.pos_z;
  NS_LOG_INFO(this << " " << pos_x << " " << pos_y << " " << pos_z);
}

//! retrieve the position into out_pos vector [x y z]
void P1906MOL_MOTOR_Pos::getPos (gsl_vector * out_pos)
{
  gsl_vector_memcpy (out_pos, pos);
  NS_LOG_INFO(this << " " << pos_x << " " << pos_y << " " << pos_z);
}

//! retrieve the position
void P1906MOL_MOTOR_Pos::getPos (double * x, double * y, double * z)
{
  *x = pos_x;
  *y = pos_y;
  *z = pos_z;
  //*x = gsl_vector_get (pos, 0);
  //*y = gsl_vector_get (pos, 1);
  //*z = gsl_vector_get (pos, 2);
  NS_LOG_INFO(this << " " << *x << " " << *y << " " << *z);
}

//! shift point by a scaled vector: new_pos = pos + d v_in
void P1906MOL_MOTOR_Pos::shiftPos (P1906MOL_MOTOR_Pos v_in, double d)
{
  gsl_vector * v = gsl_vector_alloc (3);
  
  NS_LOG_FUNCTION(this);
  
  //! get the vector
  v_in.getPos (v);
  
  //! update pos to pos + d v
  for (size_t i = 0; i< 3; i++)
    gsl_vector_set (pos, i, gsl_vector_get(pos, i) + (gsl_vector_get (v, i) * d));

  pos_x = gsl_vector_get (pos, 0);
  pos_y = gsl_vector_get (pos, 1);
  pos_z = gsl_vector_get (pos, 2);

  NS_LOG_DEBUG ("v_in " << v_in << " d: " << d);
  NS_LOG_DEBUG ("shifted position: " << this);
}

//! print the position
void P1906MOL_MOTOR_Pos::displayPos()
{
  double x, y, z;
  
  getPos (&x, &y, &z);
  NS_LOG_INFO ("Position: " << x << " " << y << " " << z);
}

P1906MOL_MOTOR_Pos::~P1906MOL_MOTOR_Pos ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
