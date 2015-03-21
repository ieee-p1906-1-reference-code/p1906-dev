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
 
 /* \details This class implements creation of a single tube
  *
  * <pre>
  * RADIUS      SEGMENT                                                     
  *      +    <---------->                               
  *      |                                               
  *      |    +----------+----------+----------XXXXX     
  *      +--> |    S1    |   S2     |   S3     |   XXX   
  *           +----------+----------+----------XX    XX  
  *                                          ^  XXX S4 XX+
  *                                          |    XXX    |
  *                                   ANGLE  +--->  XXXXX+
  *                                                      
  *                                                      
  *            <-------------------------------->        
  *              MICROTUBULE OR CARBON NANOTUBE
  * </pre>
  */

#include "ns3/log.h"

#include "ns3/p1906-mol-motor-microtubule.h"
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-tube.h"

#include "ns3/p1906-mol-motor-tube-characteristics.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_Tube");

TypeId P1906MOL_MOTOR_Tube::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_Tube")
    .SetParent<P1906MOL_MOTOR_Field> ();
  return tid;
}

//! the constructor creates the tube
P1906MOL_MOTOR_Tube::P1906MOL_MOTOR_Tube (struct tubeCharacteristcs_t * ts, gsl_vector * startPt)
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
  gsl_rng_set (r, rand());
  
  //! hold the values for a tube comprised of many segments: x_start y_start x_start x_end y_end z_end
  segMatrix = gsl_matrix_alloc (ts->segPerTube, 6);
  
  genTube(ts, r, segMatrix, startPt);
}

//! return the segMatrix, which is the tube segment end points
void P1906MOL_MOTOR_Tube::getSegmatrix(gsl_matrix * sm)
{
  gsl_matrix_memcpy (sm, segMatrix);
}
    
//! create and return a microtubule of given persistence length in segMatrix and its structural entropy in se
//! \todo randomize initial tube orientations
int P1906MOL_MOTOR_Tube::genTube(struct tubeCharacteristcs_t * ts, gsl_rng *r, gsl_matrix *segMatrix, gsl_vector *startPt)
{
  /**
    for 3D there are two angles per tube: \theta and \psi in spherical coordinates, length is already specified 
	x = r sin \theta cos \psi
	y = r sin \theta sin \psi
	z = r cos \theta
	
  */
   
  gsl_matrix * segAngleTheta = gsl_matrix_alloc (ts->numSegments, 1);
  gsl_matrix * segAnglePsi = gsl_matrix_alloc (ts->numSegments, 1);

  //printf ("(genTube) startX = %g startY = %g startZ = %g numSegments = %ld segLength = %g persistenceLength = %g\n", 
  //  gsl_vector_get (startPt, 0), 
  //  gsl_vector_get (startPt, 1),  
  //  gsl_vector_get (startPt, 2), 
  //  ts->numSegments, 
  //  ts->segLength, 
  //  ts->persistenceLength);
  
  genPersistenceLength(r, segAngleTheta, ts->segLength, ts->persistenceLength);
  genPersistenceLength(r, segAnglePsi, ts->segLength, ts->persistenceLength);
  
  ts->se = sEntropy (segAngleTheta) + sEntropy (segAnglePsi);
  
  //for (size_t i = 0; i < ts->numSegments; i++)
  //  for (size_t j = 0; j < 1; j++)
  //  {
  //    printf ("segAngleTheta(%ld,%ld) = %g\n", i, j, gsl_matrix_get (segAngleTheta, i, j));
  //    printf ("segAnglePsi(%ld,%ld) = %g\n", i, j, gsl_matrix_get (segAnglePsi, i, j));
  //  }
	
  //printf ("(genTube) segments allocated %ld %ld numSegments %ld\n", segMatrix->size1, segMatrix->size2, ts->numSegments);
  
  for (size_t i = 0; i < ts->segPerTube; i++)
  {
    //! set the start points of the segment
    if(i == 0)
    { //! use starting coordinates
      gsl_matrix_set(segMatrix, 0, 0, gsl_vector_get (startPt, 0));
      gsl_matrix_set(segMatrix, 0, 1, gsl_vector_get (startPt, 1));
	  gsl_matrix_set(segMatrix, 0, 2, gsl_vector_get (startPt, 2));
    }
    else
    { //! use end of last segment
      gsl_matrix_set(segMatrix, i, 0, gsl_matrix_get(segMatrix, i - 1, 3));
      gsl_matrix_set(segMatrix, i, 1, gsl_matrix_get(segMatrix, i - 1, 4));
	  gsl_matrix_set(segMatrix, i, 2, gsl_matrix_get(segMatrix, i - 1, 5));
    }
	//! set the end points of the segment
	double x = ts->segLength * sin(gsl_matrix_get(segAngleTheta, i, 0)) * cos(gsl_matrix_get(segAnglePsi, i, 0));
	double y = ts->segLength * sin(gsl_matrix_get(segAngleTheta, i, 0)) * sin(gsl_matrix_get(segAnglePsi, i, 0));
	double z = ts->segLength * cos(gsl_matrix_get(segAngleTheta, i, 0));
	gsl_matrix_set(segMatrix, i, 3, x + gsl_matrix_get(segMatrix, i, 0));
    gsl_matrix_set(segMatrix, i, 4, y + gsl_matrix_get(segMatrix, i, 1));
	gsl_matrix_set(segMatrix, i, 5, z + gsl_matrix_get(segMatrix, i, 2));
  }
  
  return 0;
}

//! generate angles for a structure with the given persistence length and segment length and return the angles in setAngle
double P1906MOL_MOTOR_Tube::genPersistenceLength(gsl_rng * r, gsl_matrix * segAngle, double segLength, double persistenceLength)
{
  /** 
    the angle distribution is Gaussian with zero mean and variance \f$\sigma^2 = \sqrt(2 \Delta s / l_p)\f$,
    where \f$l_p\f$ is persistence length and \f$\Delta\f$ is the segment length; angles are in radians
	
	See http://www.uic.edu/classes/phys/phys450/MARKO/N015.html.
	
	Fortunately there is a relation that we can take advantage of to figure out the stiffness of the torsion spring 
	between each segment. This relation is \f$K = L_p k_BT\f$. From this we can see that the persistence length is a characteristic 
	length scale that relates the bending rigidity of the DNA chain to the thermal energy. Larger persistence lengths indicate 
	a stiffer chain that is less susceptible to thermal fluctuations.

    What we're interested in, of course, is the energy cost for bending the DNA into the nice round coil it must be in to 
	fit in the capsid. Like a linear spring where the energy is of the form \f$E = 1/2 k x^2\f$, the energy to bend the DNA chain 
	is of the form \f$E = 1/2 K L (dt/ds)^2\f$, where \f$dt/ds\f$ is the derivative of the tangent vector to each Kuhn segment of the 
	chain. Since the chain is being bent into a circle of radius \f$R\f$, \f$dt/ds = 1/R\f$, and thus the bending energy is 
	\f$E = 1/2 K L/R^2 = 1/2 L_p L/R^2 k_BT\f$. If we assume that all of the DNA is coiled around the outer radius of the 
	capsid (R = 42 nm), then the energy cost due to bending is \f$E_{bend} = 1/2 * (50 nm)*(6800 nm)/(42 nm)2 k_BT ~ 10^2 k_BT\f$. 
	Note that this is approximately the same energy cost as we found for the loss of entropy. From the model shown on 
	the first page of the website, you may have noticed that some of the DNA is in fact coiled at smaller radii than the 
	outermost "shell". The energy cost for bending that part of the DNA will be much higher since E is proportional to \f$1/R^2\f$, 
	but even so, it is unlikely that this energy cost greatly exceeds \f$~10^3 k_BT\f$.
	
	Ref: Multiscale Modeling in Biomechanics and Mechanobiology edited by Suvranu De, Wonmuk Hwang, Ellen Kuh pp. 68-69
  */
  double angle;
  double sigma;

  if (persistenceLength > 0)
    sigma  = sqrt(2.0 * segLength / persistenceLength);
  else
	sigma = DBL_MAX; //! maximum possible variance

  //printf ("(genPersistenceLength) sigma = %g\n", sigma);
	
  for(size_t i = 0; i < segAngle->size1; i++)
  {
    //! return a valid radian [0..2\pi]
    angle = fmod(gsl_ran_gaussian (r, sigma), (2 * M_PI));
	if (!gsl_finite (angle))
	  angle = gsl_ran_ugaussian (r) * (2 * M_PI); //! just pick a uniform random angle
    //printf ("(genPersistenceLength) radian(%ld) = %g\n", i, angle);
	//! a radian is 180/Pi degrees
	//printf ("(genPersistenceLength) angle(%ld) = %g\n", i, angle * 180.00 / M_PI);
    gsl_matrix_set(segAngle, i, 0, angle);
  }
    
  sEntropy(segAngle);
  
  return 0;
}

//! compute the persistence length of a set of segments
//! \todo never implemented; may not be needed
double P1906MOL_MOTOR_Tube::getPersistenceLength()
{
  //! segMatrix rows are x_start x_end y_start y_end (may not need numSegments)
    
  return 0;
}

//! print the tube segments in segMatrix
void P1906MOL_MOTOR_Tube::displayTube()
{
  for (size_t i = 0; i < segMatrix->size1; i++)
  {
    for (size_t j = 0; j < segMatrix->size2; j++)
	{
	  printf ("segMatrix(%ld,%ld) = %g\t", i, j, gsl_matrix_get (segMatrix, i, j));
    }
	printf ("\n");
  }
}

P1906MOL_MOTOR_Tube::~P1906MOL_MOTOR_Tube ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
