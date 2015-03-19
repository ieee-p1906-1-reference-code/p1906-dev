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


#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "p1906-medium.h"
#include "p1906-communication-interface.h"
#include "p1906-field.h"
#include "p1906-message-carrier.h"
#include "p1906-receiver-communication-interface.h"
#include "p1906-specificity.h"
#include "p1906-motion.h"


NS_LOG_COMPONENT_DEFINE ("P1906Medium");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (P1906Medium);

TypeId
P1906Medium::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::P1906Medium")
    .SetParent<Channel> ()
    .AddConstructor<P1906Medium> ();

  return tid;
}

P1906Medium::P1906Medium ()
  : Channel ()
{
  NS_LOG_FUNCTION (this);
  m_communicationInterfaces = new P1906CommunicationInterfaces ();
  m_motion = 0;
}

P1906Medium::~P1906Medium ()
{
  NS_LOG_FUNCTION (this);
  m_communicationInterfaces = 0;
  m_motion = 0;
}

void
P1906Medium::DoDispose ()
{
  Channel::DoDispose ();
  m_communicationInterfaces = 0;
  m_motion = 0;
  NS_LOG_FUNCTION (this);
}

uint32_t
P1906Medium::GetNDevices () const
{
  NS_LOG_FUNCTION (this);
  return 0;//NOT USED IN P1906
}

Ptr<NetDevice>
P1906Medium::GetDevice (uint32_t i) const
{
  NS_LOG_FUNCTION (this);
  return 0; //NOT USED IN P1906
}

void
P1906Medium::SetP1906Motion (Ptr<P1906Motion> f)
{
  NS_LOG_FUNCTION (this);
  m_motion = f;
}

Ptr<P1906Motion>
P1906Medium::GetP1906Motion ()
{
  NS_LOG_FUNCTION (this);
  return m_motion;
}

void
P1906Medium::HandleTransmission (Ptr<P1906CommunicationInterface> src,
                                 Ptr<P1906MessageCarrier> message,
                                 Ptr<P1906Field> field)
{
  NS_LOG_FUNCTION (this);

  std::vector< Ptr<P1906CommunicationInterface> >::iterator it;
  for (it = m_communicationInterfaces->begin (); it != m_communicationInterfaces->end (); it++)
    {
	  Ptr<P1906CommunicationInterface> dst = *it;
	  if (dst != src)
	    {
          Ptr<P1906MessageCarrier> receivedMessageCarrier;
          double delay;

          if (m_motion)
            {
        	   delay = m_motion->ComputePropagationDelay (src, dst, message, field);
               receivedMessageCarrier = m_motion->CalculateReceivedMessageCarrier(src, dst, message, field);
            }
          else
            {
              NS_LOG_FUNCTION (this << "The Motion component has not been configured");
              receivedMessageCarrier = message;
              delay = 0.;
            }

          Simulator::Schedule(Seconds (delay), &P1906Medium::HandleReception, this, src, dst, receivedMessageCarrier);
	    }
    }
}

void
P1906Medium::HandleReception (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906ReceiverCommunicationInterface> rx = dst->GetP1906ReceiverCommunicationInterface ();
  rx->HandleReception (src, dst, message);
}

void
P1906Medium::AddP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i)
{
  NS_LOG_FUNCTION (this);
  m_communicationInterfaces->push_back (i);
}

void
P1906Medium::SetP1906CommunicationInterfaces (P1906CommunicationInterfaces* i)
{
  NS_LOG_FUNCTION (this);
  m_communicationInterfaces = i;
}

P1906Medium::P1906CommunicationInterfaces*
P1906Medium::GetP1906CommunicationInterfaces ()
{
  NS_LOG_FUNCTION (this);
  return m_communicationInterfaces;
}



} // namespace ns3
