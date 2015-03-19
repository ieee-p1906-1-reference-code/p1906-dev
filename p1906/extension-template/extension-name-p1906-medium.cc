/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
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
#include "extension-name-p1906-medium.h"
#include "extension-name-p1906-communication-interface.h"
#include "extension-name-p1906-field.h"
#include "extension-name-p1906-message-carrier.h"
#include "extension-name-p1906-receiver-communication-interface.h"
#include "extension-name-p1906-specificity.h"
#include "extension-name-p1906-motion.h"


NS_LOG_COMPONENT_DEFINE ("ExtensionNameP1906Medium");
namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ExtensionNameP1906Medium);

TypeId
ExtensionNameP1906Medium::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ExtensionNameP1906Medium")
    .SetParent<P1906Medium> ()
    .AddConstructor<ExtensionNameP1906Medium> ();

  return tid;
}

ExtensionNameP1906Medium::ExtensionNameP1906Medium ()
  : P1906Medium ()
{
  NS_LOG_FUNCTION (this);
  P1906Medium::P1906CommunicationInterfaces* interfaces = new P1906Medium::P1906CommunicationInterfaces ();
  SetP1906CommunicationInterfaces (interfaces);
  SetP1906Motion (0);
}

ExtensionNameP1906Medium::~ExtensionNameP1906Medium ()
{
  NS_LOG_FUNCTION (this);
  SetP1906CommunicationInterfaces (0);
  SetP1906Motion (0);
}

void
ExtensionNameP1906Medium::DoDispose ()
{
  Channel::DoDispose ();
  SetP1906CommunicationInterfaces (0);
  SetP1906Motion (0);
  NS_LOG_FUNCTION (this);
}

void
ExtensionNameP1906Medium::HandleTransmission (Ptr<P1906CommunicationInterface> src,
                                 Ptr<P1906MessageCarrier> message,
                                 Ptr<P1906Field> field)
{
  NS_LOG_FUNCTION (this);

  std::vector< Ptr<P1906CommunicationInterface> >::iterator it;
  for (it = GetP1906CommunicationInterfaces ()->begin (); it != GetP1906CommunicationInterfaces ()->end (); it++)
    {
	  Ptr<P1906CommunicationInterface> dst = *it;
	  if (dst != src)
	    {
          Ptr<P1906MessageCarrier> receivedMessageCarrier;
          double delay;

          if (GetP1906Motion ())
            {
        	   delay = GetP1906Motion ()->ComputePropagationDelay (src, dst, message, field);
               receivedMessageCarrier = GetP1906Motion ()->CalculateReceivedMessageCarrier(src, dst, message, field);
            }
          else
            {
              NS_LOG_FUNCTION (this << "The Motion component has not been configured");
              receivedMessageCarrier = message;
              delay = 0.;
            }

          Simulator::Schedule(Seconds (delay), &ExtensionNameP1906Medium::HandleReception, this, src, dst, receivedMessageCarrier);
	    }
    }
}

void
ExtensionNameP1906Medium::HandleReception (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906ReceiverCommunicationInterface> rx = dst->GetP1906ReceiverCommunicationInterface ();
  rx->HandleReception (src, dst, message);
}



} // namespace ns3
