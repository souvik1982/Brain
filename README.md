<html>
 <body>
 
  <h2 align="center">A neural network in a genetic algorithm develops survival behavior</h2>
  
  <p style="margin-left: 10em; margin-right: 10em">
    I have been developing, in my leisure, an original simulation of a biology-inspired neural network (brain) evolving in a natural selection environment. Survival behavior is observed to emerge rapidly. <b>This is not an effort to create an efficient learning system but an exploration of how learning mechanisms like backpropagation and reinforcement learning may emerge in biological brains.</b> Below, I point out the salient features that set this neural network apart from traditional artificial neural networks. Thereafter, I discuss the behaviors I observe and various avenues that I think this research could take.
  </p>
  
  <p style="margin-left: 10em; margin-right: 10em">
    I conjecture that evolution sets policy gradients for (genetic) survival over multiple generations. These policy gradients are burned into the network topology. The network topology, in turn, directs (something like) reinforcement learning implemented on synaptic weights such that the individual brain learns to optimize reward during the course of one lifetime.
  </p>
  
  <p style="margin-left: 10em; margin-right: 10em">
    <a href="http://home.fnal.gov/~souvik/Brain/index.html"> Animations and more details here. </a>
  </p>
  
  <table align="center">
   <tr>
    <td>
     <img src="http://home.fnal.gov/~souvik/Brain/c_World_8920400.png" width="100%" height="100%" style="margin-right: 2em" border="1"/><br/>
     <i>Bots with 30 neurons each search for and race after "food". Shown here is their behavior between generations 1000 and 1100.</i>
    </td>
    <td>
     <img src="http://home.fnal.gov/~souvik/Brain/c_dtime_generation.png" width="100%" height="100%" style="margin-left: 2em" border="1"/><br/>
     <i style="margin-left: 2em"> Punctuated equilibria leading to best possible 30 neuron brain for chasing food. </i>
    </td>
   </tr>
  </table>
  
  <p style="margin-left: 10em; margin-right: 10em">
   Each bot (blue circle) is controlled by a neural network (brain). The brain has 9 sensory neurons that take input from its field of vision, represented here as a grid in front of each bot. The brain also has 3 motor neurons that tell the bot to take a step forward, turn a degree to the left, and turn a degree to the right, respectively. The rest of the neurons in the brain (30 or so) mediate signals from the sensory to motor neurons. The connection topology of the brain is initially random. The neural network operates in time steps. Each neuron has a time-stepped model of signal integration, activation, leakage and refraction. Each synapse has a time-stepped model of Hebbian reinforcement and leakage. The initial behaviors of the bots are purely random and show no correlation between what they see and what they do. Every time a bot bumps into food (green), a daughter bot is generated with the same brain as its parent but with small random mutations in its connection topology. At the same time, the oldest bot in the group has to die. This simple evolutionary rule causes the brain topology, over a few thousand generations, to generate survival-like behavior as the bots search for and dart towards food ever more effectively.
  </p>
  <p style="margin-left: 10em; margin-right: 10em">
   The average time it takes for a bot to get to food decreases through punctuated equilibria over several generations.
  </p>
  
  
  <h2> Description of the Neural Network </h2>
  
  <ul>
   
   <li><b>Time-stepped network</b> 
    <blockquote>Unlike ANNs used for computation, this neural network operates in time steps. The state of the network at time t1 is derived from the state of the entire network at time t0. This allows input (sensory) neurons to carry time signatures, e.g. a sequential firing of neurons A, then B, and then C could mean a different thing than A, then C, and then B. This also means that the output (motor) neurons can direct what it controls in sophisticated ways. The possibility of impulses traveling in closed loops in the brain is also set up. Such self-reinforced loops, if they emerge, could indicate a form of proto-memory.
    </blockquote>
   </li>
   
   <li><b>Network topology</b>
    <blockquote> This neural network has no predetermined layering or structure of connections between the neurons. The topology of the network is defined by a directed network of 0 or 1 connections between every pair of neurons in the brain. This topology-network, or the connectome, does not vary during the lifetime of the neural network. It can be thought of an underlying hard-wired network over which the network consisting of synapses must operate. It mutates as it is passed on to oppspring. Layerings and structures relevant for (reinforcement) learning are expected to emerge in the connectome.
    </blockquote> 
   </li>
    
   <li><b>Dynamic synaptic weights</b>
    <blockquote>Synaptic weights are reinforced by Hebb's rule. When a neuron fires, it reinforces the synaptic weights of its fan-in that connect to neurons that also fired in the previous time step. Synaptic weights also decay slowly, exponentially.
    </blockquote>
   </li>
    
  </ul>
  
  
  <h2> Interesting results and observations </h2>
  
  <ul>
  
   <li><b> Minimum input frequency for a neuron to fire </b>
    <blockquote> If there is exactly 1 input to a neuron, given leakage of the neuron's potential, there has to be a minimum input frequency below which the neuron cannot get fired. One can derive an analytical expression for this minimum frequency. I posit such a condition should also exist in biology. If there is more than 1 input, a more complicated condition for the minimum input frequency for each input should exist.
    </blockquote>
   </li>
   
   <li><b> Evolution through punctuated equilibria </b>
    <blockquote> If we plot any quantity that measures "progress" in this natural selection environment, it is found to change through punctuated equilibria over generations. For example, if we plot the average time it takes for bots to get to food, it decreases through punctuated equilibria over almost 2 orders of magnitude. Mainstream theories of biology posit that changes to the environment trigger the punctuations between equilibria. Here, we have a toy example where the environment does not change and yet punctuated equilibria are observed. This can be attributed to local minima in the <a href="https://en.wikipedia.org/wiki/Fitness_landscape">fitness landscape</a> that lie close to the global minimum.
    </blockquote>
   </li>
   
   <li><b> Complex behavior in predator-prey scenarios</b> <br/>
    <blockquote>
     <img src="http://home.fnal.gov/~souvik/Brain/Movie_PredatorPrey.gif" height="50%" border="1"/>
    </blockquote>
   </li>
   
  </ul>
  
  
  <h2> Future directions </h2>
  
  <ul>
  
   <li><b>Isolate bots for learning, not breeding</b>
   </li>
  
   <li><b>More complex physical environments</b>
   </li>
   
   <li><b>A social environment</b>
    <blockquote> Mastering a complex physical game environment should result in complex brain topologies that are bounded by the complexity of the physical environment. The same can be said for mastering a set of game environments. Adversarial networks training as opponents in a game environment can take the behavioral complexity of the networks a bit farther. However, open-ended complexity may be achieved by creating a social environment between the neural networks. This would create arbitrarily complex behavior of the neural networks where much intelligence and abstract representation of the game environment would be carried in the inter-network communication.
    </blockquote>
    <blockquote> A way to create a social environment through inter-network communication would be to allow some neurons of each brain to be permanently attached to some neurons of other brains in the game with synaptic weight = 1. There is a temptation to view this as a mega-brain composed of several brains connected through bottlenecks. However, this is not the case since each individual brain is pursuing its own (genetic) survival.
    </blockquote>
    <blockquote> Within the communication between brains concepts like deceit and cooperation may arise. Given sufficient communication bandwidth between the brains and a sufficiently complex physical environment, it is conceivable that abstractions for social maneuvering fairly removed from direct representations of the game environment may arise in these communications. If such a thing happens, then the complexity achieved by these brains and the capacity for abstraction may truly be open-ended. This would parallel the evolutionary development of the neocortex seen in creatures that routinely navigate social environments -- primates, humans, dolphins and pilot whales.
    </blockquote>
    <blockquote> Generating a social environment does not require the biologically-inspired neural network model described here. It can be generated among traditional neural networks with reinforcement learning.
    </blockquote>
    
   </li>
   
  </ul>
  
 </body>
</html>
