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

TypeId P1906MOL_MOTOR_VolSurface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_VolSurface")
    .SetParent<P1906MOL_MOTOR_Field> ();
  return tid;
}

P1906MOL_MOTOR_VolSurface::P1906MOL_MOTOR_VolSurface ()
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
  
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);  
  gsl_rng_env_setup();  
}

//! print information about the volume surface
void P1906MOL_MOTOR_VolSurface::displayVolSurface()
{
  printf ("(displayVolSurface) radius: %lf type: %d\n", radius, volType);
  printf ("(displayVolSurface) center point:\n");
  center.displayPos();
}

//! define the volume location and size, in other words, the sphere center and radius
void P1906MOL_MOTOR_VolSurface::setVolume(P1906MOL_MOTOR_Pos v_center, double v_radius)
{
  double x, y, z;
  
  v_center.getPos (&x, &y, &z);
  center.setPos (x, y, z);
  radius = v_radius;
}

//! set the type, which can be FluxMeter, ReflectiveBarrier, Receiver
void P1906MOL_MOTOR_VolSurface::setType (typeOfVolume st)
{
  volType = st;
}

//! get the type, which can be FluxMeter, ReflectiveBarrier, Receiver
P1906MOL_MOTOR_VolSurface::typeOfVolume P1906MOL_MOTOR_VolSurface::getType ()
{
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

  //printf ("(reflect) Begin\n");
  
  //! x_1' - x_0 = v - 2 (v . n) n
  //! This works, but it reflects off the radius instead of the sphere.
  //! Need to reflect off line tangent to the sphere at the intersection point.
  //! So just reverse the vector direction, multiply by -1
  //! x_1' = - (v - 2 (v . n) n) + x_0
  
  //printf ("(reflect) current_pos before reflection\n");
  //current_pos.displayPos();

  //! x_1' - x_0 = v  - 2 (v . n)    n where n is the unit vector for the radius
  //!              ^vec      ^scalar ^vec
  //! x_1 is M and last_pos, x_1' is M', n is rad_vec, x_0 is the intersection point
    
  //! check if the particle passed through the surface  
  line (trajectory, last_pos, current_pos);

  //! did the particle's trajectory pass through the surface?
  sphereIntersections(trajectory, intersection);
  //printf ("(reflect) intersection size: %ld\n", intersection.size());
  if (intersection.size() == 0)
  {
      printf ("(reflect) motor did not pass through surface\n");
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
  //printf ("(reflect) lp: %lf %lf %lf\n",
  //  gsl_vector_get (lp, 0),
  //  gsl_vector_get (lp, 1),
  //  gsl_vector_get (lp, 2)
  //);
  double dot;
  //! (x_1 - x_0) . n = v . n
  gsl_blas_ddot (lp, rad_vec, &dot);
  //printf ("(reflect) dot: %lf\n", dot);
  //! 2 n [(x_1 - x_0) . n]
  gsl_vector_scale (rad_vec, 2.0 * dot);
  //printf ("(reflect) rad_vec: %lf %lf %lf\n",
  //  gsl_vector_get (rad_vec, 0),
  //  gsl_vector_get (rad_vec, 1),
  //  gsl_vector_get (rad_vec, 2)
  //); 
    
  //! v - 2 (v . n) n
  gsl_vector_sub (lp, rad_vec);
  
  //! reverse the direction of the vector
  gsl_vector_scale (lp, -1.0);
  
  //! x_1' = v - 2 (v . n) n + x_0
  gsl_vector_add (lp, ip);
  
  //! update the current position with the reflected position
  current_pos.setPos (lp);
  //printf ("(reflect) current_pos after reflection\n");
  //current_pos.displayPos();
  
  //printf ("(reflect) End\n");
}

//! return radius line segment from center to a point on the surface
//! v_radius must be allocated for a line segment
void P1906MOL_MOTOR_VolSurface::getRadiusLine(gsl_vector * v_radius, P1906MOL_MOTOR_Pos pt)
{  
  line (v_radius, center, pt);
}

//! return the angle between two vectors
double P1906MOL_MOTOR_VolSurface::vectorAngle(gsl_vector * seg1, gsl_vector * seg2)
{
  gsl_vector * v1 = gsl_vector_alloc (3);
  gsl_vector * v2 = gsl_vector_alloc (3);
  
  //! take dot product, find magnitudes, and solve for cosθ = a.b / |a||b|
  //! see http://www.wikihow.com/Find-the-Angle-Between-Two-Vectors
  gsl_vector_set (v1, 0, gsl_vector_get (seg1, 3) - gsl_vector_get (seg1, 0));
  gsl_vector_set (v1, 1, gsl_vector_get (seg1, 4) - gsl_vector_get (seg1, 1));
  gsl_vector_set (v1, 2, gsl_vector_get (seg1, 5) - gsl_vector_get (seg1, 2));

  gsl_vector_set (v2, 0, gsl_vector_get (seg2, 3) - gsl_vector_get (seg2, 0));
  gsl_vector_set (v2, 1, gsl_vector_get (seg2, 4) - gsl_vector_get (seg2, 1));
  gsl_vector_set (v2, 2, gsl_vector_get (seg2, 5) - gsl_vector_get (seg2, 2));
  
  double dot;
  gsl_blas_ddot (v1, v2, &dot);
  //printf ("(vectorAngle) dot: %lf \n", dot);
	
  //! get the magnitudes
  double m1 = gsl_blas_dnrm2 (v1);
  double m2 = gsl_blas_dnrm2 (v2);
  //printf ("(vectorAngle) m1: %lf m2 %lf\n", m1, m2);
  
  //! solve for the angle
  double arc = dot / (m1 * m2);
  //printf ("(vectorAngle) arc: %lf\n", arc);
  double angle = acos (arc) * 180.0 / M_PI;
  
  //printf ("(vectorAngle) angle: %lf\n", angle);
  
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
  
  for (size_t s = 0; s < tubeMatrix->size1; s++)
  {
    //! get the segment
    line (segment, tubeMatrix, s);
	//! find the intersecting points with surface volume
    sphereIntersections(segment, ipts);
	//! would like to know whether segment is pointing in or out of the tube
	//printf ("(fluxMeter) %ld points\n", ipts.size());
  }
    
  //! use the total number of intersecting tubes for now
  //! \todo incorporate direction and motor rate along tube
  flux += ipts.size();
  
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
  
  return isInside;
}

//! find the intersecting point(s) ipt that intersect the volume surface:
//! (1) if no intersection, then ipt has zero values
//! (2) if one intersection, then ipt has one intersection point
//! (3)if two intersections, then ipt has two intersection points
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
  
  //printf ("(sphereIntersections) Begin\n");
	
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
  
  //printf ("(sphereIntersections) segment start:\n");
  //o.displayPos ();
	
  //! convert segment into a vector
  l.setPos (
	gsl_vector_get (segment, 3) - gsl_vector_get (segment, 0),
	gsl_vector_get (segment, 4) - gsl_vector_get (segment, 1),
	gsl_vector_get (segment, 5) - gsl_vector_get (segment, 2)
  );
  //printf ("(sphereIntersections) segment vector:\n");
  //l.displayPos ();
  
  //! convert the segment vector into a unit vector; divide by length
  l.getPos (lv);
  double segMag = gsl_blas_dnrm2 (lv);
  gsl_vector_scale (lv, 1.0 / segMag);
  l.setPos (lv);
  //printf ("(sphereIntersections) segment unit vector:\n");
  //l.displayPos ();
  
  //! d = -l * (o - c) +/- sqrt((l * (o - c))^2 - |o - c|^2 + r^2)
  //! B = l * (o - c), 4AC = |o - c|^2 + r^2, -B +/- sqrt(B^2 - 4AC)/2A
  double B;
  double AC;
  gsl_vector * O = gsl_vector_alloc (3);
  gsl_vector * C = gsl_vector_alloc (3);
  
  o.getPos (O);
  center.getPos (C);
  //printf ("(sphereIntersections) C and O:\n");
  //center.displayPos ();
  //o.displayPos ();
  
  //! B = l . (o - c)
  gsl_vector_sub (O, C);
  //printf ("(sphereIntersections) 0 - C: %lf %lf %lf\n", gsl_vector_get (O, 0), gsl_vector_get (O, 1), gsl_vector_get (O, 2));
  gsl_blas_ddot (lv, O, &B);
  //printf ("(sphereIntersections) lv: %lf %lf %lf\n", gsl_vector_get (lv, 0), gsl_vector_get (lv, 1), gsl_vector_get (lv, 2));
  //printf ("(sphereIntersections) B = l . (o - c): %lf\n", B);
  
  //! AC = |o - c|^2 + r^2
  double norm = gsl_blas_dnrm2 (O);
  //printf ("(sphereIntersections) |o - c|^2: %lf\n", norm);
  AC = norm + pow(r, 2);
  //printf ("(sphereIntersections) AC = |o - c|^2 + r^2: %lf\n", AC);
  
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
		//printf ("(sphereIntersections) AC == 0 d: %lf segMag: %lf\n", d, segMag);
		//printf ("(sphereIntersections) l:\n");
		//l.displayPos();
		//printf ("(sphereIntersections) o:\n");
		//o.displayPos ();
		//! x = o + d l
		o.shiftPos (l, d);
		//printf ("(sphereIntersections) o shifted:\n");
		//o.displayPos ();
		ipt.insert(ipt.end(), o);
    }
  }
  
  //! if B^2 - 4AC > 0, then two intersections
  if (AC > 0)
  {
    P1906MOL_MOTOR_Pos tmp;
	gsl_vector * v_tmp = gsl_vector_alloc (3);
	
	//printf ("(sphereIntersections) AC: %lf\n", AC);
	
	//! set the tmp position with the same value as o
	o.getPos (v_tmp);
	tmp.setPos (v_tmp);
	
	//! only do this if d is <= length of tube
	d = -B + sqrt(AC);
    if (abs(d) <= segMag)
	{	
		//printf ("(sphereIntersections) AC > 0 and d = -B + sqrt(AC) d: %lf segMag: %lf\n", d, segMag);
		//printf ("(sphereIntersections) l:\n");
		//l.displayPos();
		//printf ("(sphereIntersections) o:\n");
		//o.displayPos ();
		//! x = o + d l
		o.shiftPos (l, d);
		//printf ("(sphereIntersections) o shifted:\n");
		//o.displayPos ();
		ipt.insert(ipt.end(), o);
    }	
	//! only do this if d is <= length of tube
	d = -B - sqrt(AC);
	if (abs(d) <= segMag)
	{
		//printf ("(sphereIntersections) AC > 0 and d = -B - sqrt(AC) d: %lf segMag: %lf\n", d, segMag);
		//printf ("(sphereIntersections) l:\n");
		//l.displayPos();
		//printf ("(sphereIntersections) tmp:\n");
		//tmp.displayPos ();
		//! x = o + d l
		tmp.shiftPos (l, d);
		//printf ("(sphereIntersections) tmp shifted:\n");
		//tmp.displayPos ();
		ipt.insert(ipt.end(), tmp);
	}
  }
  	
  //printf ("(sphereIntersections) End\n");
}

P1906MOL_MOTOR_VolSurface::~P1906MOL_MOTOR_VolSurface ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
