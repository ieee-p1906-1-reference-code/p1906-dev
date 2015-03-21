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


#include "ns3/log.h"

#include <ns3/packet.h>
#include "ns3/p1906-medium.h"
#include "ns3/p1906-net-device.h"

#include "ns3/p1906-mol-motor-communication-interface.h"
#include "ns3/p1906-mol-motor-transmitter-communication-interface.h"
#include "ns3/p1906-mol-motor-receiver-communication-interface.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_CommunicationInterface");

TypeId P1906MOL_MOTOR_CommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_CommunicationInterface")
    .SetParent<P1906CommunicationInterface> ();
  return tid;
}

P1906MOL_MOTOR_CommunicationInterface::P1906MOL_MOTOR_CommunicationInterface ()
{
  NS_LOG_FUNCTION (this );
  SetP1906NetDevice (0);
  SetP1906Medium (0);

  Ptr<P1906MOL_MOTOR_TransmitterCommunicationInterface> tx = CreateObject<P1906MOL_MOTOR_TransmitterCommunicationInterface> ();
  Ptr<P1906MOL_MOTOR_ReceiverCommunicationInterface> rx = CreateObject<P1906MOL_MOTOR_ReceiverCommunicationInterface> ();

  SetP1906TransmitterCommunicationInterface (tx);
  SetP1906ReceiverCommunicationInterface (rx);
  tx->SetP1906CommunicationInterface (this);
  rx->SetP1906CommunicationInterface (this);
}

P1906MOL_MOTOR_CommunicationInterface::~P1906MOL_MOTOR_CommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Medium (0);
  SetP1906TransmitterCommunicationInterface (0);
  SetP1906ReceiverCommunicationInterface (0);
}

} // namespace ns3
