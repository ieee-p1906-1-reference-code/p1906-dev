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


#ifndef P1906_MEDIUM_H
#define P1906_MEDIUM_H

#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/packet.h"


namespace ns3 {
class P1906CommunicationInterface;
class P1906MessageCarrier;
class P1906Field;
class P1906Motion;


/**
 * \ingroup P1906 framework
 *
 * \class P1906Medium
 *
 * \brief This class implements the medium of the P1906 framework.
 */
class P1906Medium : public Channel
{
public:

  P1906Medium ();
  virtual ~P1906Medium ();

  static TypeId GetTypeId ();

  // Methods inherrited from base class
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * \param xxx add parameters
   * The metod is is charge of delivering the message to the destination node
   */
  void HandleTransmission  (Ptr<P1906CommunicationInterface> src,
		                    Ptr<P1906MessageCarrier> message,
		                    Ptr<P1906Field> field);

  /**
   * \param xxx add parameters
   * The metod is is charge of delivering the message to the destination node
   */
  void HandleReception  (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message);

  /**
   * \param xxx add parameters
   *
   * Adds the communication interface to the list of potential receivers
   */
  void AddP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i);

  void SetP1906Motion (Ptr<P1906Motion> f);
  Ptr<P1906Motion> GetP1906Motion ();

  typedef std::vector< Ptr<P1906CommunicationInterface> > P1906CommunicationInterfaces;

private:
  P1906CommunicationInterfaces* m_communicationInterfaces;
  Ptr<P1906Motion> m_motion;

protected:
  virtual void DoDispose ();
};

}

#endif /* P1906_MEDIUM_H */
