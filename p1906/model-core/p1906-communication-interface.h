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


#ifndef P1906_COMMUNICATION_INTERFACE
#define P1906_COMMUNICATION_INTERFACE

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

namespace ns3 {

class P1906Medium;
class P1906NetDevice;
class P1906ReceiverCommunicationInterface;
class P1906TransmitterCommunicationInterface;
class Packet;


/**
 * \ingroup P1906 framework
 *
 * \class P1906CommunicationInterface
 *
 * \brief Base class implementing a communication interface, which
 * is a container of the transmitter and the receiver entities.
 */

class P1906CommunicationInterface : public Object
{
public:
  static TypeId GetTypeId (void);

  P1906CommunicationInterface ();
  virtual ~P1906CommunicationInterface ();

  void SetP1906TransmitterCommunicationInterface (Ptr<P1906TransmitterCommunicationInterface> tx);
  Ptr<P1906TransmitterCommunicationInterface> GetP1906TransmitterCommunicationInterface ();

  void SetP1906ReceiverCommunicationInterface (Ptr<P1906ReceiverCommunicationInterface> rx);
  Ptr<P1906ReceiverCommunicationInterface> GetP1906ReceiverCommunicationInterface ();

  void SetP1906NetDevice (Ptr<P1906NetDevice> d);
  Ptr<P1906NetDevice> GetP1906NetDevice ();

  void SetP1906Medium (Ptr<P1906Medium> m);
  Ptr<P1906Medium> GetP1906Medium ();

  bool HandleTransmission (Ptr<Packet> p);
  void HandleReception (Ptr<Packet> p);

private:
  Ptr<P1906NetDevice> m_dev;
  Ptr<P1906TransmitterCommunicationInterface> m_tx;
  Ptr<P1906ReceiverCommunicationInterface> m_rx;
  Ptr<P1906Medium> m_medium;
};

}

#endif /* P1906_COMMUNICATION_INTERFACE */
