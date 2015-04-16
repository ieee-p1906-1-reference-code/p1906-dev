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
 
 /* \details This class implements a volume surface, which can be used for multiple purposes:
  * (1) FluxMeter - measure flux through the volume surface
  * (2) ReflectiveBarrier - act as reflective bounding surface
  * (3) Receiver - act as a motor destination volume
  *
  * <pre>
  *       The Surface Measures Flux and
  *   	  Can Constrain Particle Motion
  *                 _,,----.
  *             _,'          `.
  *            /               `.  Tubes    ____
  *           /             ...,-+----''''''
  *          |  _,.--'           |
  *       ,.-+''  _,,  ,         |
  *   -''    _,,-'  _,'          /
  * Tubes .-'  \  ,'            /
  *    ''      _;:           _,'
  *          ,'   `-..____,,'
  *       ,-'    Volume Surface
  *      '
  * </pre>
  */

#include "ns3/log.h"

#include "ns3/p1906-mol-motor-microtubule.h"
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-tube.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_VolSurface");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_MOTOR_VolSurface);

//! \todo Trace should include flux and reflections
TypeId P1906MOL_MOTOR_VolSurface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_VolSurface")
    .SetParent<P1906MOL_MOTOR_Field> ()
	.AddConstructor<P1906MOL_MOTOR_VolSurface> ()
	.AddAttribute ("TypeOfVolume",
			   "Type of surface volume [typeOfVolume: 1/2/3].",
				IntegerValue (Receiver),
				MakeIntegerAccessor (&P1906MOL_MOTOR_VolSurface::volType),
				MakeIntegerChecker<uint32_t> ())
	.AddAttribute ("Radius",
			   "Radius of surface volume [nm].",
				DoubleValue (100.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::radius),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("CenterX",
	            "The x component of the volume surface center [nm].",
	            DoubleValue (0.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::m_center_x),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("CenterY",
	            "The y component of the volume surface center [nm].",
	            DoubleValue (0.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::m_center_y),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("CenterZ",
	            "The z component of the volume surface center [nm].",
	            DoubleValue (0.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::m_center_z),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("Sensitivity", // this should be moved to the specificity component
	            "The probability of detecting a message [probability] (only meaningful if Receiver).",
	            DoubleValue (1.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::m_sensitivity),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("Specificity", // this should be moved to the specificity component
	            "The probability of detecting a false message [probability] (only meaningful if Receiver).",
	            DoubleValue (0.0),
				MakeDoubleAccessor (&P1906MOL_MOTOR_VolSurface::m_specificity),
				MakeDoubleChecker<double_t> ())
				// LOG these trace sources and post process
	.AddTraceSource ("MessagesReceived", // track in motion component
	                 "The number of messages received [-] (only meaningful if Receiver).",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_VolSurface::m_messages_received),
					 "ns3::DoubleCallback")
	.AddTraceSource ("Bandwidth", // track in motion component
	                 "The mean information reception rate [bits/s] (only meaningful if Receiver).",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_VolSurface::m_bandwidth),
					 "ns3::DoubleCallback")
	.AddTraceSource ("Delay", // track in motion component
	                 "The mean received message delay [s] (only meaningful if Receiver).",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_VolSurface::m_delay),
					 "ns3::DoubleCallback")
	.AddTraceSource ("Reflections", // track here
	                 "The number of motors reflected from the surface [-] (only meaningful if ReflectiveBarrier).",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_VolSurface::m_reflections),
					 "ns3::DoubleCallback")
	.AddTraceSource ("Flux", // track here
	                 "The flow of particles through the surface [particles/s] (only meaningful if FluxMeter).",
				     MakeTraceSourceAccessor (&P1906MOL_MOTOR_VolSurface::m_flux),
					 "ns3::DoubleCallback")
  ;
  return tid;
}

P1906MOL_MOTOR_VolSurface::P1906MOL_MOTOR_VolSurface () :
m_center_x(0), 
m_center_y(0), 
m_center_z(0),
radius(100),
volType(Receiver)
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
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  gsl_rng_env_setup();
  
  // initialize traced values
  m_messages_received = 0.0;
  m_bandwidth = 0.0;
  m_delay = 0.0;
  m_reflections = 0.0;
  m_flux = 0.0;
}

std::ostream& operator<<(std::ostream& out, const P1906MOL_MOTOR_VolSurface& vs)
{
   return out << "center: " << vs.center << " radius " << vs.radius << " type: " << vs.volType;
}

std::istream& operator>>(std::istream& is, P1906MOL_MOTOR_VolSurface& vs)
{
  int vt;
  
  is >> vs.center >> vs.radius >> vt;
  vs.volType = static_cast<P1906MOL_MOTOR_VolSurface::typeOfVolume>(vt);
  
  return is;
}

//! print information about the volume surface
void P1906MOL_MOTOR_VolSurface::displayVolSurface()
{
  NS_LOG_INFO ("radius: " << radius << " center point: " << center << " type: " << volType);
}

//! define the volume location and size, in other words, the sphere center and radius
void P1906MOL_MOTOR_VolSurface::setVolume(P1906MOL_MOTOR_Pos v_center, double v_radius)
{
  double x, y, z;
  
  v_center.getPos (&x, &y, &z);
  center.setPos (x, y, z);
  radius = v_radius;
  NS_LOG_INFO (this);
}

//! set the type, which can be FluxMeter, ReflectiveBarrier, Receiver
void P1906MOL_MOTOR_VolSurface::setType (typeOfVolume st)
{
  volType = st;
  NS_LOG_INFO (this);
}

//! get the type, which can be FluxMeter, ReflectiveBarrier, Receiver
P1906MOL_MOTOR_VolSurface::typeOfVolume P1906MOL_MOTOR_VolSurface::getType ()
{
  NS_LOG_INFO (volType);
  return volType;
}

//! reflect a particle from the surface given the last and current positions
//! if outside the volume, adjust the current position given a reflection back into the volume
/* <pre>
    Motor M is reflected off the surface
                 _,,----..
            _,'    ^      `.
           /       |     M  `.  motion from M to M''
          /        |      \  +  but M'' outside volume so reflect to M'
         |         C-------->R  
         +'                / |
          ,              M'  / M'' <-(reflect M'' back into the volume)
           \                /
            ;:           _,'    Angle between MR (motor to intersection) and CR (radius) is known
              `-..____,,'       Angle M'R and CR should be equal to MR and CR
             Volume Surface     Angle M'R, CR is -2 * MR, CR
      
* </pre>
*/
void P1906MOL_MOTOR_VolSurface::reflect(P1906MOL_MOTOR_Pos last_pos, P1906MOL_MOTOR_Pos & current_pos)
{
  vector<P1906MOL_MOTOR_Pos> intersection;
  gsl_vector * trajectory = gsl_vector_alloc (6);
  gsl_vector * ext_traj = gsl_vector_alloc (6);
  gsl_vector * int_traj = gsl_vector_alloc (6);
  gsl_vector * vol_radius = gsl_vector_alloc (6);
  gsl_vector * rad_vec = gsl_vector_alloc (3);
  gsl_vector * lp = gsl_vector_alloc (3);
  gsl_vector * ip = gsl_vector_alloc (3);
  gsl_matrix * vectors = gsl_matrix_alloc (2, 6);

  NS_LOG_DEBUG ("last_pos: " << last_pos << " current_pos: " << current_pos);
  
  //! x_1' - x_0 = v - 2 (v . n) n
  //! This works, but it reflects off the radius instead of the sphere.
  //! Need to reflect off line tangent to the sphere at the intersection point.
  //! So just reverse the vector direction, multiply by -1
  //! x_1' = - (v - 2 (v . n) n) + x_0
  
  //LOG_DEBUG ("current_pos before reflection " << current_location);

  //! x_1' - x_0 = v  - 2 (v . n)    n where n is the unit vector for the radius
  //!              ^vec      ^scalar ^vec
  //! x_1 is M and last_pos, x_1' is M', n is rad_vec, x_0 is the intersection point
    
  //! check if the particle passed through the surface  
  line (trajectory, last_pos, current_pos);

  //! did the particle's trajectory pass through the surface?
  sphereIntersections(trajectory, intersection);
  //LOG_DEBUG ("intersection size: " << intersection.size());
  if (intersection.size() == 0)
  {
	  NS_LOG_DEBUG ("motor did not pass through surface");
  }
  
  //! segment CR is the radius of the volume
  getRadiusLine(vol_radius, intersection.front());
  //! convert radius segment into vector n
  gsl_vector_set (rad_vec, 0, gsl_vector_get (vol_radius, 3) - gsl_vector_get (vol_radius, 0));
  gsl_vector_set (rad_vec, 1, gsl_vector_get (vol_radius, 4) - gsl_vector_get (vol_radius, 1));
  gsl_vector_set (rad_vec, 2, gsl_vector_get (vol_radius, 5) - gsl_vector_get (vol_radius, 2));
  
  //! n is rad_vec, convert to a unit vector
  double s_norm = gsl_blas_dnrm2 (rad_vec);
  gsl_vector_scale (rad_vec, 1.0 / s_norm);
  
  //! segment RM'' is the trajectory outside the volume
  line (ext_traj, intersection.front(), current_pos);
  //! segment MR is the trajectory inside the volume
  line (int_traj, last_pos, intersection.front());

  //! save for display and debugging
  insertVector(vectors, 0, ext_traj);
  insertVector(vectors, 1, vol_radius);
  
  last_pos.getPos (lp);
  intersection.front().getPos (ip);
  //! lp is now (x_1 - x_0) == v
  gsl_vector_sub (lp, ip);
  NS_LOG_DEBUG ("last position (lp): " << 
    gsl_vector_get (lp, 0) << " " <<
    gsl_vector_get (lp, 1) << " " <<
    gsl_vector_get (lp, 2));

  double dot;
  //! (x_1 - x_0) . n = v . n
  gsl_blas_ddot (lp, rad_vec, &dot);
  NS_LOG_DEBUG ("dot product of last position with radius vector (dot): " << dot);
  //! 2 n [(x_1 - x_0) . n]
  gsl_vector_scale (rad_vec, 2.0 * dot);
  NS_LOG_DEBUG ("(reflect) rad_vec: " <<
    gsl_vector_get (rad_vec, 0) << " " <<
    gsl_vector_get (rad_vec, 1) << " " <<
    gsl_vector_get (rad_vec, 2)); 
    
  //! v - 2 (v . n) n
  gsl_vector_sub (lp, rad_vec);
  
  //! reverse the direction of the vector
  gsl_vector_scale (lp, -1.0);
  
  //! x_1' = v - 2 (v . n) n + x_0
  gsl_vector_add (lp, ip);
  
  //! update the current position with the reflected position
  current_pos.setPos (lp);
  
  m_reflections++;
  NS_LOG_DEBUG ("reflected current_pos: " << current_pos);
}

//! return radius line segment from center to a point on the surface
//! v_radius must be allocated for a line segment
void P1906MOL_MOTOR_VolSurface::getRadiusLine(gsl_vector * v_radius, P1906MOL_MOTOR_Pos pt)
{  
  NS_LOG_FUNCTION (this);
  line (v_radius, center, pt);
}

//! return the angle between two vectors in degrees
double P1906MOL_MOTOR_VolSurface::vectorAngle(gsl_vector * seg1, gsl_vector * seg2)
{
  gsl_vector * v1 = gsl_vector_alloc (3);
  gsl_vector * v2 = gsl_vector_alloc (3);
  
  NS_LOG_FUNCTION(this);
  //! take dot product, find magnitudes, and solve for cosθ = a.b / |a||b|
  //! see http://www.wikihow.com/Find-the-Angle-Between-Two-Vectors
  gsl_vector_set (v1, 0, gsl_vector_get (seg1, 3) - gsl_vector_get (seg1, 0));
  gsl_vector_set (v1, 1, gsl_vector_get (seg1, 4) - gsl_vector_get (seg1, 1));
  gsl_vector_set (v1, 2, gsl_vector_get (seg1, 5) - gsl_vector_get (seg1, 2));

  gsl_vector_set (v2, 0, gsl_vector_get (seg2, 3) - gsl_vector_get (seg2, 0));
  gsl_vector_set (v2, 1, gsl_vector_get (seg2, 4) - gsl_vector_get (seg2, 1));
  gsl_vector_set (v2, 2, gsl_vector_get (seg2, 5) - gsl_vector_get (seg2, 2));
  
  NS_LOG_DEBUG ("v1: " <<
    gsl_vector_get (v1, 0) << " " <<
	gsl_vector_get (v1, 2) << " " <<
	gsl_vector_get (v1, 3) << " " <<
	gsl_vector_get (v1, 4) << " " <<
	gsl_vector_get (v1, 5) << " " <<
	gsl_vector_get (v1, 6)
  );
  
  NS_LOG_DEBUG ("v2: " <<
    gsl_vector_get (v2, 0) << " " <<
	gsl_vector_get (v2, 2) << " " <<
	gsl_vector_get (v2, 3) << " " <<
	gsl_vector_get (v2, 4) << " " <<
	gsl_vector_get (v2, 5) << " " <<
	gsl_vector_get (v2, 6)
  );
  
  double dot;
  gsl_blas_ddot (v1, v2, &dot);
  NS_LOG_DEBUG ("dot v1 and v2: " << dot);
  
  //! get the magnitudes
  double m1 = gsl_blas_dnrm2 (v1);
  double m2 = gsl_blas_dnrm2 (v2);
  NS_LOG_DEBUG ("magnitudes of v1 and v2, m1: " << m1 << " m2: " << m2);
  
  //! solve for the angle
  double arc = dot / (m1 * m2);
  NS_LOG_DEBUG ("angle between v1 and v2, arc: " << arc << " radians");
  double angle = acos (arc) * 180.0 / M_PI; //! convert to degrees
  
  NS_LOG_DEBUG ("angle: " << angle << " degrees");
  return angle;
}

//! compute the flux through the sphere to implement the active network programmability metric: \f$A = \int_S \int_t \Delta f(t) dt dS\f$
double P1906MOL_MOTOR_VolSurface::fluxMeter(gsl_matrix * tubeMatrix)
{
  //! the total flux
  double flux = 0;
  //! all intersection points
  vector<P1906MOL_MOTOR_Pos> ipts;
  //! the current segment
  gsl_vector * segment = gsl_vector_alloc (6);
  
  NS_LOG_FUNCTION(this);
  for (size_t s = 0; s < tubeMatrix->size1; s++)
  {
    //! get the segment
    line (segment, tubeMatrix, s);
	//! find the intersecting points with surface volume
    sphereIntersections(segment, ipts);
	//! would like to know whether segment is pointing in or out of the tube
	NS_LOG_DEBUG (ipts.size() << " points of intersection");
  }
    
  //! use the total number of intersecting tubes for now
  //! \todo incorporate direction and motor rate along tube
  flux += ipts.size();
  
  m_flux = flux;
  NS_LOG_INFO ("flux: " << flux);
  return flux;
}

//! return true if the point is inside the volume surface
bool P1906MOL_MOTOR_VolSurface::isInsideVolSurf(P1906MOL_MOTOR_Pos pt)
{
  bool isInside = false;
  double norm = 0;
  gsl_vector * C = gsl_vector_alloc (3);
  gsl_vector * P = gsl_vector_alloc (3);
  
  //! simply check if distance from center is less than radius
  center.getPos (C);
  pt.getPos (P);
  //! vector from point to center
  gsl_vector_sub (C, P);
  //! magnitude of the vector from point to center
  norm = gsl_blas_dnrm2 (C);
  if (norm < radius)
    isInside = true;
  m_messages_received++; // this should be moved to motion where it is called and message payload counted
  NS_LOG_INFO ("motor inside volume surface?" << isInside);
  return isInside;
}

//! find the intersecting point(s) ipt that intersect the volume surface:
//! (1) if no intersection, then ipt has zero values
//! (2) if one intersection, then ipt has one intersection point
//! (3) if two intersections, then ipt has two intersection points
void P1906MOL_MOTOR_VolSurface::sphereIntersections(gsl_vector * segment, vector<P1906MOL_MOTOR_Pos> & ipt)
{
  P1906MOL_MOTOR_Pos o, l, c;
  double d, r;
  gsl_vector * lv = gsl_vector_alloc (3);
  
  //! find all the tube segments intersecting with the surface: equation for surface as function of x, y, z == equation for segment
  //! sphere = x^2 + y^2 + z^2 = r^2
  //! see int P1906MOL_MOTOR_Field::getOverlap3D(gsl_vector * segment, gsl_matrix * tubeMatrix, gsl_matrix * pts, gsl_vector * tubeSegments)
  //! find the angle of each with respect to the surface: difference segment angle differs from pointing to the center of sphere
  //! 
  //! find the direction of flow of each tube: tubes always flow from lower index to higher index in tubeMatrix
  //! flux
  
  NS_LOG_DEBUG ("Begin");
	
  //! sphere equation: |x - c|^2 = r^2 where c (3D) is the center, r (scalar) is the radius, 
  //! and x (3D) is points on the sphere  
  c = center;
  r = radius;

  //! line equation: x = o + d l where o (3D) is the starting point, d (scalar) is distance, 
  //! l (3D) is direction (unit vector), and x (3D) is points on the line
  o.setPos (
	gsl_vector_get (segment, 0),
	gsl_vector_get (segment, 1),
	gsl_vector_get (segment, 2)
  );
  
  //NS_LOG_DEBUG ("segment start: " << o.displayPos ());
	
  //! convert segment into a vector
  l.setPos (
	gsl_vector_get (segment, 3) - gsl_vector_get (segment, 0),
	gsl_vector_get (segment, 4) - gsl_vector_get (segment, 1),
	gsl_vector_get (segment, 5) - gsl_vector_get (segment, 2)
  );
  NS_LOG_DEBUG ("segment vector: " << l);
  
  //! convert the segment vector into a unit vector; divide by length
  l.getPos (lv);
  double segMag = gsl_blas_dnrm2 (lv);
  gsl_vector_scale (lv, 1.0 / segMag);
  l.setPos (lv);
  NS_LOG_DEBUG ("segment unit vector: " << l);
  
  //! d = -l * (o - c) +/- sqrt((l * (o - c))^2 - |o - c|^2 + r^2)
  //! B = l * (o - c), 4AC = |o - c|^2 + r^2, -B +/- sqrt(B^2 - 4AC)/2A
  double B;
  double AC;
  gsl_vector * O = gsl_vector_alloc (3);
  gsl_vector * C = gsl_vector_alloc (3);
  
  o.getPos (O);
  center.getPos (C);
  NS_LOG_DEBUG ("C and O: " << center << " " << o);
  
  //! B = l . (o - c)
  gsl_vector_sub (O, C);
  NS_LOG_DEBUG ("0 - C: " << gsl_vector_get (O, 0) << " " << gsl_vector_get (O, 1) << " " << gsl_vector_get (O, 2));
  gsl_blas_ddot (lv, O, &B);
  NS_LOG_DEBUG ("lv: " << gsl_vector_get (lv, 0) << " " << gsl_vector_get (lv, 1) << " " << gsl_vector_get (lv, 2));
  NS_LOG_DEBUG ("B = l . (o - c): " << B);
  
  //! AC = |o - c|^2 + r^2
  double norm = gsl_blas_dnrm2 (O);
  NS_LOG_DEBUG ("|o - c|^2: " << norm);
  AC = norm + pow(r, 2);
  NS_LOG_DEBUG ("AC = |o - c|^2 + r^2: " << AC);
  
  //! it looks like a pow(B, 2) term should be added to AC
  
  //! if B^2 - AC < 0, then no intersection
  if (AC < 0)
  {
    ipt.clear();
  }
	
  //! if B^2 - 4AC == 0, then one intersection
  if (AC == 0)
  {
    //! only do this if d is <= length of tube
	d = -B;
	if (abs(d) <= segMag)
	{
		NS_LOG_DEBUG ("AC == 0 d: " << d << " segMag: " << segMag);
		NS_LOG_DEBUG ("l: " << l);
		NS_LOG_DEBUG ("o: " << o);
		//! x = o + d l
		o.shiftPos (l, d);
		NS_LOG_DEBUG ("o shifted: " << o);
		ipt.insert(ipt.end(), o);
    }
  }
  
  //! if B^2 - 4AC > 0, then two intersections
  if (AC > 0)
  {
    P1906MOL_MOTOR_Pos tmp;
	gsl_vector * v_tmp = gsl_vector_alloc (3);
	
	NS_LOG_DEBUG ("AC: " << AC);
	
	//! set the tmp position with the same value as o
	o.getPos (v_tmp);
	tmp.setPos (v_tmp);
	
	//! only do this if d is <= length of tube
	d = -B + sqrt(AC);
    if (abs(d) <= segMag)
	{	
		NS_LOG_DEBUG ("AC > 0 and d = -B + sqrt(AC) d: " << d << " segMag: " << segMag);
		NS_LOG_DEBUG ("l: " << l);
		NS_LOG_DEBUG ("o: " << o);
		//! x = o + d l
		o.shiftPos (l, d);
		NS_LOG_DEBUG (" o shifted: " << o);
		ipt.insert(ipt.end(), o);
    }	
	//! only do this if d is <= length of tube
	d = -B - sqrt(AC);
	if (abs(d) <= segMag)
	{
		NS_LOG_DEBUG ("AC > 0 and d = -B - sqrt(AC) d: " << d << " segMag: " << segMag);
		NS_LOG_DEBUG ("l: " << l);
		NS_LOG_DEBUG ("tmp: " << tmp);
		//! x = o + d l
		tmp.shiftPos (l, d);
		NS_LOG_DEBUG ("tmp shifted: " << tmp);
		ipt.insert(ipt.end(), tmp);
	}
  }
  NS_LOG_INFO ("number of intersection points: " << ipt.size());
}

P1906MOL_MOTOR_VolSurface::~P1906MOL_MOTOR_VolSurface ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
