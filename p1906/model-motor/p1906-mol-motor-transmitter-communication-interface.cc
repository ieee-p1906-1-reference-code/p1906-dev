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
#include "p1906-mol-motor-transmitter-communication-interface.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_TransmitterCommunicationInterface");

TypeId P1906MOL_MOTOR_TransmitterCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_TransmitterCommunicationInterface")
    .SetParent<P1906TransmitterCommunicationInterface> ();
  return tid;
}

P1906MOL_MOTOR_TransmitterCommunicationInterface::P1906MOL_MOTOR_TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
}

P1906MOL_MOTOR_TransmitterCommunicationInterface::~P1906MOL_MOTOR_TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3
