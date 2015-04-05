/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright © 2014 by IEEE.
 *
 *  This source file is an essential part of IEEE Std 1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE Std 1906.1 may also be made and
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

#include "p1906-mol-motor-specificity.h"
#include "ns3/p1906-specificity.h"
#include "p1906-mol-motor.h"
#include "ns3/p1906-net-device.h"
#include "ns3/p1906-perturbation.h"
#include "ns3/p1906-receiver-communication-interface.h"
#include "ns3/p1906-transmitter-communication-interface.h"
#include "p1906-mol-motor-perturbation.h"
#include "ns3/mobility-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOL_MOTORSpecificity");

NS_OBJECT_ENSURE_REGISTERED (P1906MOL_MOTORSpecificity);

TypeId P1906MOL_MOTORSpecificity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOL_MOTORSpecificity")
    .SetParent<P1906Specificity> ()
	.AddConstructor<P1906MOL_MOTORSpecificity> ()
	.AddAttribute ("Sensitivity", // this should be moved to the specificity component
	            "The probability of detecting a message [probability] (only meaningful if Receiver).",
	            DoubleValue (1.0),
				MakeDoubleAccessor (&P1906MOL_MOTORSpecificity::m_sensitivity),
				MakeDoubleChecker<double_t> ())
	.AddAttribute ("Specificity", // this should be moved to the specificity component
	            "The probability of detecting a false message [probability] (only meaningful if Receiver).",
	            DoubleValue (0.0),
				MakeDoubleAccessor (&P1906MOL_MOTORSpecificity::m_specificity),
				MakeDoubleChecker<double_t> ())
	;
  return tid;
}

P1906MOL_MOTORSpecificity::P1906MOL_MOTORSpecificity ()
{
  NS_LOG_FUNCTION (this << "MOL MOTOR Specificity Component");
}

P1906MOL_MOTORSpecificity::~P1906MOL_MOTORSpecificity ()
{
  NS_LOG_FUNCTION (this);
}

bool
P1906MOL_MOTORSpecificity::CheckRxCompatibility (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{

  NS_LOG_FUNCTION (this);
  Ptr<P1906MOL_Motor> m = message->GetObject <P1906MOL_Motor>();

  double transmissionRate = 1. / m->GetPulseInterval ().GetSeconds ();

  Ptr<MobilityModel> srcMobility = src->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  Ptr<MobilityModel> dstMobility = dst->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  double distance = dstMobility->GetDistanceFrom (srcMobility);

  NS_LOG_FUNCTION (this << "[distance,txRate]" << distance << transmissionRate);

  double minPulseWidth = (0.4501/GetDiffusionConefficient ()) * pow(distance,2);
  double channelCapacity = 1. / minPulseWidth;

  NS_LOG_FUNCTION (this << "testcapacity: [distance, txRate, channelCapacity]" << distance << transmissionRate << channelCapacity);

  if (channelCapacity >= transmissionRate)
	{
	  NS_LOG_FUNCTION (this << "Fick's bound has been respected");
	  return true;
	}
  else
	{
	  NS_LOG_FUNCTION (this << "Fick's bound has NOT been respected --> transmission failed");
	  return false;
	}
}

void
P1906MOL_MOTORSpecificity::SetDiffusionCoefficient (double d)
{
  NS_LOG_FUNCTION (this << d);
  m_diffusionCoefficient = d;
}

double
P1906MOL_MOTORSpecificity::GetDiffusionConefficient (void)
{
  NS_LOG_FUNCTION (this);
  return m_diffusionCoefficient;
}

} // namespace ns3
