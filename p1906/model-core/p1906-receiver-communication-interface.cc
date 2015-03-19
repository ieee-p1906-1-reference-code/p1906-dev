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

#include "p1906-receiver-communication-interface.h"
#include "p1906-net-device.h"
#include <ns3/packet.h>
#include "p1906-specificity.h"
#include "p1906-message-carrier.h"
#include "p1906-communication-interface.h"
#include "p1906-medium.h"
#include "p1906-net-device.h"
#include "p1906-motion.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906ReceiverCommunicationInterface");

TypeId P1906ReceiverCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906ReceiverCommunicationInterface")
    .SetParent<Object> ();
  return tid;
}

P1906ReceiverCommunicationInterface::P1906ReceiverCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  m_specificity = 0;
}

P1906ReceiverCommunicationInterface::~P1906ReceiverCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  m_specificity = 0;
}

void
P1906ReceiverCommunicationInterface::SetP1906Specificity (Ptr<P1906Specificity> s)
{
  NS_LOG_FUNCTION (this);
  m_specificity = s;
}

Ptr<P1906Specificity>
P1906ReceiverCommunicationInterface::GetP1906Specificity ()
{
  NS_LOG_FUNCTION (this);
  return m_specificity;
}

void
P1906ReceiverCommunicationInterface::HandleReception (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{
  NS_LOG_FUNCTION (this);

  /*
   * In this part of the code, the received should use the
   * Specificity component to realize if the message can be
   * received or not.
   */

  bool isRxOk = GetP1906Specificity ()->CheckRxCompatibility (src, dst, message);
  if (isRxOk)
    {
	  //elaborate the message carrier
	  Ptr<Packet> p = message->GetMessage ();
	  GetP1906CommunicationInterface ()->HandleReception (p);
    }
  else
    {
	  //ignore the received message carrier
    }

}

void
P1906ReceiverCommunicationInterface::SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i)
{
  NS_LOG_FUNCTION (this);
  m_p1906CommunicationInterface = i;
}

Ptr<P1906CommunicationInterface>
P1906ReceiverCommunicationInterface::GetP1906CommunicationInterface (void)
{
  NS_LOG_FUNCTION (this);
  return m_p1906CommunicationInterface;
}

void
P1906ReceiverCommunicationInterface::SetP1906NetDevice (Ptr<P1906NetDevice> d)
{
  NS_LOG_FUNCTION (this);
  m_dev = d;
}

Ptr<P1906NetDevice>
P1906ReceiverCommunicationInterface::GetP1906NetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_dev;
}

void
P1906ReceiverCommunicationInterface::SetP1906Medium (Ptr<P1906Medium> m)
{
  NS_LOG_FUNCTION (this);
  m_medium = m;
}

Ptr<P1906Medium>
P1906ReceiverCommunicationInterface::GetP1906Medium ()
{
  NS_LOG_FUNCTION (this);
  return m_medium;
}

} // namespace ns3
