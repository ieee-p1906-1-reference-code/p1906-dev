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

/* \details This class implements diffusion-related differential equations
 *
 */
 
#include "ns3/log.h"

#include "gsl/gsl_sf_exp.h"
#include "ns3/p1906-mol-diffusion-wave.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_ExtendedDiffusionWave");

TypeId P1906MOL_ExtendedDiffusionWave::GetTypeId (void)
{
  static TypeId tid = TypeId ("P1906MOL_ExtendedDiffusionWave")
    .SetParent<Object> ();
  return tid;
}

P1906MOL_ExtendedDiffusionWave::P1906MOL_ExtendedDiffusionWave ()
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
  
  //! time the concentration was released [s]
  transmission_time = GSL_POSINF;
  D = GSL_POSINF;
  //! initial concentration [nmol/nm^3]
  c_0 = GSL_POSINF;
  //! location of the initial release [nm] (x,y,z)
  transmitter.setPos (GSL_POSINF, GSL_POSINF, GSL_POSINF);
  //! mark to remove since concentration is too low
  low_concentration = false;
}

//! configure a transmission
void P1906MOL_ExtendedDiffusionWave::prepare_transmission(double tt, double Cd, double ic, P1906MOL_MOTOR_Pos ip)
{
  gsl_vector * p = gsl_vector_alloc(3);
  
  //! time the concentration was released [s]
  transmission_time = tt;
  D = Cd;
  //! initial concentration [nmol/nm^3]
  c_0 = ic;
  ip.getPos (p);
  //! location of the initial release [nm] (x,y,z)
  transmitter.setPos (p);
}

//! trigger a release using last configures values
void P1906MOL_ExtendedDiffusionWave::transmit(double tt)
{
  //! time the concentration was released [s]
  transmission_time = tt;
}

// The goal is to solve for diffusion concentration in 3D given:
// injection location
// boundaries assuming simple volume (perhaps sphere)
// receiving location(s)
// diffusion coefficient
//
// Approach in:
// Socolofsky, S. A., & Jirka, G. H. (n.d.). CVEN 489-501: 
// Special Topics in Mixing and Transport Processes in the Environment (Vol. 1, pp. 1–22). 
// https://ceprofs.civil.tamu.edu/ssocolofsky/cven489/Book/Book.htm
//
// Convection-diffusion:
// https://en.wikipedia.org/wiki/Convection%E2%80%93diffusion_equation
// https://en.wikipedia.org/wiki/Numerical_solution_of_the_convection%E2%80%93diffusion_equation
//
// The solution is very simple: treat 3D diffusion as a Gaussian sphere.
// Multiple releases are simply a super-position of such Gaussian results.
// If in a closed volume, the total concentration will increase and boundary conditions may be required.
//
// Since super-position applies, make the object keep track of its own elapsed time and easy to add 
// with other such objects. Call them "perturbation" objects. If closed volume, then keep track of 
// accumulated concentration. 
//
//*********************************************************************
// sample the concentration at location receiver and time
double P1906MOL_ExtendedDiffusionWave::concentration_wave (P1906MOL_MOTOR_Pos receiver, double time)
{
  //! receiver 3D position [nm] (x,y,z)
  float D = 1.0; //! diffusion coefficient [nm^2/s]
  double r; //! radius from source [nm]
  gsl_vector *xpos = gsl_vector_alloc(3);
  gsl_vector *rpos = gsl_vector_alloc(3);
  double t; //! [s]
  double c; //! [nmol / nm^3]

  transmitter.setPos (0, 0, 0);
  transmitter.getPos (xpos);
  receiver.setPos (10, 0, 0);
  receiver.getPos (rpos);

  gsl_vector_sub (rpos, xpos);
  r = gsl_blas_dnrm2(rpos);
  
  //! if multiple transmissions, then take the super-position of all transmissions
  for (t = 1.0; t < 100.0; t = t + 1.0)
  {
    //! proportion of initial concentration
	c = c_0 * pow((4.0 * M_PI * D * t), -3.0/2.0) * gsl_sf_exp (-pow(r, 2.0)/(4.0 * D * t));
	printf ("(concentration_wave) c(t): %lf(%lf)\n", c, t);
  }
  
  return 0;
}

P1906MOL_ExtendedDiffusionWave::~P1906MOL_ExtendedDiffusionWave ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3