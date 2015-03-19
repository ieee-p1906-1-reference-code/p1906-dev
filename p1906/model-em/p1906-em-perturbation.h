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



#ifndef P1906_EM_PERTURBATION
#define P1906_EM_PERTURBATION

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-perturbation.h"

namespace ns3 {

/**
 * \ingroup P1906 framework
 *
 * \class P1906EMPerturbation
 *
 * \brief Base class implementing the Perturbation component of
 * the P1906 framework
 */

class P1906EMPerturbation : public P1906Perturbation
{
public:
  static TypeId GetTypeId (void);

  P1906EMPerturbation ();
  virtual ~P1906EMPerturbation ();

  virtual Ptr<P1906MessageCarrier> CreateMessageCarrier (Ptr<Packet> p);

  void SetPowerTransmission (double ptx);
  double GetPowerTransmission (void);
  void SetPulseDuration (Time t);
  Time GetPulseDuration (void);
  void SetPulseInterval (Time t);
  Time GetPulseInterval (void);

  void SetCentralFrequency (double f);
  double GetCentralFrequency (void);
  void SetBandwidth (double b);
  double GetBandwidth (void);
  void SetSubChannel (double c);
  double GetSubChannel (void);


private:
  double m_powerTx;
  Time m_pulseDuration;
  Time m_pulseInterval;
  double m_centralFrequency;
  double m_bandwidth;
  double m_subChannel;
};

}

#endif /* P1906_EM_PERTURBATION */
