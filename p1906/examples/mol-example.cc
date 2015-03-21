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
 * Author: Giuseppe Piro - Telematics Lab Research Group
 *                         Politecnico di Bari
 *                         giuseppe.piro@poliba.it
 *                         telematics.poliba.it/piro
 */




/*
 * Description:
 * this file models the simple example of MOLECULAR-based communication.
 * xxx: add more comments (i.e., description of the scenario)
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/p1906-helper.h"
#include "ns3/p1906-net-device.h"
#include "ns3/p1906-mol-perturbation.h"
#include "ns3/p1906-mol-field.h"
#include "ns3/p1906-mol-motion.h"
#include "ns3/p1906-mol-specificity.h"
#include "ns3/p1906-medium.h"
#include "ns3/p1906-mol-communication-interface.h"
#include "ns3/p1906-mol-transmitter-communication-interface.h"
#include "ns3/p1906-mol-receiver-communication-interface.h"

using namespace ns3;


int main (int argc, char *argv[])
{	

  //set of parameters
  double nodeDistance = 0.001; 								//  [m]
  double nbOfMoleculas = 50000; 							//  [pJ]
  double pulseInterval = 1.;								//  [ms]
  double diffusionCoefficient = 1;							//  [nm^2/ns]

  CommandLine cmd;
  cmd.AddValue("nodeDistance", "nodeDistance", nodeDistance);
  cmd.AddValue("nbOfMoleculas", "nbOfMoleculas", nbOfMoleculas);
  cmd.AddValue("diffusionCoefficient", "diffusionCoefficient", diffusionCoefficient);
  cmd.AddValue("pulseInterval", "pulseInterval", pulseInterval);
  cmd.Parse(argc, argv);


  Time::SetResolution(Time::NS);

  // Create P1906 Helper
  P1906Helper helper;
  helper.EnableLogComponents ();

  // Create nodes (typical operation of ns-3)
  NodeContainer n;
  NetDeviceContainer d;
  n.Create (2);

  // Create a medium and the Motion component
  Ptr<P1906Medium> medium = CreateObject<P1906Medium> ();
  Ptr<P1906MOLMotion> motion = CreateObject<P1906MOLMotion> ();
  motion->SetDiffusionCoefficient (diffusionCoefficient);
  medium->SetP1906Motion (motion);

  // Create Device 1 and related components/entities
  Ptr<P1906NetDevice> dev1 = CreateObject<P1906NetDevice> ();
  Ptr<P1906MOLCommunicationInterface> c1 = CreateObject<P1906MOLCommunicationInterface> ();
  Ptr<P1906MOLSpecificity> s1 = CreateObject<P1906MOLSpecificity> ();
  Ptr<P1906MOLField> fi1 = CreateObject<P1906MOLField> ();
  Ptr<P1906MOLPerturbation> p1 = CreateObject<P1906MOLPerturbation> ();
  p1->SetPulseInterval (MilliSeconds(pulseInterval));
  p1->SetMolecules (nbOfMoleculas);
  s1->SetDiffusionCoefficient (diffusionCoefficient);

  // Create Device 2 and related components/entities
  Ptr<P1906NetDevice> dev2 = CreateObject<P1906NetDevice> ();
  Ptr<P1906MOLCommunicationInterface> c2 = CreateObject<P1906MOLCommunicationInterface> ();
  Ptr<P1906MOLSpecificity> s2 = CreateObject<P1906MOLSpecificity> ();
  Ptr<P1906MOLField> fi2 = CreateObject<P1906MOLField> ();
  Ptr<P1906MOLPerturbation> p2 = CreateObject<P1906MOLPerturbation> ();
  p2->SetPulseInterval (MilliSeconds(pulseInterval));
  p2->SetMolecules (nbOfMoleculas);
  s2->SetDiffusionCoefficient (diffusionCoefficient);


  //set devices positions
  Ptr<ListPositionAllocator> positionAlloc =
		  CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector(0, 0, 0));
  positionAlloc->Add (Vector(nodeDistance, 0, 0));
  MobilityHelper mobility;
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator(positionAlloc);
  mobility.Install(n);


  // Connect devices, nodes, medium, components and entities
  d.Add (dev1);
  d.Add (dev2);
  helper.Connect(n.Get (0),dev1,medium,c1,fi1, p1,s1);
  helper.Connect(n.Get (1),dev2,medium,c2,fi2, p2,s2);


  // Create a message to sent into the network
  int pktSize = 1; //bytes
  uint8_t *buffer  = new uint8_t[pktSize];
  for (int i = 0; i < pktSize; i++)
    {
	  buffer[i] = 0; //empty information
    }
  Ptr<Packet> message = Create<Packet>(buffer, pktSize);


  c1->HandleTransmission (message);


  Simulator::Stop (Seconds (0.01));
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}
