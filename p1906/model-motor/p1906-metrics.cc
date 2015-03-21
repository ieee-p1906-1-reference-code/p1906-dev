/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
 *
 *  This source file is an essential part of IEEE Std 1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE Std 1906.1 may also be made and
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
 
/* \details This class implement the IEEE 1906.1 metrics
 *
 * <pre>
 * Metrics class monitors components, computes metrics, and exports results
 *                                               +----------------+    
 *                                    +--------> |                |    
 *                                    |          |  MATLABHelper  |    
 * +------------------------+   +-----+-----+    |                |    
 * |                        |   |           |    +----------------+    
 * | 1906 Component Classes +-> |  Metrics  |                            
 * |                        |   |           |    +--------------------+
 * +------------------------+   +-----+-----+    |                    |
 *                                    |          | MathematicaHelper  |
 *                                    +--------> |                    |
 *                                               +--------------------+
 * </pre>
 */

#include "ns3/log.h"

#include "p1906-metrics.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906_Metrics");

TypeId P1906_Metrics::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906_Metrics")
    .SetParent<Object> ();
  return tid;
}

P1906_Metrics::P1906_Metrics ()
{
  /** This class implements persistence length as described in:
	  Bush, S. F., & Goel, S. (2013). Persistence Length as a Metric for Modeling and 
	    Simulation of Nanoscale Communication Networks, 31(12), 815-824. 
		http://dx.doi.org/10.1109/JSAC.2013.SUP2.12130014.
		
    The Gnu Scientific Library (GSL) is required for these methods.
    All points and positions are in three dimensions comprised of a gsl_vector * of length three (x, y, z).
    All lines and segments are comprised of two points within a gsl_vector * of length size (x1, y1, z1), (x2, y2, z2).
    Lists of points or positions are in a gsl_matrix * of size n x 3.
    Each tube is comprised of a list of segments within a gsl_matrix * of size s x 6 -> s x ((x1, y1, z1), (x2, y2, z2)).
    A set of tubes is also a gsl_matrix * of size (s * t) x 6, where s is the number of segments and t the number of tubes.
    All random number are derived from gsl_rng *.
	  
  */

}

//! See Clause 6.1 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Message_Deliverability()
{}

//! See Clause 6.2 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Message_Lifetime()
{}

//! See Clause 6.3 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Information_Density()
{}

//! See Clause 6.4 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! Bandwidth-Delay Product is proportional to the maximum number of Message Carriers capable of fitting 
//! within the physical channel. It is important for communication protocols and algorithms to know this 
//! in order to design optimal communication control. The units are bits. Bandwidth-Delay Product is intuitively 
//! the maximum amount of bits the nanoscale communication pipe can hold at any instant in time. As the name 
//! implies it is BD where B is measured in bits-per-second and D is the transmission delay (in seconds) through 
//! the communication pipe.
//!
//! NOTE—IEEE 1906.1 systems will likely tend to have a very large bandwidth-delay product. Individual Message Carriers 
//! can move relatively slowly, and each individual Message Carrier might only encode small number of bits, but there 
//! will be large numbers of them, on the order of Avogadro’s constant. 
void P1906_Metrics::Bandwidth_Delay_Product()
{}

//! See Clause 6.5 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! The Message Carrier requires energy for its movement, propulsion (if active motion is used) and steering. If passive 
//! motion is utilized, energy is required in order to cause surrounding fluid flow or to create the gradient in the field. 
//! Moreover, energy can be required at the transmitter and receiver for the release and reception of the Message Carrier. 
//! This energy might be the result of Message Carrier synthesis at the transmitter and/or reception characteristics at 
//! the receiving parties. This is the metric that quantifies energy used in nanoscale communication. This is energy per 
//! bit of information conveyed by the Motion Component. It can be considered the efficiency of the Message Carrier. It is 
//! defined in Equation (3): 
//!	(3) \f[ E_{mc} \over I_{mc} \f]
//! where 
//!    	\f$E_{mc}\f$ is the energy expended to deliver a message
//!   	\f$I_{mc}\f$ is the information per Message Carrier in bits.
void P1906_Metrics::Information_and_Communication_Energy()
{}

//! See Clause 6.6 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! Collision Behavior measures the physical result of collision between Message Carriers. Upon impact, they 
//! can join, merge, or absorb one another or they can bounce or reflect away from one another. This metric 
//! has a significant impact upon whether and how Message Carriers disperse and their efficiency in reaching 
//! their target receivers. Collision Behavior is a measure of the interaction of colliding Message Carriers. 
//! NOTE—Collisions can be designed to accomplish more sophisticated actions, such as the exchange of Messages 
//! when collision occurs, for example, as part of a gossip-like communication routing protocol [B1]. In such 
//! a design, other metrics such as Specificity and Affinity between Message Carrier and Message can be relevant.
//! Collisions can either be elastic, meaning they conserve both momentum and kinetic energy, or inelastic, 
//! meaning they conserve momentum but not kinetic energy. An inelastic collision can also be called a plastic 
//! collision. A perfectly-inelastic collision (e.g. perfectly-plastic) is a limiting case of inelastic collision 
//! in which the two bodies stick together after impact. The degree to which a collision is elastic or inelastic is 
//! quantified by the coefficient of restitution, a value that generally ranges between zero and one. A perfectly 
//! elastic collision has a coefficient of restitution of one; a perfectly-inelastic collision has a coefficient 
//! of restitution of zero, which is defined in Equation (4)
//!  	(4) \f[ C_{r} = {V_{AC} \over V_{BC}} \f]
//! where 
//!  	\f$C_{r}\f$ is the coefficient of restitution 
//! 	\f$V_{AC}\f$ is the relative speed (energy, mass) between colliding Message Carriers after collision
//! 	\f$V_{BC}\f$ is the relative speed (energy, mass) between colliding Message Carriers before collision.
void P1906_Metrics::Collision_Behavior()
{}

//! See Clause 6.7 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Mass_Displacement()
{}

//! See Clause 6.8 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Positioning_Accuracy_of_Message_Carriers()
{}

//! See Clause 6.9 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! Persistence length is a measure of the degree to which a chain-like structure is either soft 
//! (like strings of cooked spaghetti) or rigid (like straight rods). This has a significant impact 
//! upon Message Carriers that ride along such track-like strands. If no chain-like structures comprise 
//! the media, then this value returns zero (0). Persistence length is in units of nanometers, where 
//! longer lengths define stiffer structures [B5]. Persistence length is the rate at which the tangents 
//! taken along each segment of a linear chain in a network become decorrelated from one another. If 
//! R(s) is a point on a segment s, then let u(s) be the unit tangent vector in Equation (9)
//!   	(9) \f[u(s) = {\partial R \over \partial s}\f]
//! where
//! \f$R(s)\f$ 	is a point on a segment \f$s\f$
//! The orientations of the unit tangent vectors for all segments s is quantified by the inner product in Equation (10)
//! (10) \f[ \langle u(s) \cdot u(0) \rangle = e^{-s \over \zeta_p}\f]
//! where
//! \f$u(s)\f$ is the unit tangent vector at point s in set of connected segments
//! \f$u(0)\f$ is the unit tangent vector at the origin or beginning of the chain of connected segments
//! \f$\zeta_p\f$ is the persistence length.
//! For longer persistence lengths, or for shorter tubes, the linear chain will tend to be straighter. For longer 
//! tubes or shorter persistence lengths, the impact of de-correlation along the chain tangent tends to become more 
//! significant. Longer persistence length, by definition, implies straighter tubes. However, tubes that are shorter 
//! in length are typically able to hold up better under physical stress in situ and thus are able to maintain a 
//! straighter shape as well. They also typically have less opportunity to curl up given their shorter length. 
//! However, keep in mind that this applies only to segments that comprise a tube and does not imply that the tubes 
//! themselves are aligned in any particular pattern. We can approximate the curved chains as many smaller random 
//! chains that happen to be connected end-to-end, but with de-correlated alignment. Thus, shorter persistence lengths 
//! will tend to decrease the percolation threshold, which is important in the explanation of network conductance that 
//! follows. Persistence length becomes important in applications of nanotubes in developing photovoltaic cells, fuel 
//! cells and electronic components such as transistors, primarily due to longer lengths having greater electrical 
//! resistance. The persistence length of a microtubule has been estimated to range from 0.2 to 5.2 mm, while the 
//! persistence length at the tip of a microtubule has been found to be much shorter. The rigidity and persistence 
//! length of microtubules has been found to be sensitive to various chemicals and related to various diseases. An 
//! advantage of persistence length over other metrics is that it relates to the physical nature of the tube structure. 
//! Persistence length is actually a basic mechanical property that measures not only the shape of a tube but also its 
//! stiffness. The usefulness of persistence length is that it extends beyond the shape of a tubular structure to infer 
//! its stress and strain. The persistence length is related the bending stiffness by Equation (11)
//!   	(11) \f[ \zeta_p = {B_S \over k_B T}\f]
//! where 
//! \f$B_S\f$ is the bending stiffness
//! \f$k_B\f$ is Boltzmann’s constant 
//! \f$T\f$ is temperature. 
//! The bending stiffness is related to Young’s Modulus as \f$B_S = YI\f$ where 
//! \f$B_S\f$ is Young’s Modulus 
//! \f$I\f$ is the area moment of inertia. 
//! Note that the area moment of inertia is \f$I = {\pi r^4 \over 64}\f$ where \f$r\f$ is the radius of the area moment of inertia. 
//! Thus, Young's Modulus is related to persistence length by Equation (12) where all variables have been previously defined.
//!   	(12) \f[ Y = {{64 K_B T} \over {\zeta_p \pi r^4}}\f]
//! Young's Modulus is the ratio of stress (pressure) to strain (dimensionless) and thus it has units of pressure. Knowledge 
//! of Young's Modulus allows an estimate of the degree to which a tube will extend with tension or buckle under compression. 
//! A typical technique to estimate the persistence length is to use image analysis of electron micrographs.
void P1906_Metrics::Persistence_Length()
{}

//! See Clause 6.10 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Diffusive_Flux()
{}

//! See Clause 6.11 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Langevin_Noise()
{}

//! See Clause 6.12 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Specificity()
{}

//! See Clause 6.13 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Affinity()
{}

//! See Clause 6.14 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Sensitivity()
{}

//! See Clause 6.15 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Angular_Spectrum()
{}

//! See Clause 6.16 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Delay_Spectrum()
{}

//! See Clause 6.17 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! Message Carriers can be programmed or coded such they change the underlying Media (e.g. microtubules, nanotubes, etc.) 
//! as they transport information. When specifically designed to do this in order to cause either a transient or permanent 
//! change in the Media, they are known as active message carriers. This metric provides a standard measure for how “active” 
//! a message carrier is. If Message Carriers do not change the underlying media then the value of this metric is zero. The 
//! units are change in Message Carrier flux through a given surface over a given time. An active network is designed to 
//! intentionally allow messages and information being transmitted to modify and improve the network itself. The Message 
//! Carrier might be designed to modify the network. Because this is a self-referential design (network elements 
//! constructing/improving the network that changes the motion/behavior of network elements constructing/improving the 
//! network etc…), it has been notoriously challenging to study and quantify. Active network programmability is based upon 
//! the amount and duration of change in network state that an individual Message Carrier can impart upon the network 
//! topology. This is illustrated in Figure 4 and quantified in Equation (28)
//!  	(28) \f[A = \oint_S \int_t \Delta fdt dS\f]
//! where 
//! \f$t\f$ is time
//! \f$S\f$ is a virtual surface that defines the volume through which the change in flux of Message Carriers must be clearly specified
//! \f$f(t)\f$ is the flux of Message Carriers as a function of time where flux is the rate of flow per unit area
//! \f$\Delta f(t)\f$ is the change in \f$f(t)\f$ intentionally caused by a programmed Message Carrier through a surface \f$S\f$. 
//!  
//! The impact of the change in Message Carrier flux can take time to implement and might not be permanent, thus the 
//! integral over time captures temporal aspects of the change. A discrete version of this metric is simply the change 
//! in Message Carrier flow through space as a change in network topology occurs intentionally caused by a Message Carrier, 
//! e.g. a Message Carrier causes microtubule or CNT network partitions to join together, split apart, or otherwise re-orient 
//! themselves. This effect can be measured by graph theoretic means such as the change in the graph Laplacian, a discrete 
//! form of the concept represented by this metric. The function   may be substituted by any of the metrics defined for the 
//! Field component, for example Persistence Length. The units of the function shall be stated along with the value of the 
//! Active Network Programmability.
//! \todo consider using vector field and divergence to compute this value
//! \todo see double P1906MOL_MOTOR_VolSurface::fluxMeter(gsl_matrix * tubeMatrix) to obtain the data.
void P1906_Metrics::Active_Network_Programmability(gsl_matrix * vf, gsl_vector * pt)
{
  //! sum the vector input and output to point pt
  //! may add a radius argument to define a sphere for determining flux through the surface of the sphere
  //! activeNetworkProgrammability is fundamentally a change to the vector field
}

//! See Clause 6.18 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! Perturbation Rate is a measure of both the rate and control of any type of perturbation used to send a signal 
//! in the system. It is designed to quantify the ability to control signal generation; typically, signal quality
//! degrades as the perturbation rate increases. This metric captures the change in signal quality as a function 
//! of rate. The term perturbation is used to be purposely broad in its application. It refers to changes in any 
//! of the other P1906.1 components with the intent of transmitting a signal. A larger rate of perturbation might 
//! allow for more communication bandwidth, however, this can come at a loss of accuracy in control of the perturbation 
//! or causing ill effects such as unintended resonance with other components of the system. This metric is a rate 
//! versus accuracy curve. Perturbation error is the difference between the intended perturbation rate and the 
//! actual perturbation rate.
void P1906_Metrics::Perturbation_Rate()
{}

//! See Clause 6.19 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
void P1906_Metrics::Supersystem_Degradation()
{}

//! See Clause 6.20 of P1906.1/D1.1 Draft Recommended Practice for Nanoscale and Molecular Communication Framework
//! The Bandwidth-Volume Ratio (BVR) takes into account and combines two fundamental essences of molecular 
//! and nanoscale communication, namely its size and bandwidth. The Bandwidth-Volume Ratio (BVR) is defined 
//! as the total system volume including transmitter, receiver, and message carrier multiplied by the bandwidth 
//! in bits per second that the system offers. The goal is to obtain a small total volume (V) and high 
//! bandwidth (BW), thus the ratio (BW / V) of an ideal system should approach infinity. V can be broken 
//! into the terms shown in Equation (31)
//!  	(31) \f[ V = V_{transmitter} + V_{receiver} + V_{message-carrier}\f]
//! where
//! \f$V_{transmitter}\f$ is the volume of the transmitter
//! \f$V_{receiver}\f$ is the volume of the receiver
//! \f$V_{message-carrier}\f$ is the volume of the Message Carrier.
//! Bandwidth (BW) can be broken down into various channel capacity equations. Designers can increase this metric 
//! by choosing which 1906.1 component volume to reduce versus increasing bandwidth in order to increase the value 
//! of this metric. Note that this metric applies equally well to macroscale systems including wireless, where the 
//! message carrier component is wavelength. The units are bits / (second nm3).
void P1906_Metrics::Bandwidth_Volume_Ratio()
{}

P1906_Metrics::~P1906_Metrics ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
