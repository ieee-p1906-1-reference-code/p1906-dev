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
#include "p1906-em-perturbation.h"
#include "ns3/p1906-message-carrier.h"
#include "ns3/p1906-perturbation.h"
#include "p1906-em-message-carrier.h"
#include "ns3/simulator.h"
#include "ns3/spectrum-value.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906EMPerturbation");

TypeId P1906EMPerturbation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906EMPerturbation")
    .SetParent<P1906Perturbation> ();
  return tid;
}

P1906EMPerturbation::P1906EMPerturbation ()
{
  NS_LOG_FUNCTION (this);
}

P1906EMPerturbation::~P1906EMPerturbation ()
{
  NS_LOG_FUNCTION (this);
}

void
P1906EMPerturbation::SetPowerTransmission (double ptx)
{
  NS_LOG_FUNCTION (this << ptx);
  m_powerTx = ptx;
}

double
P1906EMPerturbation::GetPowerTransmission (void)
{
  NS_LOG_FUNCTION (this);
  return m_powerTx;
}

void
P1906EMPerturbation::SetPulseDuration (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseDuration = t;
}

Time
P1906EMPerturbation::GetPulseDuration (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseDuration;
}

void
P1906EMPerturbation::SetPulseInterval (Time t)
{
  NS_LOG_FUNCTION (this << t);
  m_pulseInterval = t;
}

Time
P1906EMPerturbation::GetPulseInterval (void)
{
  NS_LOG_FUNCTION (this);
  return m_pulseInterval;
}

void
P1906EMPerturbation::SetCentralFrequency (double f)
{
  NS_LOG_FUNCTION (this << f);
  m_centralFrequency = f;
}

double
P1906EMPerturbation::GetCentralFrequency (void)
{
  NS_LOG_FUNCTION (this);
  return m_centralFrequency;
}

void
P1906EMPerturbation::SetBandwidth (double b)
{
  NS_LOG_FUNCTION (this << b);
  m_bandwidth = b;
}

double
P1906EMPerturbation::GetBandwidth (void)
{
  NS_LOG_FUNCTION (this);
  return m_bandwidth;
}

void
P1906EMPerturbation::SetSubChannel (double c)
{
  NS_LOG_FUNCTION (this << c);
  m_subChannel = c;
}

double
P1906EMPerturbation::GetSubChannel (void)
{
  NS_LOG_FUNCTION (this);
  return m_subChannel;
}

Ptr<P1906MessageCarrier>
P1906EMPerturbation::CreateMessageCarrier (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
  Ptr<P1906EMMessageCarrier> carrier = CreateObject<P1906EMMessageCarrier> ();

  double duration = m_pulseInterval.GetSeconds () * p->GetSize () * 8;
  double now = Simulator::Now ().GetSeconds ();

  NS_LOG_FUNCTION (this << "[t,bits,pulseD,pulseI,duration]" << now << p->GetSize() * 8
		  << m_pulseDuration << m_pulseInterval
		  << duration);

  //create the vector of frequencies
  std::vector<double> freqs;
  int nb_of_subchannels = GetBandwidth ()/GetSubChannel ();
  double startFrequency = GetCentralFrequency () - (GetSubChannel() * nb_of_subchannels/2) + GetSubChannel()/2;
  for (int i = 0; i < nb_of_subchannels; ++i)
    {
	  NS_LOG_FUNCTION (this << "[i,f]"<< i << startFrequency + (i*GetSubChannel ()));
      freqs.push_back (startFrequency + (i*GetSubChannel ()));
    }

  Ptr<SpectrumModel> NanoSpectrumModel = Create<SpectrumModel> (freqs);
  Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (NanoSpectrumModel);

  NS_LOG_FUNCTION (this << "[ptx,numChannels]" << m_powerTx << txPsd->GetSpectrumModel ()->GetNumBands ());

  double txPowerDensity = (m_powerTx / txPsd->GetSpectrumModel ()->GetNumBands ())/
		  GetSubChannel ();
  (*txPsd) = txPowerDensity;

  NS_LOG_FUNCTION (this << "[txPsd]" << *txPsd);

  carrier->SetPulseDuration (m_pulseDuration);
  carrier->SetPulseInterval (m_pulseInterval);
  carrier->SetDuration (Seconds(duration));
  carrier->SetCentralFrequency (GetCentralFrequency());
  carrier->SetBandwidth (GetBandwidth());
  carrier->SetSubChannel (GetSubChannel());
  carrier->SetSpectrumValue (txPsd);
  carrier->SetStartTime (Simulator::Now ());
  carrier->SetMessage (p);

  return carrier;
}

} // namespace ns3
