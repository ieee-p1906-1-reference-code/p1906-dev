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


#ifndef P1906_EM_MESSAGE_CARRIER
#define P1906_EM_MESSAGE_CARRIER

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-message-carrier.h"
#include <ns3/spectrum-value.h>


namespace ns3 {

class SpectrumValue;

/**
 * \ingroup P1906 framework
 *
 * \class P1906MessageCarrier
 *
 * \brief Base class implementing the Message Carrier component of
 * the P1906 framework
 */

class P1906EMMessageCarrier : public P1906MessageCarrier
{
public:
  static TypeId GetTypeId (void);

  P1906EMMessageCarrier ();
  virtual ~P1906EMMessageCarrier ();

  void SetSpectrumValue (Ptr<SpectrumValue>);
  Ptr<SpectrumValue> GetSpectrumValue (void);

  void SetDuration (Time t);
  Time GetDuration (void);
  void SetPulseDuration (Time t);
  Time GetPulseDuration (void);
  void SetPulseInterval (Time t);
  Time GetPulseInterval (void);
  void SetStartTime (Time t);
  Time GetStartTime (void);

  void SetCentralFrequency (double f);
  double GetCentralFrequency (void);
  void SetBandwidth (double b);
  double GetBandwidth (void);
  void SetSubChannel (double c);
  double GetSubChannel (void);

private:
  Ptr<SpectrumValue> m_spectrumValue;
  Time m_duration;
  Time m_pulseDuration;
  Time m_pulseInterval;
  Time m_startTime;
  double m_centralFrequency;
  double m_bandwidth;
  double m_subChannel;
};

}

#endif /* P1906_EM_MESSAGE_CARRIER */
