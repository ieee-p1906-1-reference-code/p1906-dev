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

#include "ns3/log.h"

#include "extension-name-p1906-transmitter-communication-interface.h"
#include "extension-name-p1906-net-device.h"
#include <ns3/packet.h>
#include "extension-name-p1906-perturbation.h"
#include "extension-name-p1906-field.h"
#include "extension-name-p1906-message-carrier.h"
#include "extension-name-p1906-communication-interface.h"
#include "extension-name-p1906-medium.h"
#include "ns3/packet.h"
#include "extension-name-p1906-medium.h"
#include "extension-name-p1906-net-device.h"




namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ExtensionNameP1906TransmitterCommunicationInterface");

TypeId ExtensionNameP1906TransmitterCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExtensionNameP1906TransmitterCommunicationInterface")
    .SetParent<P1906TransmitterCommunicationInterface> ();
  return tid;
}

ExtensionNameP1906TransmitterCommunicationInterface::ExtensionNameP1906TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Perturbation (0);
  SetP1906Field (0);
}

ExtensionNameP1906TransmitterCommunicationInterface::~ExtensionNameP1906TransmitterCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Perturbation (0);
  SetP1906Field (0);
}

bool
ExtensionNameP1906TransmitterCommunicationInterface::HandleTransmission (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<ExtensionNameP1906Perturbation> perturbation = GetP1906Perturbation ()->GetObject<ExtensionNameP1906Perturbation> ();
  Ptr<ExtensionNameP1906MessageCarrier> carrier = perturbation->CreateMessageCarrier(p)->GetObject<ExtensionNameP1906MessageCarrier> ();

  GetP1906Medium ()->HandleTransmission(GetP1906CommunicationInterface (),
		                                carrier->GetObject<P1906MessageCarrier> (),
		                                GetP1906Field ()
		                                );

  return true;
}



} // namespace ns3
