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
 * this file models the simple example of EM-based communication.
 * xxx: add more comments (i.e., description of the scenario)
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/p1906-helper.h"
#include "ns3/p1906-net-device.h"
#include "ns3/p1906-em-perturbation.h"
#include "ns3/p1906-em-field.h"
#include "ns3/p1906-em-motion.h"
#include "ns3/p1906-em-specificity.h"
#include "ns3/p1906-medium.h"
#include "ns3/p1906-em-communication-interface.h"
#include "ns3/p1906-em-transmitter-communication-interface.h"
#include "ns3/p1906-em-receiver-communication-interface.h"

using namespace ns3;


int main (int argc, char *argv[])
{	

  //set of parameters
  double waveSpeed = 3e8; 								//  [m/s]
  double nodeDistance = 0.001; 							//  [m]
  double pulseEnergy = 500; 							//  [pJ]
  double pulseDuration = 100;							//  [fs]
  double pulseInterval = 100.;							//  [ps]
  double powerTx = pulseEnergy/(pulseDuration/1000.);	//  [W]

  CommandLine cmd;
  cmd.AddValue("nodeDistance", "nodeDistance", nodeDistance);
  cmd.AddValue("pulseEnergy", "pulseEnergy", pulseEnergy);
  cmd.AddValue("pulseDuration", "pulseDuration", pulseDuration);
  cmd.AddValue("pulseInterval", "pulseInterval", pulseInterval);
  cmd.Parse(argc, argv);


  /*
   * From Ke Yang contribution:
   * For the subchannel of 0.1 THz, the central frequency will be 0.5THz,
   * 0.6THz, 0.7THz to 1.5 THz; and the overall bandwidth will be 0.45THz
   * to 1.55THz.
   */
  double centralFrequency = 1e12 * (0.45 + (1.55 - 0.45)/2.);	//  [Hz]
  double bandwidth = 1e12 * (1.55 - 0.45);						//  [Hz]
  double subChannel = 1e12 * 0.1; 								//  [Hz]


  Time::SetResolution(Time::FS);

  // Create P1906 Helper
  P1906Helper helper;
  helper.EnableLogComponents ();

  // Create nodes (typical operation of ns-3)
  NodeContainer n;
  NetDeviceContainer d;
  n.Create (2);

  // Create a medium and the Motion component
  Ptr<P1906Medium> medium = CreateObject<P1906Medium> ();
  Ptr<P1906EMMotion> motion = CreateObject<P1906EMMotion> ();
  motion->SetWaveSpeed (waveSpeed);
  medium->SetP1906Motion (motion);

  // Create Device 1 and related components/entities
  Ptr<P1906NetDevice> dev1 = CreateObject<P1906NetDevice> ();
  Ptr<P1906EMCommunicationInterface> c1 = CreateObject<P1906EMCommunicationInterface> ();
  Ptr<P1906EMSpecificity> s1 = CreateObject<P1906EMSpecificity> ();
  Ptr<P1906EMField> fi1 = CreateObject<P1906EMField> ();
  Ptr<P1906EMPerturbation> p1 = CreateObject<P1906EMPerturbation> ();
  p1->SetBandwidth (bandwidth);
  p1->SetCentralFrequency (centralFrequency);
  p1->SetSubChannel (subChannel);
  p1->SetPowerTransmission (powerTx);
  p1->SetPulseDuration (FemtoSeconds (pulseDuration));
  p1->SetPulseInterval (PicoSeconds(pulseInterval));

  // Create Device 2 and related components/entities
  Ptr<P1906NetDevice> dev2 = CreateObject<P1906NetDevice> ();
  Ptr<P1906EMCommunicationInterface> c2 = CreateObject<P1906EMCommunicationInterface> ();
  Ptr<P1906EMSpecificity> s2 = CreateObject<P1906EMSpecificity> ();
  Ptr<P1906EMField> fi2 = CreateObject<P1906EMField> ();
  Ptr<P1906EMPerturbation> p2 = CreateObject<P1906EMPerturbation> ();
  p2->SetBandwidth (bandwidth);
  p2->SetCentralFrequency (centralFrequency);
  p2->SetSubChannel (subChannel);
  p2->SetPowerTransmission (powerTx);
  p2->SetPulseDuration (FemtoSeconds (pulseDuration));
  p2->SetPulseInterval (PicoSeconds(pulseInterval));



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
