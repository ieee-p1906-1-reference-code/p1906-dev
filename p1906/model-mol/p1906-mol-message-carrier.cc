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
#include "ns3/packet.h"
#include "p1906-mol-message-carrier.h"
#include "ns3/p1906-message-carrier.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOLMessageCarrier");

TypeId P1906MOLMessageCarrier::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOLMessageCarrier")
    .SetParent<P1906MessageCarrier> ();
  return tid;
}

P1906MOLMessageCarrier::P1906MOLMessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  SetMessage (0);
}

P1906MOLMessageCarrier::~P1906MOLMessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  SetMessage (0);
}

void
P1906MOLMessageCarrier::SetDuration (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_duration = t;
}

Time
P1906MOLMessageCarrier::GetDuration (void)
{
  NS_LOG_FUNCTION (this);
  return m_duration;
}

void
P1906MOLMessageCarrier::SetPulseInterval (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseInterval = t;
}

Time
P1906MOLMessageCarrier::GetPulseInterval (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseInterval;
}

void
P1906MOLMessageCarrier::SetStartTime (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_startTime = t;
}

Time
P1906MOLMessageCarrier::GetStartTime (void)
{
  NS_LOG_FUNCTION (this);
  return m_startTime;
}

void
P1906MOLMessageCarrier::SetMolecules (double q)
{
  NS_LOG_FUNCTION (this << q);
  m_molecules = q;
}

double
P1906MOLMessageCarrier::GetMolecules (void)
{
  NS_LOG_FUNCTION (this);
  return m_molecules;
}

} // namespace ns3
