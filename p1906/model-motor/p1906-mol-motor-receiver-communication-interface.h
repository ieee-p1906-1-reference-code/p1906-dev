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


#ifndef P1906_MOL_MOTOR_RECEIVER_COMMUNICATION_INTERFACE
#define P1906_MOL_MOTOR_RECEIVER_COMMUNICATION_INTERFACE

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-communication-interface.h"
#include "ns3/p1906-receiver-communication-interface.h"

namespace ns3 {

class Packet;
class P1906Specificity;
class P1906MessageCarrier;
class P1906CommunicationInterface;
class P1906Medium;
class P1906NetDevice;
class P1906Motion;

/**
 * \ingroup P1906 framework
 *
 * \class P1906MOL_MOTOR_ReceiverCommunicationInterface
 *
 * \brief Base class implementing a the Receiver entity
 * of the P1906 framework for the MOLECULAR Example
 */

class P1906MOL_MOTOR_ReceiverCommunicationInterface : public P1906ReceiverCommunicationInterface
{
public:
  static TypeId GetTypeId (void);

  P1906MOL_MOTOR_ReceiverCommunicationInterface ();
  virtual ~P1906MOL_MOTOR_ReceiverCommunicationInterface();

  virtual void HandleReception (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message);

};

}

#endif /* P1906_MOL_MOTOR_RECEIVER_COMMUNICATION_INTERFACE */
