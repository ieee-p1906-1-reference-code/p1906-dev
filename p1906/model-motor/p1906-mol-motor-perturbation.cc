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
#include "ns3/packet.h"
#include "ns3/p1906-perturbation.h"
#include "ns3/p1906-message-carrier.h"
#include "ns3/p1906-mol-motor-perturbation.h"
#include "ns3/p1906-mol-message-carrier.h"
#include "ns3/p1906-mol-motor.h"
#include "ns3/simulator.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTOR_Perturbation");

TypeId P1906MOL_MOTOR_Perturbation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTOR_Perturbation")
    .SetParent<P1906Perturbation> ();
  return tid;
}

P1906MOL_MOTOR_Perturbation::P1906MOL_MOTOR_Perturbation ()
{
  NS_LOG_FUNCTION (this);
}

P1906MOL_MOTOR_Perturbation::~P1906MOL_MOTOR_Perturbation ()
{
  NS_LOG_FUNCTION (this);
}

void
P1906MOL_MOTOR_Perturbation::SetPulseInterval (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseInterval = t;
}

Time
P1906MOL_MOTOR_Perturbation::GetPulseInterval (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseInterval;
}

void
P1906MOL_MOTOR_Perturbation::SetMolecules (double q)
{
  NS_LOG_FUNCTION (this << q);
  m_molecules = q;
}

double
P1906MOL_MOTOR_Perturbation::GetMolecules (void)
{
  NS_LOG_FUNCTION (this);
  return m_molecules;
}

//! required for use IEEE 1906 core; this is where the user-defined Message Carrier is created
Ptr<P1906MessageCarrier>
P1906MOL_MOTOR_Perturbation::CreateMessageCarrier (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906MOL_Motor> carrier = CreateObject<P1906MOL_Motor> ();

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
