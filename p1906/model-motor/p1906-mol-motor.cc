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
#include "ns3/string.h"
#include "ns3/p1906-mol-motor.h"
#include "ns3/p1906-mol-motor-MathematicaHelper.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_Motor");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_Motor);

//! Motor references: Adio et al. 2009 PMID: 19242550; Block, 1998 PMID: 9546384; Lakamper et al. 2003 PMID: 12609885; Higuchi and Endow, 2002 PMID: 11792544
//! Cargo references: DOI: 10.1007/s12195-012-0260-9
//! Note that multiple motors can carry a single, large cargo. Here we assume one motor per cargo for initial simplification
TypeId P1906MOL_Motor::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_Motor")
    .SetParent<P1906MOLMessageCarrier> ()
	.AddConstructor<P1906MOL_Motor> ()
	.AddAttribute ("InitialXLocation",
	               "The motor starting Euclidean X position.",
				   DoubleValue (0),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_x),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("InitialYLocation",
	               "The motor starting Euclidean Y position.",
				   DoubleValue (0),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_y),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("InitialZLocation",
	               "The motor starting Euclidean Z position.",
				   DoubleValue (0),
				   MakeDoubleAccessor (&P1906MOL_Motor::start_z),
				   MakeDoubleChecker<double_t> ())
	//.AddTraceSource ("Current_Location",
	//              "The location of the motor [nm] (x,y,z).",
	//			   	P1906MOL_MOTOR_PosValue (P1906MOL_MOTOR_Pos (0, 0, 0)),
	//			   	MakeP1906MOL_MOTOR_PosAccessor (&P1906MOL_Motor::current_location),
    // 			   	MakeP1906MOL_MOTOR_PosChecker ())
	.AddAttribute ("Source",
					"The species and tissue from which the motor originated.",
					StringValue ("N. crassa; Kinesin 1"),
					MakeStringAccessor (&P1906MOL_Motor::m_source),
					MakeStringChecker ())
	.AddAttribute ("DomainStructure",
					"The motor's protein structure.",
					StringValue ("Homodimer"),
					MakeStringAccessor (&P1906MOL_Motor::m_domain_structure),
					MakeStringChecker ())
	.AddAttribute ("Lifetime",
					"Time from motor creation to disintegration [s].",
					DoubleValue (120),
					MakeDoubleAccessor (&P1906MOL_Motor::m_lifetime),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("IsAlive",
					"Whether the motor has lifetime remaining [boolean].",
					BooleanValue (true),
					MakeBooleanAccessor (&P1906MOL_Motor::isAlive),
					MakeBooleanChecker ())
	.AddAttribute ("Mass",
					"Mass of motor [megadaltons].",
					DoubleValue (1.5),
					MakeDoubleAccessor (&P1906MOL_Motor::m_mass),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("Directionality",
					"Whether motor moves toward + end of microtubule, otherwise - end (kinesin moves toward +) [boolean].",
					BooleanValue (),
					MakeBooleanAccessor (&P1906MOL_Motor::m_directionality),
					MakeBooleanChecker ())
	.AddAttribute ("Processivity",
					"Expected motor walking distance, e.g. run length, [(+/- 0.09) micrometers].",
					DoubleValue (1.75),
					MakeDoubleAccessor (&P1906MOL_Motor::m_processivity),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("Velocity",
					"Motor walking speed ~60-120 (8-nm steps) [micrometers/minute].",
					DoubleValue (120),
					MakeDoubleAccessor (&P1906MOL_Motor::m_velocity),
					MakeDoubleChecker<double_t> ())	
	.AddAttribute ("ForceGeneration",
					"Force generated by the motor during walking [pN].",
					DoubleValue (8),
					MakeDoubleAccessor (&P1906MOL_Motor::m_force),
					MakeDoubleChecker<double_t> ())
	//! mean Brownian motion displacement is sqrt(r^2(x, y, z)) = sqrt(6 * D * t))
	.AddAttribute ("CargoDiameter",
					"Diameter of cargo carried by motor (silica bead) [nm].",
					DoubleValue (200),
					MakeDoubleAccessor (&P1906MOL_Motor::m_cargo_diameter),
					MakeDoubleChecker<double_t> ())
	//! assuming DNA is the message/cargo on a glass bead
	.AddAttribute ("CargoMass",
					"Mass of DNA is a factor of ~660 with the number of base pairs [g/mol].",
					DoubleValue (660),
					MakeDoubleAccessor (&P1906MOL_Motor::m_cargo_mass),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("Diameter",
					"Diameter of motor (need to find this) [nm].",
					DoubleValue (100),
					MakeDoubleAccessor (&P1906MOL_Motor::m_diameter),
					MakeDoubleChecker<double_t> ())	
	//! Church, G. M.; Gao, Y.; Kosuri, S. (2012). "Next-Generation Digital Information Storage in DNA". Science 337 (6102): 1628
	.AddAttribute ("MessageInformationDensity",
					"DNA information storage density (petabits) [10^15 bits/mm^3].",
					DoubleValue (5.5),
					MakeDoubleAccessor (&P1906MOL_Motor::m_message_information_density),
					MakeDoubleChecker<double_t> ())	
	.AddAttribute ("CollisionType",
					"Motor-motor collision reaction [0: reflect 1: absorb, 2: join].",
					IntegerValue (0),
					MakeIntegerAccessor (&P1906MOL_Motor::m_collision_type),
					MakeIntegerChecker<uint32_t> ())
	.AddAttribute ("BindingRate",
					"Probability of motor binding to microtubule [probability].",
					DoubleValue (1.0),
					MakeDoubleAccessor (&P1906MOL_Motor::m_binding_rate),
					MakeDoubleChecker<double_t> ())
	//! Structural Basis for Kinesin-1:Cargo Recognition
	//.AddAttribute ("Affinity",
	//				"Affinity of motor to target receiver [?].",
	//				DoubleValue (),
	//				MakeDoubleAccessor (&P1906MOL_Motor::m_target_affinity),
	//				MakeDoubleChecker<double_t> ())
	.AddAttribute ("EnergyConsumed",
					"Steps for each ATP hydrolyzed at 1 mM ATP [ATP/step].",
					DoubleValue (1.0),
					MakeDoubleAccessor (&P1906MOL_Motor::m_energy_consumed),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("LocalizationError",
					"Error in motor localization (assume unit normal variance for now) [sigma].",
					DoubleValue (0.0),
					MakeDoubleAccessor (&P1906MOL_Motor::m_localization_error),
					MakeDoubleChecker<double_t> ())
	.AddAttribute ("SupersystemDegradation",
					"Impact of motor on system performance [percent loss of performance/motor].",
					DoubleValue (0.0),
					MakeDoubleAccessor (&P1906MOL_Motor::m_supersystem_degredation),
					MakeDoubleChecker<double_t> ())
	;
  return tid;
}

//! The motor constructor creates a motor, but no Receiver volume. This must be set using addVolumeSurface.
//! Without a P1906MOL_MOTOR_VolSurface::Receiver volume, their is no destination and the motor will wander forever.
P1906MOL_Motor::P1906MOL_Motor () :
start_x(0), 
start_y(0), 
start_z(0)
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
  
  m_id = reinterpret_cast<uint64_t>(this);
}

std::ostream& operator<<(std::ostream& out, const P1906MOL_Motor& m)
{
  return out << m.t.time << m.current_location;
}

std::istream& operator>>(std::istream& is, P1906MOL_Motor& m)
{
  return is >> m.t.time >> m.current_location;
}

//! create and store a new volume surface of any type: FluxMeter, ReflectiveBarrier, Receiver
void P1906MOL_Motor::addVolumeSurface(P1906MOL_MOTOR_Pos v_c, double v_radius, P1906MOL_MOTOR_VolSurface::typeOfVolume v_type)
{
  P1906MOL_MOTOR_VolSurface vs;
  
  vs.setVolume(v_c, v_radius);
  vs.setType (v_type);
  vsl.insert(vsl.end(), vs);
  NS_LOG_DEBUG ("volume surface added: " << vs);
}

//! this is where the motor starts, for example, the location of the transmitter
//! this function takes precedence over the attribute settings
void P1906MOL_Motor::setStartingPoint(gsl_vector * pt)
{
  current_location.setPos (pt);
  start_x = gsl_vector_get (pt, 0);
  start_y = gsl_vector_get (pt, 1);
  start_z = gsl_vector_get (pt, 2);
  NS_LOG_INFO (current_location);
}

//! display all the volume surfaces recognizing the motor
void P1906MOL_Motor::displayVolSurfaces()
{
  for (size_t i = 0; i < vsl.size(); i++)
    NS_LOG_INFO (vsl.at(i));
}

//! write all the volume surfaces to volsurface_<volType>_<motor_id>.mma
void P1906MOL_Motor::writeVolSurfaces()
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  char buf[256];
  
  for (size_t i = 0; i < vsl.size(); i++)
  {
	//! add m_id to filename to distinguish different motors
	sprintf (buf, "volsurface_%d_%lu.mma", vsl.at(i).volType, (unsigned long) m_id);
	NS_LOG_DEBUG ("writing volume surface file: " << buf);
    mathematica.volSurfacePlot(vsl.at(i).center, vsl.at(i).radius, buf);
  }
}

//! return true if motor is in a destination (P1906MOL_MOTOR_VolSurface::Receiver) volume, false otherwise
bool P1906MOL_Motor::inDestination()
{
  bool inDest = false;
  size_t numDest = 0;
  
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
	NS_LOG_WARN ("No destination P1906MOL_MOTOR_VolSurface::Receiver volume found!");
  }
  NS_LOG_INFO (inDest);
  return inDest;
}

//! set the current motor location to pt
void P1906MOL_Motor::setLocation(P1906MOL_MOTOR_Pos pt)
{
  double x, y, z;
  
  pt.getPos (&x, &y, &z);
  current_location.setPos (x, y, z);
  NS_LOG_INFO (current_location);
}

//! set the current location to the 3D Cartesian coordinates
void P1906MOL_Motor::setLocation(double x, double y, double z)
{
  current_location.setPos (x, y, z);
  NS_LOG_INFO (current_location);
}

//! print the current motor location
void P1906MOL_Motor::displayLocation()
{
  double x, y, z;
  
  current_location.getPos (&x, &y, &z);
  NS_LOG_INFO (current_location);
}

//! return simulation time
double P1906MOL_Motor::getTime()
{
  NS_LOG_INFO (t.time);
  return t.time;
}

//! initialize simulation time
void P1906MOL_Motor::initTime()
{
  t.time = 0;
  NS_LOG_INFO (t.time);
}

//! update simulation time
void P1906MOL_Motor::updateTime(double event_time)
{
  t.time += event_time;
  NS_LOG_DEBUG ("event duration: " << event_time);
  NS_LOG_INFO ("motor time: " << t.time);
  NS_LOG_INFO ("motor location: " << current_location);
}

//! return the elapsed time since the motor time was last initialized
double P1906MOL_Motor::propagationDelay()
{
  NS_LOG_INFO (getTime());
  return getTime();
}

P1906MOL_Motor::~P1906MOL_Motor ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
