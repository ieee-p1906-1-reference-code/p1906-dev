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
 * Author: Giuseppe Piro - Telematics Lab Research Group
 *                         Politecnico di Bari
 *                         giuseppe.piro@poliba.it
 *                         telematics.poliba.it/piro
 */

#include "ns3/log.h"

#include "p1906-mol-motion.h"
#include "ns3/p1906-communication-interface.h"
#include "ns3/p1906-message-carrier.h"
#include "ns3/p1906-field.h"
#include "ns3/mobility-model.h"
#include "ns3/p1906-net-device.h"
#include <ns3/spectrum-value.h>
#include "p1906-mol-message-carrier.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906MOLMotion");

TypeId P1906MOLMotion::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906MOLMotion")
    .SetParent<P1906Motion> ();
  return tid;
}

P1906MOLMotion::P1906MOLMotion ()
{
  NS_LOG_FUNCTION (this);
}

P1906MOLMotion::~P1906MOLMotion ()
{
  NS_LOG_FUNCTION (this);
}


double
P1906MOLMotion::ComputePropagationDelay (Ptr<P1906CommunicationInterface> src,
		                                Ptr<P1906CommunicationInterface> dst,
		                                Ptr<P1906MessageCarrier> message,
		                                Ptr<P1906Field> field)
{

  /*
   * The Motion component implements a propagation model as presented in
   *
   * Detection Techniques for Diffusion-based Molecular Communication
   * I Llatser, A Cabellos-Aparicio, M Pierobon, E Alarcón
   * Selected Areas in Communications, IEEE Journal on 31 (12), 726-734
   *
   */

  NS_LOG_FUNCTION (this << "Fick's low");

  Ptr<MobilityModel> srcMobility = src->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  Ptr<MobilityModel> dstMobility = dst->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();


  double distance = dstMobility->GetDistanceFrom (srcMobility);
  double delay = pow(distance,2)/(GetDiffusionConefficient ()*6);

  NS_LOG_FUNCTION (this << "[dist,diffusion,delay]" << distance << GetDiffusionConefficient () << delay);

  return delay;
}


Ptr<P1906MessageCarrier>
P1906MOLMotion::CalculateReceivedMessageCarrier(Ptr<P1906CommunicationInterface> src,
		                                       Ptr<P1906CommunicationInterface> dst,
		                                       Ptr<P1906MessageCarrier> message,
		                                       Ptr<P1906Field> field)
{

  NS_LOG_FUNCTION (this << "Do nothing for the Fick's low");
  return message;
}

void
P1906MOLMotion::SetDiffusionCoefficient (double d)
{
  NS_LOG_FUNCTION (this << d);
  m_diffusionCoefficient = d;
}

double
P1906MOLMotion::GetDiffusionConefficient (void)
{
  NS_LOG_FUNCTION (this);
  return m_diffusionCoefficient;
}


} // namespace ns3
