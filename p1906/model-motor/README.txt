== Reference Code Extensions ==
This is a quick start guide to the IEEE P1906.1 Reference Code: Molecular Motor Extension.

== Map ==
The following figure illustrates how the IEEE 1906 Components map to Molecular Motor Communication.

<pre>
  IEEE 1906 Component        Molecular Motor 
                              Instantiation
 +----------------------+-----------------------+
 |                      |                       |
 |    MESSAGE           |  MOTOR CARGO          |
 |                      |                       |
 +----------------------------------------------+
 |                      |                       |
 |    MESSAGE CARRIER   |  MOLECULAR MOTOR      |
 |                      |                       |
 +----------------------------------------------+
 |                      |                       |
 |    MOTION            |  BROWNIAN / WALK      |
 |                      |                       |
 +----------------------------------------------+
 |                      |                       |
 |    FIELD             |  MICROTUBULE          |
 |                      |                       |
 +----------------------------------------------+
 |                      |                       |
 |    PERTURBATION      |  MOTOR CARGO TYPE     |
 +----------------------------------------------+
 |                      |                       |
 |    SPECIFICITY       |  BINDING TO TARGET    |
 |                      |                       |
 +----------------------+-----------------------+
</pre>

== Class Summary ==
The following is concise summary of each class in the Molecular Motor extension.

=== P1906MOL_MOTOR_MicrotubulesField [extends P1906MOL_MOTOR_Field] ===
File: p1906-mol-motor-field-microtubule.cc
This class implements a set of microtubules. It's constructor currently contains a set of unit tests.

=== P1906MOL_MOTOR_Field [extends P1906MOLField] ===
File: p1906-mol-motor-field.cc 
This class extends the 1906.1 Field component class with vector field related methods.

=== P1906MOL_MOTOR_Motion [extends P1906MOLMotion] ===
File: p1906-mol-motor-motion.cc
This class extends the 1906.1 Motion component class with different types of molecular motion.

=== P1906MOL_MOTOR_Tube [extends P1906MOL_MOTOR_Field] ===
File: p1906-mol-motor-tube.cc
This class implements a tube-like nanoscale structure, e.g. microtubule or nanotube; comprised of tube geometry methods.

=== P1906MOL_MOTOR_Pos [extends Object] ===
File: p1906-mol-pos.cc
This class implements three dimensional location management for recording position.

=== P1906MOL_Motor [extends P1906MOL_MOTORMessageCarrier] ===
File: p1906-mol-motor.cc
This class implements a molecular motor. It decides when to walk on a tube and float freely. It also maintains volume surfaces described later.

=== P1906MOL_MOTOR_VolSurface [extends P1906MOL_MOTOR_Field] ===
File: p1906-mol-motor-vol-surface.cc
This class implements volume surfaces that can take the form of a FluxMeter, ReflectiveBarrier, and Receiver. It is used to measure flow, bound movement, and define where the receiver is located respectively.

=== P1906MOL_MOTOR_MathematicaHelper [extends Object] ===
File: p1906-mol-motor-MathematicaHelper.cc
This class writes data to be imported into Mathematica.

=== P1906MOL_MOTOR_MATLABHelper [extends Object] ===
File: p1906-mol-motor-MATLABHelper.cc
This class writes data to be imported into MATLAB.

=== P1906_Metrics [extends Object] ===
File: p1906-metrics.cc
This class implements the IEEE 1906.1 metrics. It holds methods for all the metrics, however, only Active Network Programmability is currently implemented.

=== P1906MOL_MOTOR_Perturbation [extends P1906Perturbation] ===
File: p1906-mol-motor-perturbation.cc
Required to extend the IEEE 1906 core reference model.

=== P1906MOL_MOTOR_CommunicationInterface [extends P1906CommunicationInterface] ===
p1906-mol-motor-communication-interface.cc
Required to extend the IEEE 1906 core reference model.

=== P1906MOL_MOTOR_CommunicationInterface [extends P1906ReceiverCommunicationInterface] ===
File: p1906-mol-motor-receiver-communication-interface.cc
Required to extend the IEEE 1906 core reference model.

=== P1906MOL_MOTOR_TransmitterCommunicationInterface [extends P1906TransmitterCommunicationInterface] ===
File: p1906-mol-motor-transmitter-communication-interface.cc
Required to extend the IEEE 1906 core reference model.

=== microtubules-example.cc ===
File: examples/microtubules-example.cc
Uses all of the above and ns-3 to send a packet via a molecular motor message carrier.

== Quick Start ==
These are the general steps to get up and running quickly by showing a simple, example model. See P1906MOL_MOTOR_MicrotubulesField::unitTest methods in the file p1906-mol-field-microtubule.cc for more examples.
It is assumed that the reader is familiar with both ns-3 and the IEEE 1906 core reference model classes at this point.

=== Step 1: Create Microtubules ===
Microtubules are not required to exist, however, if you wish to create them, they are constructed as shown in the following Sample Code. They remain in the extended Field class and can impact motion.

==== Sample Code ====

  //! set the microtubule network properties
  setTubeVolume(&ts, 25);
  setTubeLength(&ts, 100);
  setTubeIntraAngle(&ts, 30);
  setTubeInterAngle(&ts, 10);
  setTubeDensity(&ts, 10);
  setTubePersistenceLength(&ts, 50);
  setTubeSegments(&ts, 10);
 
  //! optionally display the microtubule network properties
  displayTubeChars(&ts);
  
  //! allocate space for the microtubules
  tubeMatrix = gsl_matrix_alloc (ts.numTubes * ts.segPerTube, 6);
  
  //! this method actually creates the microtubules
  genTubes(&ts);

  //! write the microtubules to a Mathematica file  
  mathematica.tubes2Mma(tubeMatrix, ts.segPerTube, "tubes.mma");

=== Step 2: Create a Motor ===
In this step we create a motor and set it's initial position. Notice that GetDiffusionConefficient() is inherited from the molecular diffusion model and allows us to reuse the diffusivity coefficient.

==== Sample Code ====
 
  //! create a Mathematica object to help with writing data
  P1906MOL_MOTOR_MathematicaHelper mathematica;
  //! allocate space for the starting location
  gsl_vector * startPt = gsl_vector_alloc (3);
  //! this is the time duration for each movement step
  double timePeriod = 100;
  //! allocate space for the Mathematica output file name
  char plot_filename[256];
  //! convert meters to nanometers
  float distanceMultiplier = pow(10, 9);
  //! mass diffusivity (default)
  double D = 1.0; //! mass diffusivity (default value)
  
  //! this is an ns-3 log
  NS_LOG_FUNCTION (this << "beginning ComputePropagationDelay");
  
  //! the coefficient is entered at run time; this is reused from the molecular diffusion model
  D = GetDiffusionConefficient ();
  //! note that D is not used here, the goal is just to show how it can be retrieved
  
  //! retrieve ns-3 node position from the ns-3 mobility model
  Ptr<MobilityModel> srcMobility = src->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  Ptr<MobilityModel> dstMobility = dst->GetP1906NetDevice ()->GetNode ()->GetObject<MobilityModel> ();
  
  //! store the positions
  Vector sv = srcMobility->GetPosition();
  Vector dv = dstMobility->GetPosition();
    
  //! create a motor; the motor extends the IEEE 1906 core Message Carrier
  Ptr<P1906MOL_Motor> motor = message->GetObject <P1906MOL_Motor> ();
  
  //! reset the motor's timer
  motor->initTime();
   
  //! Starting position is the transmitting node location
  P1906MOL_MOTOR_Field::point (startPt, sv.x, sv.y, sv.z);
  
  motor->setStartingPoint(startPt);
    
=== Step 3: Set Destination and Reflective Boundary ===
We need to tell the motor where it's destination is located so it knows when to stop. This is extremely important, otherwise the motor will continue wandering forever without a destination. In the illustration below, motors are created in the center of the Reflective Barrier surface volume and are considered to be received with then pass through the Receiver volume surface. In this example we ignore microtubules for simplicity.

==== Volume Surface Diagram ====

<pre>
          The Surface Measures Flux, Constrains Particle 
                 Motion, and Defines a Receiver
                     _,.,---''''''''---..__
                _.-''                      `-.._
             ,-'                                `..
          ,-' __                                   `._
        ,'  ,'  `-.  Motor received here              `.
      ,'   /      _\____                                \
     /    |    X   |   /                                 `.
    /      \      ,'  /____                                \
   /        `._,,'        /                                 \
  |    Receiver Surface  /                                   |
  |                     /    Motor transmitted here          |
 |                      -------X  _,''   ``._                |
 |                               /           \               |
 |                              /             \              |
  |                            |       X       |             /
  \                            `.             .'            /
   \                            |             |            ,'
    \                           `-.         ,'            ,'
     `.                            `..__,,,'             /
       `.                       FluxMeter Surface      ,'
         `.                                          ,'
           `.                                     _,'
             `-._                              ,,'
                 `-..__                  _,.-''
                       ``---........---''

          Reflective Barrier Volume Surface
           
</pre>

==== Sample Code ====
  
  //! create a position object
  P1906MOL_MOTOR_Pos dvol;
  //! Receiver volume surface center is based upon the receiving Node's location
  dvol.setPos (dv.x * distanceMultiplier, dv.y, dv.z);
  //! the receiving volume is a sphere centered at the receiving Node's location with a radius that is slightly smaller than the distance from the transmitter
  motor->addVolumeSurface(dvol, (dv.x * distanceMultiplier)/1.0001, P1906MOL_MOTOR_VolSurface::Receiver);
 
  //! add a reflective barrier sphere around the source and destination, centered at the transmitter
  dvol.setPos (sv.x, sv.y, sv.z);
  //! the reflective barrier sphere radius is just larger than the Receiver so that it includes the receiving node
  motor->addVolumeSurface(dvol, distanceMultiplier * (dv.x + (0.1 * dv.x)), P1906MOL_MOTOR_VolSurface::ReflectiveBarrier);

  //! the reflective barrier volume surface must overlap with receiver volume in order for the test to end
  motor->displayVolSurfaces();

=== Step 4: Configure Measurements ===
We can configure measurements, including those necessary for IEEE 1906 metrics, by creating the FluxMeter volume surface. Only the Active Network Programmability metric has been implemented thus far.

==== Sample Code ====
  
  //! add another volume surface to measure flow
  P1906MOL_MOTOR_Pos v_c;
  v_c.setPos (500, 0, 0);
  motor->addVolumeSurface(v_c, 100, P1906MOL_MOTOR_VolSurface::FluxMeter);

  //! print out all the motor's volume surfaces
  motor->displayVolSurfaces();
  
=== Step 5: Execute the Model ===
Now that everything has been created and configured, simulate the actual motion of the motor in the Sample Code below.

It is important to be aware of the motor motion method that is used:
* float2Destination() will ignore the microtubules and simply use Brownian motion until the destination is reached. 
* move2Destination() will walk along the tubes if contact is made with a tube. 

Another important point to keep in mind is that because motion is random, it may take a *very* long time to reach the destination. The P1906MOL_MOTOR_VolSurface::ReflectiveBarrier can help with  this by bounding the space within which the motor can move.

==== Sample Code ====

  //! send the motor to a type of motion until destination reached
  float2Destination(motor, timePeriod);
  
=== Step 6: Create Output ===
The Mathematica and MATLAB helper classes may be useful for exporting data for analysis as well
as for debugging. See the Sample Code below for printing out the movement history of the motor
from the previous steps.

==== Sample Code ====

  //! create a unique filename based upon Node x locations
  sprintf (plot_filename, "float2destination_%lf_%lf.mma", sv.x, dv.x * 1000);
  
  //! export the motors movement history to Mathematica
  mathematica.connectedPoints2Mma(motor->pos_history, plot_filename);

=== Step 7: Return ===
Compute and return the motor propagation time.

==== Sample Code ====

  //! return the time for the motor's journey form its creation to the receiver  
  return motor->getTime();

=== Step 8: Integrate with IEEE 1906 Reference Code ===
This is probably the most important step to learn: how to properly integrate your code with the reference model. The two most important methods for this integration are shown in the Sample Code below. These are methods that appear in the Motion class and are created when we extend the Motion class for our particular application. This application extends the molecular diffusion model, which had extended the core Motion class. 

First, ComputePropagationDelay() provides pointers to the ns-3 information required to simulate the motor, or hopefully any, propagation delay. All of the previous Sample Code is inside (except creation of the microtubules) is inside this method. 

Second, CalculateReceivedMessageCarrier() simply returns the message carrier as it appears upon reception at the receiver, which in this case is simply a motor.

==== Sample Code ====

  double P1906MOL_MOTOR_Motion::ComputePropagationDelay (Ptr<P1906CommunicationInterface> src,
  		                                  Ptr<P1906CommunicationInterface> dst,
  		                                  Ptr<P1906MessageCarrier> message,
  		                                  Ptr<P1906Field> field)
  {
    (all the prior code above goes here in order to compute propagation delay by actually simulating a motor)
  }


  //! this method is called from inside the core Medium class before reception occurs
  //! this returns the receivedMessageCarrier that appears at the receiver
  Ptr<P1906MessageCarrier> P1906MOL_MOTOR_Motion::CalculateReceivedMessageCarrier(Ptr<P1906CommunicationInterface> src,
  		                                                           Ptr<P1906CommunicationInterface> dst,
  		                                                           Ptr<P1906MessageCarrier> motor,
    		                                                           Ptr<P1906Field> field)
  {
    //! 'message' above is really the message carrier (motor)
    
    NS_LOG_FUNCTION (this << "Do nothing for motor");
    return motor;
  }

== Notes ==
* vector field reconstruction using 3D interpolation is done using output data in Mathematica
* more IEEE 1906 metrics should be implemented and tested

[[Category:Reference Model]]