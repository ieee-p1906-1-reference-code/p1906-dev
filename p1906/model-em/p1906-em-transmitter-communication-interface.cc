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


#include "ns3/log.h"

#include "p1906-em-transmitter-communication-interface.h"
#include "ns3/p1906-net-device.h"
#include <ns3/packet.h>
#include "ns3/p1906-perturbation.h"
#include "ns3/p1906-field.h"
#include "ns3/p1906-message-carrier.h"
#include "ns3/p1906-communication-interface.h"
#include "ns3/p1906-medium.h"
#include "ns3/packet.h"
#include "ns3/p1906-medium.h"
#include "ns3/p1906-net-device.h"




namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906EMTransmitterCommunicationInterface");

TypeId P1906EMTransmitterCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906EMTransmitterCommunicationInterface")
    .SetParent<P1906TransmitterCommunicationInterface> ();
  return tid;
}

P1906EMTransmitterCommunicationInterface::P1906EMTransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
}

P1906EMTransmitterCommunicationInterface::~P1906EMTransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3
