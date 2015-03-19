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
#include "p1906-message-carrier.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MessageCarrier");

TypeId P1906MessageCarrier::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MessageCarrier")
    .SetParent<Object> ();
  return tid;
}

P1906MessageCarrier::P1906MessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  m_message = 0;
}

P1906MessageCarrier::~P1906MessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  m_message = 0;
}

void
P1906MessageCarrier::SetMessage (Ptr<Packet> message)
{
  NS_LOG_FUNCTION (this);
  m_message = message;
}

Ptr<Packet>
P1906MessageCarrier::GetMessage ()
{
  NS_LOG_FUNCTION (this);
  return m_message;
}


} // namespace ns3
