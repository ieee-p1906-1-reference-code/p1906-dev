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
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "ns3/seq-ts-header.h"
#include "ns3/simulator.h"
#include "p1906-net-device.h"
#include "p1906-communication-interface.h"


NS_LOG_COMPONENT_DEFINE ("P1906NetDevice");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (P1906NetDevice);

TypeId
P1906NetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906NetDevice")
    .SetParent<NetDevice> ()
    .AddConstructor<P1906NetDevice> ()
  ;
  return tid;
}

P1906NetDevice::P1906NetDevice ()
{
  NS_LOG_FUNCTION (this);
  m_p1906CommunicationInterface = 0;
}

P1906NetDevice::~P1906NetDevice ()
{
  NS_LOG_FUNCTION (this);
  m_p1906CommunicationInterface = 0;
}

void
P1906NetDevice::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  NetDevice::DoDispose ();
}

void
P1906NetDevice::SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i)
{
  m_p1906CommunicationInterface = i;
}

Ptr<P1906CommunicationInterface>
P1906NetDevice::GetP1906CommunicationInterface (void)
{
  return m_p1906CommunicationInterface;
}









































































void
P1906NetDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION (index);
  m_ifIndex = index;
}

uint32_t
P1906NetDevice::GetIfIndex (void) const
{
  NS_LOG_FUNCTION (this);
  return m_ifIndex;
}

bool
P1906NetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_FUNCTION (mtu);
  return (mtu == 0);
}

uint16_t
P1906NetDevice::GetMtu (void) const
{
  NS_LOG_FUNCTION (this);
  return 0;
}

Ptr<Channel>
P1906NetDevice::GetChannel (void) const
{
  NS_LOG_FUNCTION (this);
  return 0;
}

void
P1906NetDevice::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this);
}

Address
P1906NetDevice::GetAddress (void) const
{
  NS_LOG_FUNCTION (this);
  return Address ();
}

bool
P1906NetDevice::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

Address
P1906NetDevice::GetBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  return Address ();
}

bool
P1906NetDevice::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

Address
P1906NetDevice::GetMulticast (Ipv4Address addr) const
{
  NS_LOG_FUNCTION (addr);
  return Address ();
}

Address
P1906NetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (addr);
  return Address ();
}

bool
P1906NetDevice::IsPointToPoint (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
P1906NetDevice::IsBridge (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}


Ptr<Node>
P1906NetDevice::GetNode (void) const
{
  NS_LOG_FUNCTION (this);
  return m_node;
}

void
P1906NetDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (node);
  m_node = node;
}

bool
P1906NetDevice::NeedsArp (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
P1906NetDevice::IsLinkUp (void) const
{
  NS_LOG_FUNCTION (this);
  return false;
}

void
P1906NetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  NS_LOG_FUNCTION (&callback);
}

void
P1906NetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION (&cb);
}

void
P1906NetDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION (&cb);
}

bool
P1906NetDevice::SupportsSendFrom () const
{
  NS_LOG_FUNCTION (this);
  return false;
}

bool
P1906NetDevice::Send (Ptr<Packet> packet,const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  return false;
}

bool
P1906NetDevice::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << src << dest << protocolNumber);
  return false;
}


} // namespace ns3
