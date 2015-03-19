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

#include "p1906-transmitter-communication-interface.h"
#include "p1906-net-device.h"
#include <ns3/packet.h>
#include "p1906-perturbation.h"
#include "p1906-field.h"
#include "p1906-message-carrier.h"
#include "p1906-communication-interface.h"
#include "p1906-medium.h"
#include "ns3/packet.h"
#include "p1906-medium.h"
#include "p1906-net-device.h"




namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906TransmitterCommunicationInterface");

TypeId P1906TransmitterCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906TransmitterCommunicationInterface")
    .SetParent<Object> ();
  return tid;
}

P1906TransmitterCommunicationInterface::P1906TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  m_perturbation = 0;
  m_field = 0;
}

P1906TransmitterCommunicationInterface::~P1906TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  m_perturbation = 0;
}

void
P1906TransmitterCommunicationInterface::SetP1906Perturbation (Ptr<P1906Perturbation> p)
{
  NS_LOG_FUNCTION (this);
  m_perturbation = p;
}

Ptr<P1906Perturbation>
P1906TransmitterCommunicationInterface::GetP1906Perturbation ()
{
  NS_LOG_FUNCTION (this);
  return m_perturbation;
}

void
P1906TransmitterCommunicationInterface::SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i)
{
  NS_LOG_FUNCTION (this);
  m_p1906CommunicationInterface = i;
}

Ptr<P1906CommunicationInterface>
P1906TransmitterCommunicationInterface::GetP1906CommunicationInterface (void)
{
  NS_LOG_FUNCTION (this);
  return m_p1906CommunicationInterface;
}

void
P1906TransmitterCommunicationInterface::SetP1906Field (Ptr<P1906Field> f)
{
  NS_LOG_FUNCTION (this);
  m_field = f;
}

Ptr<P1906Field>
P1906TransmitterCommunicationInterface::GetP1906Field ()
{
  NS_LOG_FUNCTION (this);
  return m_field;
}

bool
P1906TransmitterCommunicationInterface::HandleTransmission (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906MessageCarrier> carrier = m_perturbation->CreateMessageCarrier(p);

  GetP1906Medium ()->HandleTransmission(m_p1906CommunicationInterface,
		                                carrier,
		                                m_field);

  return true;
}

void
P1906TransmitterCommunicationInterface::SetP1906NetDevice (Ptr<P1906NetDevice> d)
{
  NS_LOG_FUNCTION (this);
  m_dev = d;
}

Ptr<P1906NetDevice>
P1906TransmitterCommunicationInterface::GetP1906NetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_dev;
}

void
P1906TransmitterCommunicationInterface::SetP1906Medium (Ptr<P1906Medium> m)
{
  NS_LOG_FUNCTION (this);
  m_medium = m;
}

Ptr<P1906Medium>
P1906TransmitterCommunicationInterface::GetP1906Medium ()
{
  NS_LOG_FUNCTION (this);
  return m_medium;
}

} // namespace ns3
