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


#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"

#include "ns3/p1906-medium.h"

#include "ns3/p1906-mol-motor.h"
#include "ns3/p1906-mol-motor-motion.h"
#include "ns3/p1906-mol-motor-medium.h"
#include "ns3/p1906-mol-motor-field.h"
#include "ns3/p1906-mol-motor-communication-interface.h"
#include "ns3/p1906-mol-motor-receiver-communication-interface.h"
#include "ns3/p1906-mol-motor-specificity.h"

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTORMedium");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_MOTORMedium);

TypeId
P1906MOL_MOTORMedium::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTORMedium")
    .SetParent<P1906Medium> ()
    .AddConstructor<P1906MOL_MOTORMedium> ();

  return tid;
}

P1906MOL_MOTORMedium::P1906MOL_MOTORMedium ()
{
  NS_LOG_FUNCTION (this);
}

P1906MOL_MOTORMedium::~P1906MOL_MOTORMedium ()
{
  NS_LOG_FUNCTION (this);
}

void
P1906MOL_MOTORMedium::HandleTransmission (Ptr<P1906CommunicationInterface> src,
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
			   NS_LOG_FUNCTION (this << "delay: " << delay);
			   if (delay < 0)
			   {
				   NS_LOG_FUNCTION (this << "motion is not complete");
				   delay = fabs(delay);
				   Simulator::Schedule(Seconds (delay), &P1906MOL_MOTORMedium::HandleTransmission, this, src, message, field);
				   return;
			   }
			   else
			   {
				   receivedMessageCarrier = m_motion->CalculateReceivedMessageCarrier(src, dst, message, field);
			   }
            }
          else
            {
              NS_LOG_FUNCTION (this << "The Motion component has not been configured");
              receivedMessageCarrier = message;
              delay = 0.;
            }
		  NS_LOG_FUNCTION (this << "scheduling reception for " << delay);
          Simulator::Schedule(Seconds (delay), &P1906MOL_MOTORMedium::HandleReception, this, src, dst, receivedMessageCarrier);
	    }
    }
}

} // namespace ns3
