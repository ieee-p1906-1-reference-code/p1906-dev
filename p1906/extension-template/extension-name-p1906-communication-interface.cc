/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
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

#include "extension-name-p1906-communication-interface.h"
#include "extension-name-p1906-transmitter-communication-interface.h"
#include "extension-name-p1906-receiver-communication-interface.h"
#include <ns3/packet.h>
#include "extension-name-p1906-medium.h"
#include "extension-name-p1906-net-device.h"



namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ExtensionNameP1906CommunicationInterface");

TypeId ExtensionNameP1906CommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExtensionNameP1906CommunicationInterface")
    .SetParent<P1906CommunicationInterface> ();
  return tid;
}

ExtensionNameP1906CommunicationInterface::ExtensionNameP1906CommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Medium (0);
  Ptr<ExtensionNameP1906TransmitterCommunicationInterface> tx = CreateObject<ExtensionNameP1906TransmitterCommunicationInterface> ();
  Ptr<ExtensionNameP1906ReceiverCommunicationInterface> rx = CreateObject<ExtensionNameP1906ReceiverCommunicationInterface> ();
  tx->SetP1906CommunicationInterface (this);
  rx->SetP1906CommunicationInterface (this);
  SetP1906TransmitterCommunicationInterface (tx);
  SetP1906ReceiverCommunicationInterface (rx);

}

ExtensionNameP1906CommunicationInterface::~ExtensionNameP1906CommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906Medium (0);
  SetP1906TransmitterCommunicationInterface (0);
  SetP1906ReceiverCommunicationInterface (0);
  SetP1906Medium (0);
}

bool
ExtensionNameP1906CommunicationInterface::HandleTransmission (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << "Transmitting a packet [id,size]" << p->GetUid() << p->GetSize ());
  return GetP1906TransmitterCommunicationInterface ()->HandleTransmission (p);
}

void ExtensionNameP1906CommunicationInterface::HandleReception (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << "Receiving a packet [id,size]" << p->GetUid() << p->GetSize ());
  //XXX: forward the message to upper layers
}


} // namespace ns3
