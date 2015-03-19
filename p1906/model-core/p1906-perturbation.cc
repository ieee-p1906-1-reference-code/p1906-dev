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
#include "p1906-perturbation.h"
#include "p1906-message-carrier.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906Perturbation");

TypeId P1906Perturbation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906Perturbation")
    .SetParent<Object> ();
  return tid;
}

P1906Perturbation::P1906Perturbation ()
{
  NS_LOG_FUNCTION (this);
}

P1906Perturbation::~P1906Perturbation ()
{
  NS_LOG_FUNCTION (this);
}

Ptr<P1906MessageCarrier>
P1906Perturbation::CreateMessageCarrier (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);

  Ptr<P1906MessageCarrier> carrier = CreateObject<P1906MessageCarrier> ();
  carrier->SetMessage (p);
  return carrier;
}

} // namespace ns3
