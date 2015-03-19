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
#include "extension-name-p1906-receiver-communication-interface.h"
#include "extension-name-p1906-net-device.h"
#include <ns3/packet.h>
#include "extension-name-p1906-specificity.h"
#include "extension-name-p1906-message-carrier.h"
#include "extension-name-p1906-communication-interface.h"
#include "extension-name-p1906-medium.h"
#include "extension-name-p1906-net-device.h"
#include "extension-name-p1906-motion.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ExtensionNameP1906ReceiverCommunicationInterface");

TypeId ExtensionNameP1906ReceiverCommunicationInterface::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ExtensionNameP1906ReceiverCommunicationInterface")
    .SetParent<P1906ReceiverCommunicationInterface> ();
  return tid;
}

ExtensionNameP1906ReceiverCommunicationInterface::ExtensionNameP1906ReceiverCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Specificity (0);
}

ExtensionNameP1906ReceiverCommunicationInterface::~ExtensionNameP1906ReceiverCommunicationInterface ()
{
  NS_LOG_FUNCTION (this);
  SetP1906NetDevice (0);
  SetP1906Specificity (0);
}

void
ExtensionNameP1906ReceiverCommunicationInterface::HandleReception (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message)
{
  NS_LOG_FUNCTION (this);

  /*
   * In this part of the code, the received should use the
   * Specificity component to realize if the message can be
   * received or not.
   */
  Ptr<ExtensionNameP1906Specificity> spec = GetP1906Specificity ()->GetObject<ExtensionNameP1906Specificity> ();
  bool isRxOk = spec->CheckRxCompatibility (src, dst, message);
  if (isRxOk)
    {
	  //elaborate the message carrier
	  Ptr<Packet> p = message->GetMessage ();
	  GetP1906CommunicationInterface ()->HandleReception (p);
    }
  else
    {
	  //ignore the received message carrier
    }

}

} // namespace ns3
