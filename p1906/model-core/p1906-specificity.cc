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

#include "p1906-specificity.h"
#include "p1906-motion.h"
#include "p1906-communication-interface.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906Specificity");

TypeId P1906Specificity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906Specificity")
    .SetParent<Object> ();
  return tid;
}

P1906Specificity::P1906Specificity ()
{
  NS_LOG_FUNCTION (this << "Created default Specificity Component");
}

P1906Specificity::~P1906Specificity ()
{
  NS_LOG_FUNCTION (this);
}

bool
P1906Specificity::CheckRxCompatibility (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{
  NS_LOG_FUNCTION (this << "Default behavior: compatibility ok");
  return true;
}


void
P1906Specificity::SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i)
{
  NS_LOG_FUNCTION (this);
  m_p1906CommunicationInterface = i;
}

Ptr<P1906CommunicationInterface>
P1906Specificity::GetP1906CommunicationInterface (void)
{
  NS_LOG_FUNCTION (this);
  return m_p1906CommunicationInterface;
}
} // namespace ns3
