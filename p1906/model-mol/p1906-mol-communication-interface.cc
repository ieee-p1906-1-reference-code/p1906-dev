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

#include "p1906-mol-communication-interface.h"
#include "p1906-mol-transmitter-communication-interface.h"
#include "p1906-mol-receiver-communication-interface.h"
#include <ns3/packet.h>
#include "ns3/p1906-medium.h"
#include "ns3/p1906-net-device.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOLCommunicationInterface");

TypeId P1906MOLCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOLCommunicationInterface")
    .SetParent<P1906CommunicationInterface> ();
  return tid;
}

P1906MOLCommunicationInterface::P1906MOLCommunicationInterface ()
{
  NS_LOG_FUNCTION (this );
  SetP1906NetDevice (0);
  SetP1906Medium (0);

  Ptr<P1906MOLTransmitterCommunicationInterface> tx = CreateObject<P1906MOLTransmitterCommunicationInterface> ();
  Ptr<P1906MOLReceiverCommunicationInterface> rx = CreateObject<P1906MOLReceiverCommunicationInterface> ();

  SetP1906TransmitterCommunicationInterface (tx);
  SetP1906ReceiverCommunicationInterface (rx);
  tx->SetP1906CommunicationInterface (this);
  rx->SetP1906CommunicationInterface (this);
}

P1906MOLCommunicationInterface::~P1906MOLCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Medium (0);
  SetP1906TransmitterCommunicationInterface (0);
  SetP1906ReceiverCommunicationInterface (0);
}

} // namespace ns3
