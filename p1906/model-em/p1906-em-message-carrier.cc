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


#include "ns3/log.h"
#include "ns3/packet.h"
#include "p1906-em-message-carrier.h"
#include "ns3/p1906-message-carrier.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906EMMessageCarrier");

TypeId P1906EMMessageCarrier::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906EMMessageCarrier")
    .SetParent<P1906MessageCarrier> ();
  return tid;
}

P1906EMMessageCarrier::P1906EMMessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  SetMessage (0);
}

P1906EMMessageCarrier::~P1906EMMessageCarrier ()
{
  NS_LOG_FUNCTION (this);
  SetMessage (0);
}


void
P1906EMMessageCarrier::SetSpectrumValue (Ptr<SpectrumValue> s)
{
  NS_LOG_FUNCTION (this << s);
  m_spectrumValue = s;
}

Ptr<SpectrumValue>
P1906EMMessageCarrier::GetSpectrumValue (void)
{
  NS_LOG_FUNCTION (this);
  return m_spectrumValue;
}

void
P1906EMMessageCarrier::SetDuration (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_duration = t;
}

Time
P1906EMMessageCarrier::GetDuration (void)
{
  NS_LOG_FUNCTION (this);
  return m_duration;
}

void
P1906EMMessageCarrier::SetPulseDuration (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseDuration = t;
}

Time
P1906EMMessageCarrier::GetPulseDuration (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseDuration;
}

void
P1906EMMessageCarrier::SetPulseInterval (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseInterval = t;
}

Time
P1906EMMessageCarrier::GetPulseInterval (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseInterval;
}

void
P1906EMMessageCarrier::SetStartTime (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_startTime = t;
}

Time
P1906EMMessageCarrier::GetStartTime (void)
{
  NS_LOG_FUNCTION (this);
  return m_startTime;
}

void
P1906EMMessageCarrier::SetCentralFrequency (double f)
{
  NS_LOG_FUNCTION (this << f);
  m_centralFrequency = f;
}

double
P1906EMMessageCarrier::GetCentralFrequency (void)
{
  NS_LOG_FUNCTION (this);
  return m_centralFrequency;
}

void
P1906EMMessageCarrier::SetBandwidth (double b)
{
  NS_LOG_FUNCTION (this << b);
  m_bandwidth = b;
}

double
P1906EMMessageCarrier::GetBandwidth (void)
{
  NS_LOG_FUNCTION (this);
  return m_bandwidth;
}

void
P1906EMMessageCarrier::SetSubChannel (double c)
{
  NS_LOG_FUNCTION (this << c);
  m_subChannel = c;
}

double
P1906EMMessageCarrier::GetSubChannel (void)
{
  NS_LOG_FUNCTION (this);
  return m_subChannel;
}

} // namespace ns3
