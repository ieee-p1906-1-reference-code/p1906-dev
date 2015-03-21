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
 
/* \details This class extends the IEEE 1906 Motion class to implement different type of molecular motor motion.
 * Note that motion and time are related in that motion always updates simulation time.
 *
 * <pre> 
 * Molecular 3D Motor Motion
 *   Z ^     Y
 *     |  /                   +----->              
 *     |/                   ++ MOTOR                    
 *     +------>           +-++------------+        
 *            X           |               |        
 *    ++                  +---------------+        
 *    ++                                           
 *  MOLECULAR                MICROTUBULE           
 *    MOTOR                                        
 *                                                
 * UNBOUND MOTION               BOUND MOTION       
 * Next step random Gaussian    Next step along tube
 * </pre>
 */

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/p1906-mol-motor-motion.h"
#include "ns3/p1906-mol-motor-tube.h"
#include "ns3/p1906-mol-motor-pos.h"

#include "ns3/p1906-mol-motor-MathematicaHelper.h"
#include "ns3/p1906-mol-motor-MATLABHelper.h"
#include "ns3/p1906-metrics.h"
#include "ns3/p1906-mol-motor.h"
#include "ns3/p1906-mol-motor-pos.h"

#include "ns3/p1906-communication-interface.h"
#include "ns3/mobility-model.h"
#include "ns3/p1906-net-device.h"

//! for ODE test \todo remove before submitting
#include "ns3/p1906-mol-diffusion.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_Motion");

TypeId P1906MOL_MOTOR_Motion::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_Motion")
    .SetParent<P1906MOLMotion> ();
  return tid;
}

P1906MOL_MOTOR_Motion::P1906MOL_MOTOR_Motion ()
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
    
  NS_LOG_FUNCTION (this);
  NS_LOG_FUNCTION (this << "Created MOL Extended Motion Component");
}

//! assumes motor is within radius of a tube, otherwise it simply returns
//! if the motor is within radius of a tube, motor walks along along the tube until unbound (binding_time) or reaches end of tube
void P1906MOL_MOTOR_Motion::motorWalk(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> &pts, gsl_matrix * tubeMatrix, size_t segPerTube, vector<P1906MOL_MOTOR_VolSurface> & vsl)
{
  /** 
    See "Movements of Molecular Motors," Reinhard Lipowsky
	movement speed: ~1 um / sec
	bound time ~2 sec
	assumes startPt is on a tube in tubeMatrix
  */
  gsl_vector * segment = gsl_vector_alloc(6);
  gsl_vector * pt1 = gsl_vector_alloc(3);
  gsl_vector * pt2 = gsl_vector_alloc(3);
  P1906MOL_MOTOR_Pos Pos1;
  P1906MOL_MOTOR_Pos Pos2;
  //! tube radius nm
  double radius = 15; // [nm]
  //! motor movement rate (nm / sec)
  double movementRate = 1000; // [nm/s]
  //! motor mean binding time (s)
  //! double bindingTime = 2; // [s]
  double x1, y1, z1;
  double x2, y2, z2;
  //! motor mean binding probability (default 1.0)
  double binding_probability = 1.0; //! always bind for testing purposes
  //double binding_time = 1.0; //! sec \todo implement binding time
  Ptr<P1906MOL_Motor> motor = carrier->GetObject <P1906MOL_Motor> ();
  
  //! bind with a given probability
  if (gsl_rng_uniform(r) > binding_probability) //! \todo set realistic binding probability
  {
    printf ("(motorWalk) motor did not bind\n");
    return;
  }
  
  //! find the tube the motor is starting on
  size_t seg = P1906MOL_MOTOR_Field::findNearestTube(startPt, tubeMatrix, radius); //! \todo set tube radius (thickness) globally
  
  //! no tube is within the radius, so exit
  if (seg == ULONG_MAX)
  {
    printf ("(motorWalk) no tube is within radius: %f\n", radius);
    return;
  }
  
  //! record the current location
  P1906MOL_MOTOR_Pos Pos;
  Pos.setPos ( gsl_vector_get(startPt, 0),
               gsl_vector_get(startPt, 1),
               gsl_vector_get(startPt, 2) );
  pts.insert(pts.end(), Pos);
  //printf ("(motorWalk) recorded first location\n");
  //Pos.displayPos();
  
  //! walk along tube for distance determined by bound time
  //! segments are sequential in tubeMatrix of length segPerTube
  size_t segOfTube = seg % segPerTube; //! the current segment within the tube
  size_t segToGo = segPerTube - segOfTube; //! segments until the end of tube
  
  //printf ("(motorWalk) seg: %ld segOfTube: %ld segToGo: %ld\n", seg, segOfTube, segToGo);
  for (size_t i = seg; i < (seg + segToGo); i++)
  {
    //! walk to end of segment
    P1906MOL_MOTOR_Field::line(segment, tubeMatrix, i);

	//! record the position after moving to the end of the segment
    P1906MOL_MOTOR_Pos Pos;
	Pos.setPos ( gsl_vector_get(segment, 3), 
	             gsl_vector_get(segment, 4), 
			     gsl_vector_get(segment, 5) );
	pts.insert(pts.end(), Pos);
	//printf ("(motorWalk) segment(%ld) recorded position\n", i);
	//Pos.displayPos();
	
	//! get next-to-last point
	Pos1 = pts.at(pts.size() - 2);
	Pos1.getPos (&x1, &y1, &z1);
	P1906MOL_MOTOR_Field::point (pt1, x1, y1, z1);
	
	//! get last point
    Pos2 = pts.at(pts.size() - 1);
	Pos2.getPos (&x2, &y2, &z2);
    P1906MOL_MOTOR_Field::point (pt2, x2, y2, z2);

	//printf ("(motorWalk) distance: %f movementRate: %f time: %f\n", 
	//  P1906MOL_MOTOR_Field::distance(pt1, pt2), 
	//  movementRate, 
	//  P1906MOL_MOTOR_Field::distance(pt1, pt2) / movementRate);
    motor->updateTime(P1906MOL_MOTOR_Field::distance(pt1, pt2) / movementRate);
  }
}

//! print the position in pt
void P1906MOL_MOTOR_Motion::displayPos(gsl_vector *pt)
{
  printf ("Position: %g %g %g\n", 
    gsl_vector_get(pt, 0), 
	gsl_vector_get(pt, 1),
	gsl_vector_get(pt, 2));
}

//! motor is unbound from tube and floats via Brownian motion until it makes contact with a tube
//!   pts - locations of the motor during its random walk comprised of npts
//!   startPt - where the motor began its random walk
//!   timePeriod - length of each step of the walk
//!   returns the index of the contact segment in tubeMatrix
size_t P1906MOL_MOTOR_Motion::float2Tube(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> &pts, gsl_matrix * tubeMatrix, double timePeriod, vector<P1906MOL_MOTOR_VolSurface> & vsl)
{
  gsl_vector * currentPos = gsl_vector_alloc (3);
  gsl_vector * newPos = gsl_vector_alloc (3);
  int numPts = 0; //! total number of points traversed
  double timeout = 100; //! stop if no tube found
  int ts; //! nearest tube segment
  double radius = 15;
  Ptr<P1906MOL_Motor> motor = carrier->GetObject <P1906MOL_Motor> ();
  double D = 1.0; //! mass diffusivity (default)
  
  D = GetDiffusionConefficient ();
  
  //! begin at the starting point
  P1906MOL_MOTOR_Field::point (currentPos, 
    gsl_vector_get (startPt, 0), 
	gsl_vector_get (startPt, 1), 
	gsl_vector_get (startPt, 2));

  //! float to the nearest tube within a given radius
  for (int i = 0; i < timeout; i++)
  {
    P1906MOL_MOTOR_Pos Pos;
	Pos.setPos ( gsl_vector_get (currentPos, 0), 
	             gsl_vector_get (currentPos, 1), 
			     gsl_vector_get (currentPos, 2) );
	//printf ("(float2Tube) position\n");
	//Pos.displayPos ();
	pts.insert(pts.end(), Pos);
	numPts++; //! consider starting position the first point
	brownianMotion(r, currentPos, newPos, timePeriod, D, vsl);
	motor->updateTime(timePeriod);
    gsl_vector_set (currentPos, 0, gsl_vector_get (newPos, 0));
	gsl_vector_set (currentPos, 1, gsl_vector_get (newPos, 1));
	gsl_vector_set (currentPos, 2, gsl_vector_get (newPos, 2));
	ts = P1906MOL_MOTOR_Field::findNearestTube(currentPos, tubeMatrix, radius);
	if ( ts !=  -1 )
	{
	  printf ("motor contact with segment: %d\n", ts);
	  break; //! end after contact with tube
	}
  }

  return ts;
}

//! return newPos based upon Brownian motion from currentPos over timePeriod.
//! distance travelled will be a function of particle diameter, temperature, diffusion coefficient.
//! for simplicity, the second moment is \f$\bar{x^2} = 2 D t\f$, where \f$D\f$ is the mass diffusivity and \f$t\f$ is time.
//! note that Brownian motion landing on a receiver is a form of the "narrow escape" problem.
void P1906MOL_MOTOR_Motion::brownianMotion(gsl_rng * r, gsl_vector * currentPos, gsl_vector * newPos, double timePeriod, double D, vector<P1906MOL_MOTOR_VolSurface> & vsl)
{
  //! the new position is Gaussian with variance proportional to time taken: W_t - W_s ~ N(0, t - s)
  //! sigma is the standard deviation
  double sigma = sqrt(2 * D * timePeriod); /* sigma should be proportional to time */
  
  P1906MOL_MOTOR_Field::point (newPos, 
    gsl_vector_get (currentPos, 0) + gsl_ran_gaussian (r, sigma), /* x distance */
    gsl_vector_get (currentPos, 1) + gsl_ran_gaussian (r, sigma), /* y distance */
    gsl_vector_get (currentPos, 2) + gsl_ran_gaussian (r, sigma)  /* z distance */
  );
  
  //! check for reflection if contact with the volume surface of a P1906MOL_MOTOR_VolSurface::ReflectiveBarrier
  vector<P1906MOL_MOTOR_Pos> ipt;
  gsl_vector * segment = gsl_vector_alloc (6);
  P1906MOL_MOTOR_Pos cp, np;
  
  cp.setPos (currentPos);
  //printf ("(brownianMotion) current position\n");
  //cp.displayPos ();
  np.setPos (newPos);
  //printf ("(brownianMotion) new position\n");
  //np.displayPos ();
  
  P1906MOL_MOTOR_Field::line (segment, cp, np);
  //P1906MOL_MOTOR_Field::displayLine (segment);
  
  //printf ("(brownianMotion) vsl.size: %ld\n", vsl.size());
  
  //! there could be more than one reflective surface to check
  for (size_t i = 0; i < vsl.size(); i++)
  {
    //printf ("(brownianMotion) volume surface (%ld) Type: %d ReflectiveBarrier %d\n", i, vsl.at(i).getType(), P1906MOL_MOTOR_VolSurface::ReflectiveBarrier);
	//vsl.at(i).displayVolSurface();
	
    if (vsl.at(i).getType() == P1906MOL_MOTOR_VolSurface::ReflectiveBarrier)
    {
      vsl.at(i).sphereIntersections(segment, ipt);
	  
	  //printf ("(brownianMotion) ipt.size(): %ld\n", ipt.size());
	  
	  //! intersection with volume surface
	  if (ipt.size() != 0)
	  {
	    //printf ("(brownianMotion) surface intersection\n");
	    //ipt.front().displayPos ();
	  
	    //! reflect np from surface
	    vsl.at(i).reflect(cp, np);
	  
	    //! update the reflected position
        np.getPos (newPos);
        //printf ("(brownianMotion) after reflection\n");
        //np.displayPos ();
	  }
    }
  }
  
  //printf ("(brownianMotion) End\n");
}

//! implements a motor floating via Brownian motion for time steps with step lengths of timePeriod
int P1906MOL_MOTOR_Motion::freeFloat(Ptr<P1906MessageCarrier> carrier, gsl_rng * r, gsl_vector * startPt, vector<P1906MOL_MOTOR_Pos> & pts, int time, double timePeriod, vector<P1906MOL_MOTOR_VolSurface> & vsl)
{
  //P1906MOL_MOTOR_Tube tube;
  gsl_vector * currentPos = gsl_vector_alloc (3);
  gsl_vector * newPos = gsl_vector_alloc (3);
  P1906MOL_MOTOR_Field::point (currentPos, 
    gsl_vector_get (currentPos, 0), 
	gsl_vector_get (currentPos, 1), 
	gsl_vector_get (currentPos, 2));
  int numPts = 0;
  Ptr<P1906MOL_Motor> motor = carrier->GetObject <P1906MOL_Motor> ();
  double D = 1.0; //! mass diffusivity (default)
   
  D = GetDiffusionConefficient ();
  
  for (int i = 0; i < time; i++)
  {
 	P1906MOL_MOTOR_Pos Pos;
	Pos.setPos ( gsl_vector_get (currentPos, 0), 
	             gsl_vector_get (currentPos, 1), 
			     gsl_vector_get (currentPos, 2) );
	pts.insert(pts.end(), Pos);
	
	brownianMotion(r, currentPos, newPos, timePeriod, D, vsl);
	motor->updateTime(timePeriod);
    gsl_vector_set (currentPos, 0, gsl_vector_get (newPos, 0));
	gsl_vector_set (currentPos, 1, gsl_vector_get (newPos, 1));
	gsl_vector_set (currentPos, 2, gsl_vector_get (newPos, 2));
    numPts++;
  }
  
  return numPts;
}

//! this is called inside the core Medium class before reception
//! this is where the main action happens: motor movement occurs here
//! field is actually the extended P1906MOL_MOTOR_MicrotubulesField of the transmitting node
//! <pre>
//!          The Surface Measures Flux, Constrains Particle 
//!                 Motion, and Defines a Receiver
//!                     _,.,---''''''''---..__
//!                _.-''                      `-.._
//!             ,-'                                `..
//!          ,-' __                                   `._
//!        ,'  ,'  `-.  Motor received here              `.
//!      ,'   /      _\____                                .
//!     /    |    X   |   /                                 `.
//!    /      \      ,'  /____                                .
//!   /        `._,,'        /                                 .
//!  |    Receiver Surface  /                                   |
//!  |                     /    Motor transmitted here          |
//! |                     -------X   _,''   ``._                |
//! |                               /           \               |
//! |                              /             \              |
//!  |                            |       X       |             /
//!  \                            `.             .'            /
//!   \                            |             |            ,'
//!    \                           `-.         ,'            ,'
//!     `.                            `..__,,,'             /
//!       `.                       FluxMeter Surface      ,'
//!         `.                                          ,'
//!           `.                                     _,'
//!             `-._                              ,,'
//!                 `-..__                  _,.-''
//!                       ``---........---''
//!                Reflective Barrier Volume Surface
//!           
//!</pre>
double P1906MOL_MOTOR_Motion::ComputePropagationDelay (Ptr<P1906CommunicationInterface> src,
  		                                  Ptr<P1906CommunicationInterface> dst,
  		                                  Ptr<P1906MessageCarrier> message,
  		                                  Ptr<P1906Field> field)
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  double timePeriod = 100;
  char plot_filename[256];
  //! fix the units used here with those passed in via _RUN_MOTOR_CHANNEL_CAPACITY_
  float distanceMultiplier = pow(10.0, 9); //! convert meters to nanometers
  double D = 1.0; //! mass diffusivity (default)
  
  //! begin test of ODE - \todo remove before submitting
  P1906MOL_ExtendedDiffusion dif;
  dif.displayODE ();
  dif.unitTest_ODE ();
  dif.unitTest_diffusion ();
  //! end test of ODE

  NS_LOG_FUNCTION (this << "beginning ComputePropagationDelay");
  
  //! reusing the coefficient from the MOL model that is entered at run time
  //! this can be called directly from brownianMotion 
  D = GetDiffusionConefficient ();
  
  Ptr<MobilityModel> srcMobility = src->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  Ptr<MobilityModel> dstMobility = dst->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();

  Vector sv = srcMobility->GetPosition();
  Vector dv = dstMobility->GetPosition();
  
  //printf ("(ComputePropagationDelay) transmitter location x: %lf y: %lf z: %lf\n", sv.x, sv.y, sv.z);
  //printf ("(ComputePropagationDelay) receiver location x: %lf y: %lf z: %lf\n", dv.x, dv.y, dv.z);
  
  Ptr<P1906MOL_Motor> motor = message->GetObject <P1906MOL_Motor> ();
 
  //! reset the motor's timer
  motor->initTime();
   
  //! Starting position is the transmitting node location
  P1906MOL_MOTOR_Field::point (startPt, sv.x, sv.y, sv.z);
    
  //! Receiver volume surface center comes from the receiver Node location
  P1906MOL_MOTOR_Pos dvol;
  printf ("(ComputePropagationDelay) position dv.x: %lf multiplier: %lf\n", dv.x, distanceMultiplier);
  dvol.setPos (dv.x * distanceMultiplier, dv.y, dv.z);
  motor->addVolumeSurface(dvol, (dv.x * distanceMultiplier)/1.0001, P1906MOL_MOTOR_VolSurface::Receiver);
 
  //! add a reflective barrier around the source and destination
  dvol.setPos (sv.x, sv.y, sv.z);
  motor->addVolumeSurface(dvol, distanceMultiplier * (dv.x + (0.1 * dv.x)), P1906MOL_MOTOR_VolSurface::ReflectiveBarrier);

  //! volume surface must overlap with destination in order for the test to end
  motor->displayVolSurfaces();
  
  /*
   * move randomly until destination reached
   */
  motor->setStartingPoint(startPt);
  
  float2Destination(motor, timePeriod);
  
  NS_LOG_FUNCTION (this << "[propagation time]" << motor->getTime());
  sprintf (plot_filename, "float2destination_%lf_%lf.mma", sv.x, dv.x * distanceMultiplier);
  mathematica.connectedPoints2Mma(motor->pos_history, plot_filename);
  
  NS_LOG_FUNCTION (this << "completed ComputePropagationDelay");
  
  return motor->getTime();
}

//! this is called inside the core Medium class before reception
//! this returns the receivedMessageCarrier that appears at the receiver
Ptr<P1906MessageCarrier> P1906MOL_MOTOR_Motion::CalculateReceivedMessageCarrier(Ptr<P1906CommunicationInterface> src,
  		                                                           Ptr<P1906CommunicationInterface> dst,
  		                                                           Ptr<P1906MessageCarrier> motor,
  		                                                           Ptr<P1906Field> field)
{
  //! 'message' above is really the message carrier (motor)

  NS_LOG_FUNCTION (this << "Do nothing for motor");
  return motor;
}

//! motor uses Brownian motion until the destination volume is reached
void P1906MOL_MOTOR_Motion::float2Destination(Ptr<P1906MessageCarrier> carrier, double timePeriod)
{
  gsl_vector * newPos = gsl_vector_alloc (3);
  P1906MOL_MOTOR_Pos Pos;
  Ptr<P1906MOL_Motor> motor = carrier->GetObject <P1906MOL_Motor> ();
  double D = 1.0; //! mass diffusivity (default)
    
  D = GetDiffusionConefficient ();
  
  Pos.setPos ( gsl_vector_get (motor->current_location, 0),
               gsl_vector_get (motor->current_location, 1),
			   gsl_vector_get (motor->current_location, 2) );
  motor->pos_history.insert (motor->pos_history.end(), Pos);
  
  //printf ("(float2Destination) motor location:\n");
  //displayPos(current_location);
	
  //! float until in destination volume
  while (!motor->inDestination())
  {
	brownianMotion(motor->r, motor->current_location, newPos, timePeriod, D, motor->vsl);
	motor->updateTime(timePeriod);
    gsl_vector_set (motor->current_location, 0, gsl_vector_get (newPos, 0));
	gsl_vector_set (motor->current_location, 1, gsl_vector_get (newPos, 1));
	gsl_vector_set (motor->current_location, 2, gsl_vector_get (newPos, 2));
	
    //printf ("(float2Destination) motor location:\n");
    //displayPos(current_location);
	
	P1906MOL_MOTOR_Pos Pos;
    Pos.setPos ( gsl_vector_get (motor->current_location, 0),
                 gsl_vector_get (motor->current_location, 1),
			     gsl_vector_get (motor->current_location, 2) );
    motor->pos_history.insert (motor->pos_history.end(), Pos);
  }
}

//! use microtubules, if available, Brownian motion otherwise until destination is reached
void P1906MOL_MOTOR_Motion::move2Destination(Ptr<P1906MessageCarrier> carrier, gsl_matrix * tubeMatrix, size_t segPerTube, double timePeriod, vector<P1906MOL_MOTOR_Pos> & pts)
{
  int timeout = 100; //! in case motor never reaches destination
  int loops = 0; //! keep track of iterations
  Ptr<P1906MOL_Motor> motor = carrier->GetObject <P1906MOL_Motor> ();
  
  while (!motor->inDestination() && (loops < timeout))
  {
    //! returns the index of the segment in tubeMatrix to which the motor is bound 
    float2Tube(motor, motor->r, motor->current_location, pts, tubeMatrix, timePeriod, motor->vsl);
	motor->setLocation(pts.back());
    //printf ("(move2Destination) current location after float2Tube\n");
	//displayLocation();
	//pts.back().displayPos();
	//! walk along tube until end of tube or unbound
	motorWalk(motor, motor->r, motor->current_location, pts, tubeMatrix, segPerTube, motor->vsl);
	motor->setLocation(pts.back());
    //printf ("(move2Destination) current location after motorWalk\n");
	//pts.back().displayPos();
	//displayLocation();
	loops++;
  }
}

P1906MOL_MOTOR_Motion::~P1906MOL_MOTOR_Motion ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
