/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2014 by IEEE.
 *
 *  This source file is an essential part of IEEE P1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE P1906.1 may also be made and
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

/*
 * Description:
 * this file models the molecular communication based on microtubules structures in class P1906MOL_MOTOR_Field
 */
 
//! \todo plot some results
//! \todo modify _RUN_MOTOR_CHANNEL_CAPACITY_.sh to generate plot of distance versus Brownian motion
//! \todo extra-credit: modify _RUN_MOTOR_CHANNEL_CAPACITY_.sh to generate plot of distance versus Brownian motion WITH TUBES @ given orientations
//! \todo extra-credit: modify _RUN_MOTOR_CHANNEL_CAPACITY_.sh to generate plot of structural entropy WITH TUBES @ given orientations
//! \todo update/complete the extensions/README.txt file
//! \todo rerun Doxygen for final SVN update
 
//! \details 1906 Component map to the molecular motor instantiation
//! <pre>
//!  1906 Component             Molecular Motor 
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
//! <pre>
//!          The Surface Measures Flux, Constrains Particle 
//!                 Motion, and Defines a Receiver
//!                     _,.,---''''''''---..__
//!                _.-''                      `-.._
//!             ,-'                                `..
//!          ,-' __                                   `._
//!        ,'  ,'  `-.                                   `.
//!      ,'   /Node 2_\____                                . 
//!     /    |    X   |   /   Brownian Motion               `.
//!    /      \      ,'  /____                                . 
//!   /        `._,,'        /                                 . 
//!  |    Receiver Surface  /                                   |
//!  |                     /    Node 1                          |
//! |                     -------X                              |
//! |                                                           |
//! |                                                           |
//!  |                                                          /
//!  \                                                         /
//!   \                                                       ,'
//!    \                                                     ,'
//!     `.                                                  /
//!       `.                                              ,'
//!         `.                                          ,'
//!           `.                                     _,'
//!             `-._                              ,,'
//!                 `-..__                  _,.-''
//!                       ``---........---''
//!                Reflective Barrier Volume Surface
//!           
//!</pre>

#include "ns3/log.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"

#include "ns3/p1906-helper.h"
#include "ns3/p1906-net-device.h"

#include "ns3/p1906-mol-motor-medium.h"
#include "ns3/p1906-mol-motor-specificity.h"
#include "ns3/p1906-mol-motor-perturbation.h"
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-motion.h"
#include "ns3/p1906-mol-motor-microtubule.h"
#include "ns3/p1906-mol-motor-communication-interface.h"

using namespace ns3;

static void 
MessagesReceivedTracer (double oldval, double newval)
{
	std::cout << "Traced " << oldval << " to " << newval << std::endl;
}

int main (int argc, char *argv[])
{	

  //set of parameters
  double m_diffusion_coefficient = 1;				// [nm^2/ns]
  double m_tube_volume = 25; 						// [nm^3]
  double m_orientation_phi = 3.14/4.0; 				// [radians]
  double m_orientation_theta = 3.14/4.0; 			// [radians]
  double m_mean_tube_length = 100.0; 				// [nm]
  double m_mean_intra_tube_angle = 0.0; 			// [radians]
  double m_mean_inter_tube_angle = 0.0; 			// [radians]
  double m_mean_tube_density = 10.0; 				// [tube segments/nm^3]
  double m_tube_persistence_length = 20.0;			// [nm]
  uint32_t m_seg_per_tube = 100.0; 					// [segments/microtubule]
  std::string m_transmitter_location = "0 0 0"; 	// [x y z nm]
  std::string m_receiver_location = "100 100 100 100"; // [x y z radius nm]
  std::string m_reflective_barrier = "0 0 0 1000"; 	// [x y z radius nm]
  double m_packet_rate = 1;							// [packets/s]
  uint32_t m_num_packets = 1;						// [-]
  bool m_report_motor = false;						// [bool]

  CommandLine cmd;
  cmd.AddValue("diffusion-coefficient", "Diffusion coefficient [nm^2/ns].", m_diffusion_coefficient);
  cmd.AddValue("tube-volume", "Volume in which tubes may form [nm^3].", m_tube_volume);
  cmd.AddValue("mean-tube-length", "The mean length of each tube [nm].", m_mean_tube_length);
  cmd.AddValue("mean_intra_tube_angle", "The mean angle between tube segments [degrees].", m_mean_intra_tube_angle);
  cmd.AddValue("mean_inter_tube_angle", "The mean angle between tubes [degrees].", m_mean_inter_tube_angle);
  cmd.AddValue("mean_tube_density", "The mean density of tube segments within the volume [segments/nm^3].", m_mean_tube_density);
  cmd.AddValue("tube_persistenceLength", "The mean tube persistence length [nm].", m_tube_persistence_length);
  cmd.AddValue("segments-per-tube", "The number of segments per tube [-].", m_seg_per_tube);
  cmd.AddValue("transmitter-location", "The location where motors are released [x y z nm].", m_transmitter_location);
  cmd.AddValue("receiver-location", "Defines the receiver volume [x y z radius nm].", m_receiver_location);
  cmd.AddValue("reflective-barrier", "Defines the containing reflective barrier [x y z radius nm].", m_reflective_barrier);
  cmd.AddValue("packet-rate", "Rate of packet generation [packets/s].", m_packet_rate);
  cmd.AddValue("num-packets", "Total number of packets to create [-].", m_num_packets);
  cmd.AddValue("report-motor", "Write motor position history file [0].", m_report_motor);
  cmd.AddValue("orientation-phi", "The initial direction of growth of microtubules in degrees in 3D space using spherical coordinates. This is the azimuthal angle phi.", m_orientation_phi);
  cmd.AddValue("orientation-theta", "The initial direction of growth of microtubules in degrees in 3D space using spherical coordinates. This is the polar angle theta.", m_orientation_theta);
  
  cmd.Parse(argc, argv);
  
  // Modify the size of the receiver volume surface area
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::ReportTubes", BooleanValue (true));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::Volume", DoubleValue (m_tube_volume));
  
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanTubeOrientationPhi", DoubleValue (m_orientation_phi));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanTubeOrientationTheta", DoubleValue (m_orientation_theta));

  Config::SetDefault ("ns3::P1906MOL_MOTORSpecificity::Sensitivity", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTORSpecificity::Specificity", DoubleValue (0));
  
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanLength", DoubleValue (m_mean_tube_length));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanIntraTubeAngle", DoubleValue (m_mean_intra_tube_angle));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanInterTubeAngle", DoubleValue (m_mean_inter_tube_angle));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::MeanTubeDensity", DoubleValue (m_mean_tube_density));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::SegmentLength", DoubleValue (10));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::NumberOfSegments", IntegerValue (10));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::PersistenceLength", DoubleValue (m_tube_persistence_length));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::SegmentsPerTube", IntegerValue (m_seg_per_tube));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::NumberofTubes", IntegerValue (10));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_MicrotubulesField::StructuralEntropy", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::TypeOfVolume", IntegerValue (2));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::Radius", DoubleValue (100));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::CenterX", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::CenterY", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::CenterZ", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::Sensitivity", DoubleValue (1));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::Specificity", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::ExpectedLength", DoubleValue (50));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::PersistenceLength", DoubleValue (30));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::Diameter", DoubleValue (24));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::HalfLife", DoubleValue (600));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::RateOfGrowth", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::RateOfShortening", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::FrequencyOfCatastrophe", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::FrequencyOfRescue", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_Tube::Conductivity", DoubleValue (1.5));
  Config::SetDefault ("ns3::P1906MOL_Motor::InitialXLocation", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_Motor::InitialYLocation", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_Motor::InitialZLocation", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_Motor::Source", StringValue ("N. crassa; Kinesin 1"));
  Config::SetDefault ("ns3::P1906MOL_Motor::DomainStructure", StringValue ("Homodimer"));
  Config::SetDefault ("ns3::P1906MOL_Motor::Lifetime", DoubleValue (120));
  Config::SetDefault ("ns3::P1906MOL_Motor::IsAlive", BooleanValue (true));
  Config::SetDefault ("ns3::P1906MOL_Motor::Mass", DoubleValue (1.5));
  Config::SetDefault ("ns3::P1906MOL_Motor::Directionality", BooleanValue (false));
  Config::SetDefault ("ns3::P1906MOL_Motor::Processivity", DoubleValue (1.75));
  Config::SetDefault ("ns3::P1906MOL_Motor::Velocity", DoubleValue (120));
  Config::SetDefault ("ns3::P1906MOL_Motor::ForceGeneration", DoubleValue (8));
  Config::SetDefault ("ns3::P1906MOL_Motor::CargoDiameter", DoubleValue (200));
  Config::SetDefault ("ns3::P1906MOL_Motor::CargoMass", DoubleValue (660));
  Config::SetDefault ("ns3::P1906MOL_Motor::Diameter", DoubleValue (100));
  Config::SetDefault ("ns3::P1906MOL_Motor::MessageInformationDensity", DoubleValue (5.5));
  Config::SetDefault ("ns3::P1906MOL_Motor::CollisionType", IntegerValue (0));
  Config::SetDefault ("ns3::P1906MOL_Motor::BindingRate", DoubleValue (1));
  Config::SetDefault ("ns3::P1906MOL_Motor::EnergyConsumed", DoubleValue (1));
  Config::SetDefault ("ns3::P1906MOL_Motor::LocalizationError", DoubleValue (0));
  Config::SetDefault ("ns3::P1906MOL_Motor::SupersystemDegradation", DoubleValue (0));
  
  Config::SetDefault ("P1906MOL_ExtendedDiffusionWave::Transmission_Time", DoubleValue (0));
  Config::SetDefault ("P1906MOL_ExtendedDiffusionWave::Diffusion_Coefficient", DoubleValue (0));
  Config::SetDefault ("P1906MOL_ExtendedDiffusionWave::Initial_Concentration", DoubleValue (0));
  Config::SetDefault ("P1906MOL_ExtendedDiffusionWave::Transmitter_Location", StringValue("0 0 0"));
  Config::SetDefault ("P1906MOL_ExtendedDiffusionWave::isRemoveLowConcentration", BooleanValue(false));
    
  Time::SetResolution(Time::NS);

  // Create P1906 Helper
  P1906Helper helper;
  //helper.EnableLogComponents ();

  // Create nodes (typical operation of ns-3)
  NodeContainer n;
  NetDeviceContainer d;
  n.Create (2);

  // Create a medium and the Motion component - all parameters should be set via ns-3 configuration above
  Ptr<P1906MOL_MOTORMedium> medium = CreateObject<P1906MOL_MOTORMedium> ();
  Ptr<P1906MOL_MOTOR_Motion> motion = CreateObject<P1906MOL_MOTOR_Motion> ();
  
  motion->SetDiffusionCoefficient (m_diffusion_coefficient);
  medium->SetP1906Motion (motion);

  // Create Device 1 and related components/entities
  Ptr<P1906NetDevice> dev1 = CreateObject<P1906NetDevice> ();
  Ptr<P1906MOL_MOTOR_CommunicationInterface> c1 = CreateObject<P1906MOL_MOTOR_CommunicationInterface> ();
  Ptr<P1906MOL_MOTORSpecificity> s1 = CreateObject<P1906MOL_MOTORSpecificity> ();
    //! this class creates the MTs (field)
  Ptr<P1906MOL_MOTOR_MicrotubulesField> fi1 = CreateObject<P1906MOL_MOTOR_MicrotubulesField> ();
  fi1->setTubeVolume (m_tube_volume);
  fi1->setTubeOrientationPhi(m_orientation_phi);
  fi1->setTubeOrientationTheta(m_orientation_theta);
  fi1->setTubeLength (m_mean_tube_length);
  fi1->setTubeIntraAngle (m_mean_intra_tube_angle);
  fi1->setTubeInterAngle (m_mean_inter_tube_angle);
  fi1->setTubeDensity (m_mean_tube_density);
  fi1->setTubePersistenceLength (m_tube_persistence_length);
  fi1->setTubeSegments (m_seg_per_tube);
  fi1->createTubes(); // could create a watchdog timer here to re-create tubes periodically
  
  //! this class creates the molecular motors (message carrier)
  Ptr<P1906MOL_MOTOR_Perturbation> p1 = CreateObject<P1906MOL_MOTOR_Perturbation> ();
  
  //NS_LOG_DEBUG ("Device 1 created");

  // Create Device 2 and related components/entities
  Ptr<P1906NetDevice> dev2 = CreateObject<P1906NetDevice> ();
  Ptr<P1906MOL_MOTOR_CommunicationInterface> c2 = CreateObject<P1906MOL_MOTOR_CommunicationInterface> ();
  Ptr<P1906MOL_MOTORSpecificity> s2 = CreateObject<P1906MOL_MOTORSpecificity> ();
  //! this class creates the MTs (field)
  Ptr<P1906MOL_MOTOR_MicrotubulesField> fi2 = CreateObject<P1906MOL_MOTOR_MicrotubulesField> ();
  //! this class creates the molecular motors (message carrier)
  Ptr<P1906MOL_MOTOR_Perturbation> p2 = CreateObject<P1906MOL_MOTOR_Perturbation> ();
  
  //NS_LOG_DEBUG ("Device 2 created");

  // Set position of nodes: receiver volume surface is a sphere located at the
  //   receiving Node's location (ComputePropagationDelay) where radius 
  //   is currently set as function of position
  Ptr<ListPositionAllocator> positionAlloc =
		  CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector(0, 0, 0));
  
  std::istringstream iss(m_receiver_location);
  double x, y, z, r;
  iss >> x >> y >> z >> r;
  positionAlloc->Add (Vector(x, y, z));
  Config::SetDefault ("ns3::P1906MOL_MOTOR_VolSurface::Radius", DoubleValue (r));
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install(n);

  // Connect devices, nodes, medium, components and entities
  d.Add (dev1);
  d.Add (dev2);
  helper.Connect(n.Get (0), dev1, medium, c1, fi1, p1, s1);
  helper.Connect(n.Get (1), dev2, medium, c2, fi2, p2, s2);
  
  //NS_LOG_DEBUG ("Connected devices, nodes, medium, components and entities");
    
  // Trace records messages entering the Receiver volume surface
  Config::ConnectWithoutContext ("/NodeList/1/$ns3::P1906MOL_MOTOR_VolSurface::MessagesReceived", MakeCallback (&MessagesReceivedTracer));

  // Create a message to send into the network
  int pktSize = 1; //bytes
  uint8_t *buffer  = new uint8_t[pktSize];
  for (int i = 0; i < pktSize; i++)
    {
	  buffer[i] = 0; //empty information
    }
  
  Ptr<Packet> message = Create<Packet>(buffer, pktSize);
  
  //NS_LOG_DEBUG ("Packet created");

  //! c1 is the P1906MOLCommunicationInterface for Node 1
  c1->HandleTransmission (message);
  
  //NS_LOG_DEBUG ("c1->HandleTransmission (message)");
  
  // Output config store to txt format
  Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("output-attributes.txt"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("RawText"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  ConfigStore outputConfig2;
  outputConfig2.ConfigureDefaults ();
  outputConfig2.ConfigureAttributes ();

  Simulator::Stop (Seconds (100.0));
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}
