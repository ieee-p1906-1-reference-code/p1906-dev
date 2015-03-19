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


#ifndef P1906_TRANSMITTER_COMMUNICATION_INTERFACE
#define P1906_TRANSMITTER_COMMUNICATION_INTERFACE

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "p1906-communication-interface.h"

namespace ns3 {

class Packet;
class P1906Perturbation;
class P1906CommunicationInterface;
class P1906Field;
class P1906Force;
class P1906Medium;
class P1906NetDevice;


/**
 * \ingroup P1906 framework
 *
 * \class P1906TransmitterCommunicationInterface
 *
 * \brief Base class implementing the Transmitter entity in
 * the P1906 framework
 */

class P1906TransmitterCommunicationInterface : public Object
{
public:
  static TypeId GetTypeId (void);

  P1906TransmitterCommunicationInterface ();
  virtual ~P1906TransmitterCommunicationInterface();

  virtual bool HandleTransmission (Ptr<Packet> p);

  void SetP1906Perturbation (Ptr<P1906Perturbation> p);
  Ptr<P1906Perturbation> GetP1906Perturbation ();

  void SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i);
  Ptr<P1906CommunicationInterface> GetP1906CommunicationInterface (void);

  void SetP1906Field (Ptr<P1906Field> f);
  Ptr<P1906Field> GetP1906Field ();

  void SetP1906NetDevice (Ptr<P1906NetDevice> d);
  Ptr<P1906NetDevice> GetP1906NetDevice ();

  void SetP1906Medium (Ptr<P1906Medium> m);
  Ptr<P1906Medium> GetP1906Medium ();


private:
  Ptr<P1906Perturbation> m_perturbation;
  Ptr<P1906Field> m_field;
  Ptr<P1906CommunicationInterface> m_p1906CommunicationInterface;
  Ptr<P1906NetDevice> m_dev;
  Ptr<P1906Medium> m_medium;
};

}

#endif /* P1906_TRANSMITTER_COMMUNICATION_INTERFACE */
