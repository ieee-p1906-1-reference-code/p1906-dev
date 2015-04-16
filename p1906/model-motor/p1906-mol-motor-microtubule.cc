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
 
//! \details IEEE 1906 Component map to the molecular motor instantiation
//!
//! <pre>
//!  IEEE 1906 Component        Molecular Motor 
//!                              Instantiation
//! +----------------------+-----------------------+
//! |                      |                       |
//! |    MESSAGE           |  MOTOR CARGO          |
//! |                      |                       |
//! +----------------------------------------------+
//! |                      |                       |
//! |    MESSAGE CARRIER   |  MOLECULAR MOTOR      |
//! |                      |                       |
//! +----------------------------------------------+
//! |                      |                       |
//! |    MOTION            |  BROWNIAN / WALK      |
//! |                      |                       |
//! +----------------------------------------------+
//! |                      |                       |
//! |    FIELD             |  MICROTUBULE          |
//! |                      |                       |
//! +----------------------------------------------+
//! |                      |                       |
//! |    PERTURBATION      |  MOTOR CARGO TYPE     |
//! +----------------------------------------------+
//! |                      |                       |
//! |    SPECIFICITY       |  BINDING TO TARGET    |
//! |                      |                       |
//! +----------------------+-----------------------+
//! 
//!            Motor and Microtubule Network
//!
//!     XXXXX                                             
//!         XXX XX                 XXXX                 
//!                XX X       XXXXX   XXX               
//!            _        XXXXXXXX          XXXXXX         
//!            /|        XXXXX                 XXX       
//!    +-+   /     XXXXXX    XX                 XXX     
//!    +-+      XXX           XX                  XX    
//! MOLECULAR XXX                X                  XXXXX  
//!   MOTOR                      XXXXX                  X
//!                                                     
//!            XXXXX                                    
//!                 X XX X                              
//!                       X X XX                      XX
//!                             XXXX XX XXXXXX XXXXXXX  
//!                                                     
//!                              MICROTUBULES
//! </pre>
  
#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/simulator.h"

#include "ns3/p1906-mol-motor-microtubule.h"
#include "ns3/p1906-mol-motor-MathematicaHelper.h"
#include "ns3/p1906-mol-motor-MATLABHelper.h"
#include "ns3/p1906-metrics.h"
#include "ns3/p1906-mol-motor.h"
#include "ns3/p1906-mol-motor-tube.h"
#include "ns3/p1906-mol-motor-pos.h"
#include "ns3/p1906-mol-motor-motion.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_MicrotubulesField");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_MOTOR_MicrotubulesField);

TypeId P1906MOL_MOTOR_MicrotubulesField::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_MicrotubulesField")
    .SetParent<P1906MOL_MOTOR_Field> ()
	.AddConstructor<P1906MOL_MOTOR_MicrotubulesField> ()
	.AddAttribute ("ReportTubes",
	               "Write tube locations to file tubes.mma [boolean].",
				   	BooleanValue (true),
				    MakeBooleanAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_report_tubes),
				    MakeBooleanChecker ())
	.AddAttribute ("Volume",
	               "The volume within which microtubules are created in cubic nm where the origin is the lower left location of the space.",
				   DoubleValue (25.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_volume),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("MeanTubeOrientationPhi",
	              "The initial direction of growth of microtubules in radians in 3D space using spherical coordinates. This is the azimuthal angle phi.",
				   DoubleValue (0.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_orientation_phi),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("MeanTubeOrientationTheta",
	              "The initial direction of growth of microtubules in radians in 3D space using spherical coordinates. This is the polar angle theta.",
				   DoubleValue (M_PI),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_orientation_theta),
				   MakeDoubleChecker<double_t> ())				   
	.AddAttribute ("MeanLength",
	              "The expected length of a microtubule in nm.",
				   DoubleValue (100.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_mean_length),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("MeanIntraTubeAngle",
	              "The expected angle between segments of a microtubule in radians.",
				   DoubleValue (30.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_mean_intra_tube_angle),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("MeanInterTubeAngle",
	               "The expected angle between microtubules in radians (overridden by persistence length).",
				   DoubleValue (10.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_mean_inter_tube_angle),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("MeanTubeDensity",
	               "The expected density of microtubules in segments per cubic nm.",
				   DoubleValue (10.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_mean_density),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("SegmentLength",
	               "The length of each segment comprising a microtubule in nm.",
				   DoubleValue (10.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_segment_length),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("NumberOfSegments",
	               "The total number of all segments comprising a microtubule.",
				   IntegerValue (10),
				   MakeIntegerAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_number_of_segments),
				   MakeIntegerChecker<uint32_t> ())
	.AddAttribute ("PersistenceLength",
	               "The persistence length of each microtubule in nm.",
				   DoubleValue (50.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_persistence_length),
				   MakeDoubleChecker<double_t> ())
	.AddAttribute ("SegmentsPerTube",
	               "The number of segments per microtubule.",
				   IntegerValue (10),
				   MakeIntegerAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_segments_per_tube),
				   MakeIntegerChecker<uint32_t> ())
	.AddAttribute ("NumberofTubes",
	               "The total number of microtubules.",
				   IntegerValue (10),
				   MakeIntegerAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_number_of_tubes),
				   MakeIntegerChecker<uint32_t> ())
	.AddAttribute ("StructuralEntropy",
	               "The structural entropy of all microtubules in bits (actually a returned value, not set).",
				   DoubleValue (0.0),
				   MakeDoubleAccessor (&P1906MOL_MOTOR_MicrotubulesField::m_structural_entropy),
				   MakeDoubleChecker<double_t> ())
	;
  return tid;
}

/** 
  This class implements persistence length as described in:
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
  
  Next steps:
	  \todo consider implementing Random Markov Field (RMF) for analysis of motor motion
	  \todo curve fit to tube segments to obtain vector field equation (vector field reconstruction) is currently done in separate Mathematica code
	  \todo relate structural entropy to energy, force, and chemical complexity
	  \todo add convection-diffusion using vector field lines
	  \todo could grow tubes from alternating ends so that starting point is in the center
	  \todo consider many motors operating simultaneously: they would need to update simultaneously  
	  \todo plot motor starting in random location versus time to reach destination as function of tube structure
	  \todo plot structural entropy versus delay
	  \todo plot binding time versus delay
	  \todo plot distance travelled versus delay, structural entropy, etc.
*/
P1906MOL_MOTOR_MicrotubulesField::P1906MOL_MOTOR_MicrotubulesField () : 
m_report_tubes(true),
m_volume(25.0),
m_orientation_phi(0.0),
m_orientation_theta(M_PI/2.0),
m_mean_length(100.0),
m_mean_intra_tube_angle(30.0),
m_mean_inter_tube_angle(10.0),
m_mean_density(10.0),
m_segment_length(10.0),
m_number_of_segments(10),
m_persistence_length(500.0),
m_segments_per_tube(10),
m_number_of_tubes(10),
m_structural_entropy(0.0)
{  
  NS_LOG_FUNCTION(this);
  //! allocate and start the random number generator
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);
  gsl_rng_env_setup();
    
  //! set the microtubule network properties
  setTubeVolume(m_volume);
  setTubeOrientationPhi(m_orientation_phi);
  setTubeOrientationTheta(m_orientation_theta);
  setTubeLength(m_mean_length);
  setTubeIntraAngle(m_mean_intra_tube_angle);
  setTubeInterAngle(m_mean_inter_tube_angle);
  setTubeDensity(m_mean_density);
  setTubePersistenceLength(m_persistence_length);
  setTubeSegments(m_segments_per_tube);
   
  NS_LOG_FUNCTION (this << "Created P1906MOL_MOTOR_MicrotubulesField");
}

std::ostream& operator<<(std::ostream& out, const P1906MOL_MOTOR_MicrotubulesField& m_field)
{
  // display segments that comprise the tube tubeMatrix (see displayTube)
  for (size_t i = 0; i < m_field.tubeMatrix->size1; i++)
  {
    for (size_t j = 0; j < m_field.tubeMatrix->size2; j++)
	{
	  out << gsl_matrix_get (m_field.tubeMatrix, i, j) << " ";
    }
  }
  return out;
}

std::istream& operator>>(std::istream& is, P1906MOL_MOTOR_MicrotubulesField& m_field)
{
  // read in the segments for the field
  double pt_x, pt_y, pt_z;
  size_t i;
  
  i = 0;
  while (is >> pt_x >> pt_y >> pt_z)
  {
    gsl_matrix_set (m_field.tubeMatrix, i, 0, pt_x);
	gsl_matrix_set (m_field.tubeMatrix, i, 1, pt_y);
	gsl_matrix_set (m_field.tubeMatrix, i, 2, pt_z);
	i++;
  }
  
  return is;
}

//! print tube characteristics to standard output
void P1906MOL_MOTOR_MicrotubulesField::displayTubeChars()
{
  NS_LOG_INFO ("volume = " << ts.volume << " " <<
               "mean_tube_length = " << ts.mean_tube_length << " " <<
	           "mean_intra_tube_angle = " << ts.mean_intra_tube_angle << " " <<
	           "mean_inter_tube_angle = " << ts.mean_inter_tube_angle << " " <<
	           "mean_tube_density = " << ts.mean_tube_density << " " <<
	           "segLength = " << ts.segLength << " " <<
	           "numSegments = " << ts.numSegments
  );
}

//! create the microtubules
void P1906MOL_MOTOR_MicrotubulesField::createTubes()
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;

  NS_LOG_INFO (this);
  //! create the microtubules
  tubeMatrix = gsl_matrix_alloc (ts.numTubes * ts.segPerTube, 6);
  genTubes();
  std::ostringstream strs;
  strs << "tubes-" << Simulator::Now().GetSeconds() << ".mma";
  std::string fname = strs.str();
  cout << fname << "\n";
  if (m_report_tubes) mathematica.tubes2Mma(tubeMatrix, ts.segPerTube, fname.c_str());
  NS_LOG_DEBUG ("completed tube creation");

  //! create the vector field
  vf = gsl_matrix_alloc (ts.numTubes * ts.segPerTube, 6);
  tubes2VectorField(tubeMatrix, vf);
  
  Simulator::Schedule(Seconds(0.5), &P1906MOL_MOTOR_MicrotubulesField::createTubes, this);

  //! test the computation of distance
  //unitTest_Distance();

  //! test computation of a single overlap
  //unitTest_Overlap();

  //! test computation of all segment overlaps
  //unitTest_AllOverlaps();

  //! test the plotting the vector field
  //unitTest_VectorField();
  
  //! test the volume surface flux measurement
  //unitTest_VolSurface();

  //vector<P1906MOL_MOTOR_Pos> pts;
  //! test the movement of a motor floating to a tube and walking along the tube
  //unitTest_MotorMovement(pts);
    
  //! test plot2mma to plot the position history stored in pts
  //unitTest_Plot2Mma(pts);
  
  //! start at zero and utilize Brownian motion to reach destination
  //unitTest_NoTubeMotion();
  
  //! test motor movement to destination using Brownian motion or microtubules if available
  //unitTest_MotorMove2Destination(pts);
  
  //! test the reflective barrier volume surface
  //unitTest_ReflectiveBarrier();
  
  //! test the FluxMeter
  //unitTest_FluxMeter();
  
  //! test persistence length versus entropy plot - NB: this test changes the tubeMatrix
  //unitTest_PersistenceLengthsVsEntropy();
}

//! set the space in which the tube centers will be formed
void P1906MOL_MOTOR_MicrotubulesField::setTubeVolume(double volume)
{
  NS_LOG_INFO (volume);
  ts.volume = volume;
}

//! set the space in which the tube centers will be formed
void P1906MOL_MOTOR_MicrotubulesField::setTubeOrientationPhi(double orientation_phi)
{
  NS_LOG_INFO (orientation_phi);
  ts.mean_orientation_phi = orientation_phi;
}

//! set the space in which the tube centers will be formed
void P1906MOL_MOTOR_MicrotubulesField::setTubeOrientationTheta(double orientation_theta)
{
  NS_LOG_INFO (orientation_theta);
  ts.mean_orientation_theta = orientation_theta;
}

//! set the mean tube length, segment is arbitrarily set to 1/5 the length of a tube
void P1906MOL_MOTOR_MicrotubulesField::setTubeLength(double mean_tube_length)
{
  NS_LOG_INFO (mean_tube_length);
  ts.mean_tube_length = mean_tube_length;
  ts.segLength = ts.mean_tube_length / 5;
}

//! set the mean angle between segments within a tube
void P1906MOL_MOTOR_MicrotubulesField::setTubeIntraAngle(double mean_intra_tube_angle)
{
  NS_LOG_INFO (mean_intra_tube_angle);
  ts.mean_intra_tube_angle = mean_intra_tube_angle;
}

//! set the mean angle between tubes
void P1906MOL_MOTOR_MicrotubulesField::setTubeInterAngle(double mean_inter_tube_angle)
{
  NS_LOG_INFO (mean_inter_tube_angle);
  ts.mean_inter_tube_angle = mean_inter_tube_angle;
}

//! this is really the segment density and also derives and sets the total number of segments based on the volume
void P1906MOL_MOTOR_MicrotubulesField::setTubeDensity(double mean_tube_density)
{
  NS_LOG_INFO (mean_tube_density);
  ts.mean_tube_density = mean_tube_density;
  ts.numSegments = ts.mean_tube_density * ts.volume;
}

//! set the persistence length of each tube
void P1906MOL_MOTOR_MicrotubulesField::setTubePersistenceLength(double persistenceLength)
{
  NS_LOG_INFO (persistenceLength);
  ts.persistenceLength = persistenceLength;
}

//! set the number of segments per tube and also derives and sets the number of tubes
void P1906MOL_MOTOR_MicrotubulesField::setTubeSegments(size_t segPerTube)
{
  NS_LOG_INFO (segPerTube);
  ts.segPerTube = segPerTube;
  ts.numTubes = floor(ts.numSegments / ts.segPerTube);
}

//! return the size of the matrix to allocate
void P1906MOL_MOTOR_MicrotubulesField::getTubesSize(double * rows, double * cols)
{
  *rows = tubeMatrix->size1;
  *cols = tubeMatrix->size2;
  NS_LOG_INFO (*rows << *cols);
}

//! export tubes; copy the object's tubeMatrix into tm for use outside the object
void P1906MOL_MOTOR_MicrotubulesField::getTubes(gsl_matrix * tm)
{
  NS_LOG_FUNCTION (this);
  // copy tubeMatrix to tm, but only if tm->size1 and tm->size2 are consistent with tubeMatrix
  if (tm->size1 == tubeMatrix->size1 && tm->size2 == tubeMatrix->size2)
  NS_LOG_WARN ("sizes not equal tm->size1: " << tm->size1 << 
               " tubeMatrix_size1: " << tubeMatrix->size1 << 
			   " tm->size2: " << tm->size2 << 
			   " tubeMatrix->size2: " << tubeMatrix->size2);
  gsl_matrix_memcpy (tm, tubeMatrix);
}

//! import tubes; create a local copy of the tube structure tm in tubeMatrix
void P1906MOL_MOTOR_MicrotubulesField::setTubes(gsl_matrix * tm)
{
  NS_LOG_FUNCTION (this);
  //copy tm to tubeMatrix, but only if same size tm->size1 and tm->size2
  gsl_matrix_memcpy (tubeMatrix, tm);
  // may need to free and then reallocate if different size
}

//! for each of the persistenceLengths in the vector, generate tubes and plot persistence length versus 
//! structural entropy in persistenceVersusEntropy.mma, also write tubes_<n>.mma for each of the persistenceLengths
void P1906MOL_MOTOR_MicrotubulesField::persistenceVersusEntropy(gsl_vector * persistenceLengths)
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  char plot_filename[256];
  //! store the results here
  gsl_matrix * pve = gsl_matrix_alloc (persistenceLengths->size, 2);
  
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("persistenceLengths->size: " << persistenceLengths->size);
  for (size_t i = 0; i < persistenceLengths->size; i++)
  {	
    NS_LOG_DEBUG ("persistenceLengths(" << i << ") = " << gsl_vector_get (persistenceLengths, i));
    setTubePersistenceLength (gsl_vector_get (persistenceLengths, i));
    genTubes();
	
	NS_LOG_DEBUG ("i: " << i << 
	              " tubeMatrix: " << tubeMatrix->size1 << 
				  " x " << tubeMatrix->size2 << 
				  " segPerTube: " << ts.segPerTube);
	//gsl_matrix_fprintf (stdout, tubeMatrix, "%f");
	
	//! store the set of tubes
	sprintf (plot_filename, "tubes_%ld.mma", i);
    mathematica.tubes2Mma(tubeMatrix, ts.segPerTube, plot_filename);
	gsl_matrix_set (pve, i, 0, gsl_vector_get (persistenceLengths, i));
	gsl_matrix_set (pve, i, 1, ts.se);
  }
  
  //! plot the results
  mathematica.plot2Mma(pve, "persistenceVersusEntropy.mma", "persistence length", "structural entropy");
}

//! generate a set of tubes comprised of a total of numSegments in volume with segPerTube segments of segLength 
//! and persistenceLength and return in tubeMatrix
void P1906MOL_MOTOR_MicrotubulesField::genTubes()
{
  //! create a given density of tubes of numSegments in given volume
  //! volume starts at 0, 0, 0 to volume^(1/4) in each dimension
  
  NS_LOG_FUNCTION (this);
  //! don't delete, just refill the tubeMatrix
  //! free the matrix of any previous allocation
  //gsl_matrix_free (tubeMatrix);
  //! re-allocate the tubeMatrix
  //tubeMatrix = gsl_matrix_alloc (ts->numTubes * ts->segPerTube, 6);

  //! \todo get actual tube graph properties from biologist
  gsl_vector * startPt = gsl_vector_alloc (3);
  //! hold the values for a tube comprised of many segments: x_start y_start x_start x_end y_end z_end
  gsl_matrix * segMatrix = gsl_matrix_alloc (ts.segPerTube, 6);
  double total_structural_entropy = 0;
  
  NS_LOG_DEBUG ("tubeMatrix: " << tubeMatrix->size1 << 
                " x " << tubeMatrix->size2 << 
				" numTubes: " << ts.numTubes << 
				" segPerTube: " << ts.segPerTube << 
				" volume: " << ts.volume);
  
  //! volume starts at 0, 0, 0 to volume^(1/4) in each dimension
  for(size_t i = 0; i < ts.numTubes; i++)
  {
    //! set the starting location for the tube
    point (startPt, 
	  gsl_ran_gaussian (r, pow(ts.volume, (1/4))),
	  gsl_ran_gaussian (r, pow(ts.volume, (1/4))),
	  gsl_ran_gaussian (r, pow(ts.volume, (1/4))));

	//! create a single tube
    P1906MOL_MOTOR_Tube tube(&ts, startPt);
	
	//! accumulate the structural entropy
	total_structural_entropy += (ts.se);
	
	//! retrieve MT into segMatrix
	tube.getSegmatrix(segMatrix);
	//! copy this MT into main tube matrix, tubeMatrix
	for(size_t j = 0; j < ts.segPerTube; j++)
	  for(size_t k = 0; k < 6; k++)
	  {
	    gsl_matrix_set(tubeMatrix, i * ts.segPerTube + j, k, gsl_matrix_get(segMatrix, j, k));
	  }
  }
  
  ts.se = total_structural_entropy;
}

//! \todo test the volume surface as a flux meter and later as a compartmentalization volume
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_VolSurface()
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  P1906MOL_MOTOR_Pos c;
  c.setPos (0, 0, 0);
  P1906MOL_MOTOR_VolSurface vs;
  vector<P1906MOL_MOTOR_Pos> ipt;
  gsl_vector * segment = gsl_vector_alloc (6);
  gsl_vector * radius = gsl_vector_alloc (6);
  gsl_matrix * vectors = gsl_matrix_alloc (3, 6);
  double flux;
  
  // test tube input/output rates relative to the volume surface
  NS_LOG_DEBUG ("beginning unitTest_VolSurface");  
  vs.setVolume (c, 100);
  
  //! segment start point
  gsl_vector_set (segment, 0, 90);
  gsl_vector_set (segment, 1, 90);
  gsl_vector_set (segment, 2, 90);
  
  //! segment end point
  gsl_vector_set (segment, 3, 110);
  gsl_vector_set (segment, 4, 110);
  gsl_vector_set (segment, 5, 110);
  
  vs.sphereIntersections(segment, ipt);
  
  //! draw the volume surface
  mathematica.volSurfacePlot(vs.center, vs.radius, "volsurface.mma");
  
  for (size_t i = 0; i < ipt.size(); i++)
  {
    NS_LOG_INFO ("intersecting point(s): " << ipt.at(i));
  }
  
  //! plot the intersection points with the volume
  mathematica.points2Mma(ipt, "volsurfintersections.mma");
  
  //! find the vector angle with the sphere surface: 
  //!   compare intersecting segment with sphere radius to determine angle
  
  //! radius start point is the same as the intersecting tube segment start point
  gsl_vector_set (radius, 0, 90);
  gsl_vector_set (radius, 1, 90);
  gsl_vector_set (radius, 2, 90);
  
  //! radius end point is at the center of the sphere
  gsl_vector_set (radius, 3, 0);
  gsl_vector_set (radius, 4, 0);
  gsl_vector_set (radius, 5, 0);
  
  double angle = vs.vectorAngle(segment, radius);
  NS_LOG_DEBUG ("angle: " << angle);
	
  insertVector(vectors, 0, segment);
  insertVector(vectors, 1, radius);
  
  mathematica.vectorPlotMma (vectors, "volsurfvector.mma");
  
  P1906MOL_MOTOR_Pos last_pos;
  P1906MOL_MOTOR_Pos current_pos;
  
  /*
   * reflection 1
   */
  
  last_pos.setPos (40, 40, 40);
  NS_LOG_DEBUG ("last_pos " << last_pos);
  current_pos.setPos (110, 110, 110);
  NS_LOG_DEBUG ("current_pos " << current_pos);
  
  line (segment, last_pos, current_pos);
  vs.sphereIntersections(segment, ipt);
  vs.getRadiusLine(radius, ipt.front()); //! assuming at least one intersection returned
  line (segment, ipt.front(), current_pos);
  
  insertVector(vectors, 0, segment);
  insertVector(vectors, 1, radius);
  line (segment, last_pos, ipt.front());
  insertVector(vectors, 2, segment);
  
  mathematica.vectorPlotMma (vectors, "volsurfvector1beforereflection.mma");
  
  vs.reflect(last_pos, current_pos);
  NS_LOG_DEBUG ("new current_pos " << current_pos);
  
  //! update the reflected segment to display
  line (segment, ipt.front(), current_pos);
  insertVector(vectors, 0, segment);
  insertVector(vectors, 1, radius);
  line (segment, last_pos, ipt.front());
  insertVector(vectors, 2, segment);
  
  mathematica.vectorPlotMma (vectors, "volsurfvector1afterreflection.mma");
  
  /*
   * reflection 2
   */
  
  last_pos.setPos (20, 40, 20);
  NS_LOG_DEBUG ("last_pos " << last_pos);
  current_pos.setPos (120, 90, 90);
  NS_LOG_DEBUG ("(unitTest_VolSurface) current_pos " << current_pos);

  line (segment, last_pos, current_pos);
  vs.sphereIntersections(segment, ipt);
  vs.getRadiusLine(radius, ipt.front()); //! assuming at least one intersection returned
  line (segment, ipt.front(), current_pos);
  
  insertVector(vectors, 0, segment);
  insertVector(vectors, 1, radius);
  line (segment, last_pos, ipt.front());
  insertVector(vectors, 2, segment);
  
  mathematica.vectorPlotMma (vectors, "volsurfvector2beforereflection.mma");
  
  vs.reflect(last_pos, current_pos);
  NS_LOG_DEBUG ("new current_pos " << current_pos);
  
  //! update the reflected segment to display
  line (segment, ipt.front(), current_pos);
  insertVector(vectors, 0, segment);
  
  line (segment, last_pos, current_pos);
  insertVector(vectors, 1, radius);
  line (segment, last_pos, ipt.front());
  insertVector(vectors, 2, segment);
  
  mathematica.vectorPlotMma (vectors, "volsurfvector2afterreflection.mma");
  
  //! measure flow through surface
  flux = vs.fluxMeter(tubeMatrix);
  NS_LOG_DEBUG ("flux: " << flux);
  
  NS_LOG_DEBUG ("completed unitTest_VolSurface");
  
  return true;
}

//! test the volume surface as a compartmentalization volume
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_ReflectiveBarrier()
{
  Ptr<P1906MOL_Motor> motor = CreateObject<P1906MOL_Motor> ();
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  double timePeriod = 100;
  P1906MOL_MOTOR_Pos volCenter;
  P1906MOL_MOTOR_Motion motion;

  NS_LOG_DEBUG ("Beginning");
  //! start at zero
  point (startPt, 0, 0, 0);
  volCenter.setPos (0, 0, 0);
  
  //! \todo add test if the ReflectiveBarrier includes the Receiver destination volume, if not, warn that motion will continue forever
  motor->addVolumeSurface (volCenter, 1500.0, P1906MOL_MOTOR_VolSurface::ReflectiveBarrier);
  
  //! reset the motor's timer
  motor->initTime();
    
  //! volume surface must overlap with destination in order for the test to end
  motor->displayVolSurfaces();
  /*
   * move randomly until destination reached
   */
  motor->setStartingPoint(startPt);
  motion.float2Destination(motor, timePeriod);
  NS_LOG_DEBUG ("propagation time: " << motor->getTime());
  mathematica.connectedPoints2Mma(motor->pos_history, "float2destination.mma");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test the FluxMeter
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_FluxMeter()
{
  Ptr<P1906MOL_Motor> motor = CreateObject<P1906MOL_Motor> ();
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  double timePeriod = 100;
  P1906MOL_MOTOR_Pos volCenter;
  P1906MOL_MOTOR_Motion motion;

  NS_LOG_DEBUG ("Beginning");
  //! start at zero
  point (startPt, 0, 0, 0);
  volCenter.setPos (0, 0, 0);
  
  //! add a FluxMeter surface volume
  motor->addVolumeSurface (volCenter, 1500.0, P1906MOL_MOTOR_VolSurface::FluxMeter);
  
  //! reset the motor's timer
  motor->initTime();
    
  //! volume surface must overlap with destination in order for the test to end
  motor->displayVolSurfaces();  
  /*
   * move randomly until destination reached
   */
  motor->setStartingPoint(startPt);
  motion.float2Destination(motor, timePeriod);
  NS_LOG_DEBUG ("propagation time: " << motor->getTime());
  mathematica.connectedPoints2Mma(motor->pos_history, "float2destination.mma");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test distance calculation
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_Distance()
{
  gsl_vector * startPt = gsl_vector_alloc (3);
  gsl_vector * segment = gsl_vector_alloc (6);
  gsl_vector * pt1 = gsl_vector_alloc (3);
  gsl_vector * pt2 = gsl_vector_alloc (3);
  
  NS_LOG_DEBUG ("Beginning");
  point (startPt, 0, 0, 0);
  point (pt1, -1, -1, -1);
  point (pt2, 2, 2, 2);
  line (segment, pt1, pt2);
  double d = P1906MOL_MOTOR_Field::distance (startPt, segment);
  NS_LOG_DEBUG ("distance: " << d);
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test finding a segment overlap
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_Overlap()
{
  gsl_vector * segment3D = gsl_vector_alloc (6);
  gsl_matrix * pts3D = gsl_matrix_alloc (1, 3);
  gsl_matrix * tubeMatrix3D = gsl_matrix_alloc (1, 6);
  gsl_matrix_set_zero (pts3D);
  gsl_vector * pt1 = gsl_vector_alloc (3);
  gsl_vector * pt2 = gsl_vector_alloc (3);
  gsl_vector * pt3 = gsl_vector_alloc (3);
  gsl_vector * pt4 = gsl_vector_alloc (3);
  gsl_vector * tubeSegments = gsl_vector_alloc (1);
  
  NS_LOG_DEBUG ("Beginning");
  point (pt1, 0, 0, 0);
  point (pt2, 5, 5, 0);
  point (pt3, 5, 0, 0);
  point (pt4, 0, 5, 0);
  line (segment3D, pt1, pt2);
  line (tubeMatrix3D, 0, pt3, pt4);
  getOverlap3D(segment3D, tubeMatrix3D, pts3D, tubeSegments);
  if (isPointOverlap(pt1, segment3D))
    NS_LOG_INFO ("point overlaps");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test finding all segment overlaps in a tube network
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_AllOverlaps()
{
  vector<P1906MOL_MOTOR_Pos> pts;
  P1906MOL_MOTOR_MathematicaHelper mathematica;

  NS_LOG_DEBUG ("Beginning");
  getAllOverlaps3D(tubeMatrix, pts);
  mathematica.points2Mma(pts, "pfile.mma");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! plot persistence length versus entropy
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_PersistenceLengthsVsEntropy()
{
  gsl_vector * persistenceLengths = gsl_vector_alloc (10);
  
  NS_LOG_DEBUG ("Beginning");
  for (size_t i = 0; i < 10; i++)
    gsl_vector_set (persistenceLengths, i, i * 100);
  persistenceVersusEntropy(persistenceLengths);
  NS_LOG_DEBUG ("Completed");

  return true;
}

//! test creating a vector field with tubes2VectorField, creates vectorField.mma and vectorField.dat
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_VectorField()
{
  P1906MOL_MOTOR_MATLABHelper matlab;
  P1906MOL_MOTOR_MathematicaHelper mathematica;

  NS_LOG_DEBUG ("Beginning");
  mathematica.vectorFieldPlotMma(vf, "vectorField.mma");
  matlab.vectorFieldMeshMATLAB(vf, "vectorField.dat");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test motor movement using Brownian motion until destination volume reached
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_NoTubeMotion()
{
  Ptr<P1906MOL_Motor> motor = CreateObject<P1906MOL_Motor> ();
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  double timePeriod = 100;
  P1906MOL_MOTOR_Motion motion;

  NS_LOG_DEBUG ("Beginning");
  //! reset the motor's timer
  motor->initTime();
  
  //! start at zero
  point (startPt, 0, 0, 0);
  
  //! volume surface must overlap with destination in order for the test to end
  motor->displayVolSurfaces();  
  /*
   * move randomly until destination reached
   */
  motor->setStartingPoint(startPt);
  motion.float2Destination(motor, timePeriod);
  //NS_LOG_DEBUG ("propagation time: " << motor.getTime());
  mathematica.connectedPoints2Mma(motor->pos_history, "float2destination.mma");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test motor movement to a tube and then walking along the tube
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_MotorMovement(vector<P1906MOL_MOTOR_Pos> & pts)
{
  Ptr<P1906MOL_Motor> motor = CreateObject<P1906MOL_Motor> ();
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  //double timePeriod = 100;
  P1906MOL_MOTOR_Motion motion;

  NS_LOG_DEBUG ("Beginning");
  //! reset the motor's timer
  motor->initTime();
  
  /*
   * move randomly until overlap with tube
   */
  motor->pos_history.clear();
  point (startPt, 
    gsl_matrix_get (tubeMatrix, 0, 0) + 30, //! start 10 nanometers away from the first tube segment
	gsl_matrix_get (tubeMatrix, 0, 1),
	gsl_matrix_get (tubeMatrix, 0, 2));
  motion.float2Tube(motor, r, startPt, motor->pos_history, tubeMatrix, 0.1, motor->vsl);
  NS_LOG_DEBUG ("Completed float2Tube");
  NS_LOG_DEBUG ("float2Tube propagation time: " << motor->getTime());
  NS_LOG_DEBUG ("float2Tube number of positions: " << motor->pos_history.size());
  mathematica.connectedPoints2Mma(motor->pos_history, "motion2tube.mma");
  NS_LOG_DEBUG ("Completed");
  
  //! start where the motor ended
  P1906MOL_MOTOR_Pos Pos;
  Pos = motor->pos_history.back();
  double x, y, z;
  Pos.getPos (&x, &y, &z);
  point(startPt, x, y, z);
  NS_LOG_DEBUG ("starting point for motor walk near tube: " << startPt);

  /*
   * now walk along the tube
   */
  motor->pos_history.clear();
  motion.motorWalk(motor, r, startPt, motor->pos_history, tubeMatrix, ts.segPerTube, motor->vsl);
  NS_LOG_DEBUG ("motorWalk propagation time: " << motor->getTime());
  NS_LOG_DEBUG ("motorWalk number of positions: " << motor->pos_history.size());
  mathematica.connectedPoints2Mma(motor->pos_history, "motion2end_of_tube.mma");
  //! append the motor history into pts
  pts.insert(pts.end(), motor->pos_history.begin(), motor->pos_history.end());
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test plotting to a Mathematica file using plot2mma, creates plottest.mma
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_Plot2Mma(vector<P1906MOL_MOTOR_Pos> & pts)
{
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  P1906MOL_MOTOR_Pos Pos;
  double x, y, z;
  
  NS_LOG_DEBUG ("Beginning");
  NS_LOG_DEBUG ("number of points: " << pts.size());
  if (pts.size() == 0) //! nothing to plot
  {
    NS_LOG_WARN ("nothing to plot");
    return false;
  }
  
  //! test plotting points - move to a unit test
  gsl_matrix * vals = gsl_matrix_alloc (pts.size(), 2);
  for (size_t i = 0; i < pts.size(); i++)
  {
    Pos = pts.at(i);
	Pos.getPos (&x, &y, &z);
    //for (size_t j = 0; j < 2; j++)
    gsl_matrix_set (vals, i, 0, x);
	gsl_matrix_set (vals, i, 1, y);
  }
  mathematica.plot2Mma(vals, "plottest.mma", "x value", "y value");
  NS_LOG_DEBUG ("Completed");
  
  return true;
}

//! test motor movement to destination using Brownian and microtubules if they exist
bool P1906MOL_MOTOR_MicrotubulesField::unitTest_MotorMove2Destination(vector<P1906MOL_MOTOR_Pos> & pts)
{
  Ptr<P1906MOL_Motor> motor = CreateObject<P1906MOL_Motor> ();
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  gsl_vector * startPt = gsl_vector_alloc (3);
  double timePeriod = 100;
  P1906MOL_MOTOR_Motion motion;

  NS_LOG_DEBUG ("Beginning");
  //! reset the motor's timer
  motor->initTime();
  
  //! start at zero
  point (startPt, 0, 0, 0);

  motor->pos_history.clear(); //! reset the position history
  motor->setStartingPoint(startPt);

  motion.move2Destination(motor, tubeMatrix, ts.segPerTube, timePeriod, motor->pos_history);
  NS_LOG_DEBUG ("propagation time: " << motor->getTime());
  mathematica.connectedPoints2Mma(motor->pos_history, "motion2destination.mma");
  //! append the motor history into pts
  pts.insert(pts.end(), motor->pos_history.begin(), motor->pos_history.end());
  NS_LOG_DEBUG ("completed");
  
  return true;
}

P1906MOL_MOTOR_MicrotubulesField::~P1906MOL_MOTOR_MicrotubulesField ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
