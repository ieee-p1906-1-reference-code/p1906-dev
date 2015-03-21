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
 
#include <algorithm>    // std::remove_if
 
#include "ns3/log.h"

#include "gsl/gsl_sf_exp.h"
#include "ns3/p1906-mol-diffusion.h"
#include "ns3/p1906-mol-motor-pos.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_ExtendedDiffusion");

TypeId P1906MOL_ExtendedDiffusion::GetTypeId (void)
{
  static TypeId tid = TypeId ("P1906MOL_ExtendedDiffusion")
    .SetParent<Object> ();
  return tid;
}

//! \todo transmission belongs in Perturbation
//! \todo diffusion-wave is a Message Carrier
//! \todo receive should be part of Specificity 
P1906MOL_ExtendedDiffusion::P1906MOL_ExtendedDiffusion ()
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
}

//! trigger a release of a vector wave at time tt
//! \todo wv is a growing list of waves, will need to remove waves that have reached equilibrium (or disseminated)
void P1906MOL_ExtendedDiffusion::transmit(double tt, double D, double ic, P1906MOL_MOTOR_Pos ip)
{
  P1906MOL_ExtendedDiffusionWave wave;
  
  //! record the time [s], concentration [nmol/nm^3], and location of transmission
  wave.prepare_transmission(tt, D, ic, ip);
  //! store the wave
  wv.insert (wv.end(), wave);
}

bool IsDissipated(const P1906MOL_ExtendedDiffusionWave & o)
{
    return o.low_concentration;
}

//! check for waves below the minimum and remove (this needs to be a specific time and LOCATION)
void P1906MOL_ExtendedDiffusion::clean_wavevector(double min_concentration, P1906MOL_MOTOR_Pos receiver, double time)
{

  for (size_t i; i < wv.size(); i++)
  {
    if (wv.at(i).concentration_wave (receiver, time) < min_concentration)
	  wv.at(i).low_concentration = true;
  }

  wv.erase( std::remove_if( wv.begin(), wv.end(), IsDissipated ), wv.end() );
}

//! sample the concentration at time rt and location rp
//! \todo wv is a growing list of waves, will need to remove waves that have reached equilibrium (or disseminated)
double P1906MOL_ExtendedDiffusion::receive(double rt, P1906MOL_MOTOR_Pos rp)
{
  //! the accumulated concentration
  double c = 0;
  
  for (size_t i = 0; i < wv.size(); i++)
  {
    c += wv.at(i).concentration_wave (rp, rt);
  }
  
  return c;
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

double P1906MOL_ExtendedDiffusion::unitTest_diffusion ()
{
  P1906MOL_ExtendedDiffusionWave wave;
  P1906MOL_MOTOR_Pos receiver; //! receiver 3D position [nm] (x,y,z)
  P1906MOL_MOTOR_Pos transmitter; //! receiver 3D position [nm] (x,y,z)
  double D = 1.0; //! diffusion coefficient [nm^2/s]
  //double r; //! radius from source [nm]
  gsl_vector *xpos = gsl_vector_alloc(3);
  gsl_vector *rpos = gsl_vector_alloc(3);
  //double t; //! [s]
  //double c; //! [nmol / nm^3]
  //gsl_vector * p = gsl_vector_alloc(3);
  //! time the concentration was released [s]
  //transmission_time = tt;
  //! initial concentration [nmol/nm^3]
  double c_0 = 1.0;
  //ip.getPos (p);
  //! location of the initial release [nm] (x,y,z)
  //transmitter.setPos (p);

  transmitter.setPos (0, 0, 0);
  transmitter.getPos (xpos);
  receiver.setPos (10, 0, 0);
  receiver.getPos (rpos);

  transmit(0, D, c_0, transmitter);
  
  return 0;
}

void P1906MOL_ExtendedDiffusion::displayODE ()
{
  printf ("(displayODE) testing ODE\n");
}

// The following details are taken from the GSL documentation

// 
// The following program solves the second-order nonlinear 
//  Van der Pol oscillator equation (see Landau/Paez 14.12, part 1),
//
//     x"(t) + \mu x'(t) (x(t)^2 - 1) + x(t) = 0
//
// This can be converted into a first order system suitable for 
//  use with the library by introducing a separate variable for 
//  the velocity, v = x'(t).  We assign x --> y[0] and v --> y[1].
//  So the equations are:
// x' = v                  ==>  dy[0]/dt = f[0] = y[1]
// v' = -x + \mu v (1-x^2) ==>  dy[1]/dt = f[1] = -y[0] + mu*y[1]*(1-y[0]*y[0])
//
//*********************************************************************

// function prototypes 
int unitTest_func (double t, const double y[], double f[], void *params_ptr);
int unitTest_jac (double t, const double y[], double *dfdy, double dfdt[], void *params_ptr);

int
unitTest_func (double t, const double y[], double f[],
      void *params)
{
  double mu = *(double *)params;
  f[0] = y[1];
  f[1] = -y[0] - mu*y[1]*(y[0]*y[0] - 1);
  return GSL_SUCCESS;
}

int
unitTest_jac (double t, const double y[], double *dfdy, 
     double dfdt[], void *params)
{
  double mu = *(double *)params;
  gsl_matrix_view dfdy_mat 
    = gsl_matrix_view_array (dfdy, 2, 2);
  gsl_matrix * m = &dfdy_mat.matrix; 
  gsl_matrix_set (m, 0, 0, 0.0);
  gsl_matrix_set (m, 0, 1, 1.0);
  gsl_matrix_set (m, 1, 0, -2.0*mu*y[0]*y[1] - 1.0);
  gsl_matrix_set (m, 1, 1, -mu*(y[0]*y[0] - 1.0));
  dfdt[0] = 0.0;
  dfdt[1] = 0.0;
  return GSL_SUCCESS;
}

double P1906MOL_ExtendedDiffusion::unitTest_ODE ()
{
	const gsl_odeiv_step_type * T
	 = gsl_odeiv_step_rk8pd;

	gsl_odeiv_step * s
	 = gsl_odeiv_step_alloc (T, 2);
	gsl_odeiv_control * c
	 = gsl_odeiv_control_y_new (1e-6, 0.0);
	gsl_odeiv_evolve * e
	 = gsl_odeiv_evolve_alloc (2);

	double mu = 10;
	gsl_odeiv_system sys = {unitTest_func, unitTest_jac, 2, &mu};

	double t = 0.0, t1 = 100.0;
	double h = 1e-6;
	double y[2] = { 1.0, 0.0 };

	while (t < t1)
	{
	   int status = gsl_odeiv_evolve_apply (e, c, s,
											&sys,
											&t, t1,
											&h, y);

	   if (status != GSL_SUCCESS)
		   break;

	   printf ("(unitTestODE) %.5e %.5e %.5e\n", t, y[0], y[1]);
	}

	gsl_odeiv_evolve_free (e);
	gsl_odeiv_control_free (c);
	gsl_odeiv_step_free (s);
	return 0;
}

P1906MOL_ExtendedDiffusion::~P1906MOL_ExtendedDiffusion ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3