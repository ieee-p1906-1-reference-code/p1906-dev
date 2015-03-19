/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
 *
 *  This source file is an essential part of IEEE ExtensionNameP1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE ExtensionNameP1906.1 may also be made and
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


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/p1906-helper.h"
#include "ns3/extension-name-p1906-net-device.h"
#include "ns3/extension-name-p1906-perturbation.h"
#include "ns3/extension-name-p1906-field.h"
#include "ns3/extension-name-p1906-motion.h"
#include "ns3/extension-name-p1906-specificity.h"
#include "ns3/extension-name-p1906-medium.h"
#include "ns3/extension-name-p1906-communication-interface.h"
#include "ns3/extension-name-p1906-transmitter-communication-interface.h"
#include "ns3/extension-name-p1906-receiver-communication-interface.h"

using namespace ns3;


int main (int argc, char *argv[])
{	

  // Create P1906 Helper
  P1906Helper helper;
  helper.EnableLogComponents ();

  // Create nodes (typical operation of ns-3)
  NodeContainer n;
  NetDeviceContainer d;
  n.Create (2);

  // Create a medium and the Motion component
  Ptr<ExtensionNameP1906Medium> medium = CreateObject<ExtensionNameP1906Medium> ();
  Ptr<ExtensionNameP1906Motion> motion = CreateObject<ExtensionNameP1906Motion> ();
  medium->SetP1906Motion (motion);


  // Create Device 1 and related components/entities
  Ptr<ExtensionNameP1906NetDevice> dev1 = CreateObject<ExtensionNameP1906NetDevice> ();
  Ptr<ExtensionNameP1906CommunicationInterface> c1 = CreateObject<ExtensionNameP1906CommunicationInterface> ();
  Ptr<ExtensionNameP1906Specificity> s1 = CreateObject<ExtensionNameP1906Specificity> ();
  Ptr<ExtensionNameP1906Field> fi1 = CreateObject<ExtensionNameP1906Field> ();
  Ptr<ExtensionNameP1906Perturbation> p1 = CreateObject<ExtensionNameP1906Perturbation> ();

  // Create Device 2 and related components/entities
  Ptr<ExtensionNameP1906NetDevice> dev2 = CreateObject<ExtensionNameP1906NetDevice> ();
  Ptr<ExtensionNameP1906CommunicationInterface> c2 = CreateObject<ExtensionNameP1906CommunicationInterface> ();
  Ptr<ExtensionNameP1906Specificity> s2 = CreateObject<ExtensionNameP1906Specificity> ();
  Ptr<ExtensionNameP1906Field> fi2 = CreateObject<ExtensionNameP1906Field> ();
  Ptr<ExtensionNameP1906Perturbation> p2 = CreateObject<ExtensionNameP1906Perturbation> ();

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
