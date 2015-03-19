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
#include "p1906-mol-perturbation.h"
#include "ns3/p1906-message-carrier.h"
#include "ns3/p1906-perturbation.h"
#include "p1906-mol-message-carrier.h"
#include "ns3/simulator.h"
#include "ns3/spectrum-value.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOLPerturbation");

TypeId P1906MOLPerturbation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOLPerturbation")
    .SetParent<P1906Perturbation> ();
  return tid;
}

P1906MOLPerturbation::P1906MOLPerturbation ()
{
  NS_LOG_FUNCTION (this);
}

P1906MOLPerturbation::~P1906MOLPerturbation ()
{
  NS_LOG_FUNCTION (this);
}

void
P1906MOLPerturbation::SetPulseInterval (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseInterval = t;
}

Time
P1906MOLPerturbation::GetPulseInterval (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseInterval;
}

void
P1906MOLPerturbation::SetMolecules (double q)
{
  NS_LOG_FUNCTION (this << q);
  m_molecules = q;
}

double
P1906MOLPerturbation::GetMolecules (void)
{
  NS_LOG_FUNCTION (this);
  return m_molecules;
}


Ptr<P1906MessageCarrier>
P1906MOLPerturbation::CreateMessageCarrier (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906MOLMessageCarrier> carrier = CreateObject<P1906MOLMessageCarrier> ();

  double duration = m_pulseInterval.GetSeconds () * p->GetSize () * 8;
  double now = Simulator::Now ().GetSeconds ();

  NS_LOG_FUNCTION (this << "[t,bits,pulseI,duration]" << now << p->GetSize() * 8
		  << m_pulseInterval << duration);


  carrier->SetPulseInterval (m_pulseInterval);
  carrier->SetDuration (Seconds(duration));
  carrier->SetStartTime (Simulator::Now ());
  carrier->SetMolecules (GetMolecules ());
  carrier->SetMessage (p);

  return carrier;
}

} // namespace ns3
