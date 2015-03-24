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
 
/* \details This class implements the molecular motor.
 *
 * <pre>
 *                                                upper-right corner
 *                               +--------------------+             
 *                               |                    |             
 *                               |                    |             
 *                   ++          |    DESTINATION     |             
 *    +-->           ++          |       VOLUME       |             
 *    |           MOLECULAR      |                    |             
 *    |             MOTOR        |                    |             
 *    +                          |                    |             
 * STARTING                      +--------------------+             
 *  POINT                  lower-left corner                                             
 * </pre>
 *
 * Molecular motors are derived from Message Carriers and are created as fast as Packets arrive. 
 * Thus, there may be many motors operating in parallel with one another depending upon the packet rate and motor propagation rate.
 * There is currently no motor interaction, but this is something that should be considered.
 */

#include "ns3/log.h"

#include "ns3/p1906-mol-motor.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_Motor");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_Motor);

TypeId P1906MOL_Motor::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_Motor")
    .SetParent<P1906MOLMessageCarrier> ()
	.AddConstructor<P1906MOL_Motor> ()
	.AddAttribute ("Initial X Location",
	               "The motor starting Euclidean X position.",
				   ObjectVectorValue (),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_x),
				   MakeDoubleChecker<double> ())
	.AddAttribute ("Initial Y Location",
	               "The motor starting Euclidean Y position.",
				   ObjectVectorValue (),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_y),
				   MakeDoubleChecker<double> ())
	.AddAttribute ("Initial Z Location",
	               "The motor starting Euclidean Z position.",
				   ObjectVectorValue (),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_z),
				   MakeDoubleChecker<double> ())
	//.AddAttribute ("Current location",
	//               "The current motor location.",
	//			   P1906MOL_MOTOR_PosValue (),
	//			   MakeP1906MOL_MOTOR_PosAccessor (&P1906MOL_Motor::current_location),
	//			   MakeP1906MOL_MOTOR_PosChecker ())
	;
  return tid;
}

//! The motor constructor creates a motor, but no Receiver volume. This must be set using addVolumeSurface.
//! Without a P1906MOL_MOTOR_VolSurface::Receiver volume, their is no destination and the motor will wander forever.
P1906MOL_Motor::P1906MOL_Motor ()
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
  //current_location = gsl_vector_alloc (3);
  current_location.setPos (start_x, start_y, start_z);
    
  //! start with an empty record of for tracking position
  pos_history.clear();
  
  //! random number generation structures and initialization
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  gsl_rng_env_setup();
}

std::ostream& operator<<(std::ostream& out, const P1906MOL_Motor& m)
{
  return out << m.t.time << " " << m.current_location;
}

std::istream& operator>>(std::istream& is, P1906MOL_Motor& m)
{
  return is >> m.t.time >> m.current_location;
}

//! create and store a new volume surface of any type: FluxMeter, ReflectiveBarrier, Receiver
void P1906MOL_Motor::addVolumeSurface(P1906MOL_MOTOR_Pos v_c, double v_radius, P1906MOL_MOTOR_VolSurface::typeOfVolume v_type)
{
  P1906MOL_MOTOR_VolSurface vs;
  
  NS_LOG_FUNCTION(this);
  vs.setVolume(v_c, v_radius);
  vs.setType (v_type);
  vsl.insert(vsl.end(), vs);
  
  NS_LOG_DEBUG ("volume surface added: " << vs);
}

//! this is where the motor starts, for example, the location of the transmitter
//! this function takes precedence over the attribute settings
void P1906MOL_Motor::setStartingPoint(gsl_vector * pt)
{
  NS_LOG_FUNCTION(this);
  current_location.setPos (pt);
  start_x = gsl_vector_get (pt, 0);
  start_y = gsl_vector_get (pt, 1);
  start_z = gsl_vector_get (pt, 2);
}

//! display all the volume surfaces recognizing the motor
void P1906MOL_Motor::displayVolSurfaces()
{
  for (size_t i = 0; i < vsl.size(); i++)
    vsl.at(i).displayVolSurface();
}

//! return true if motor is in a destination (P1906MOL_MOTOR_VolSurface::Receiver) volume, false otherwise
bool P1906MOL_Motor::inDestination()
{
  bool inDest = false;
  size_t numDest = 0;
  
  NS_LOG_FUNCTION(this);
  //! find the Receiver space(s)
  for (size_t i = 0; i < vsl.size(); i++)
  {
    if (vsl.at(i).getType() == P1906MOL_MOTOR_VolSurface::Receiver)
	{
	  numDest++;
	  if (vsl.at(i).isInsideVolSurf(current_location))
	    inDest = true;
	}
  }
  
  if (numDest < 1)
  {
    printf ("(inDestination) Warning! No destination P1906MOL_MOTOR_VolSurface::Receiver volume found!\n");
	NS_LOG_WARN ("No destination P1906MOL_MOTOR_VolSurface::Receiver volume found!");
  }
  
  return inDest;
}

//! set the current motor location to pt
void P1906MOL_Motor::setLocation(P1906MOL_MOTOR_Pos pt)
{
  double x, y, z;
  
  NS_LOG_FUNCTION(this);
  pt.getPos (&x, &y, &z);
  current_location.setPos (x, y, z);
}

//! set the current location to the 3D Cartesian coordinates
void P1906MOL_Motor::setLocation(double x, double y, double z)
{
  NS_LOG_FUNCTION(this);
  current_location.setPos (x, y, z);
}

//! print the current motor location
void P1906MOL_Motor::displayLocation()
{
  double x, y, z;
  
  NS_LOG_FUNCTION(this);
  current_location.getPos (&x, &y, &z);
  printf ("current location: %f %f %f\n", x, y, z);
}

//! return simulation time
double P1906MOL_Motor::getTime()
{
  NS_LOG_FUNCTION(this);
  return t.time;
}

//! initialize simulation time
void P1906MOL_Motor::initTime()
{
  NS_LOG_FUNCTION(this);
  t.time = 0;
}

//! update simulation time
void P1906MOL_Motor::updateTime(double event_time)
{
  NS_LOG_FUNCTION(this);
  t.time += event_time;
  NS_LOG_DEBUG ("event occurred of duration: " << event_time);
}

//! return the elapsed time since the motor time was last initialized
double P1906MOL_Motor::propagationDelay()
{
  NS_LOG_FUNCTION(this);
  return getTime();
}

P1906MOL_Motor::~P1906MOL_Motor ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
