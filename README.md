# Brain
Biology-inspired Neural Networks evolving through Natural Selection

<img height="20%" width="20%" src="http://home.fnal.gov/~souvik/Brain/BotsScreenshot.png"/>

I have been developing, in my leisure, an original simulation of a biology-inspired neural network (brain) evolving in a naturally selective environment. I intend to uncover what natural mechanism, analogous to the artificial one of backpropagation, arises in biological networks that allow them to learn within a lifetime. The brains are found to quickly evolve survival behavior.

#### Time-stepped network

Unlike ANNs used for computation, this neural network operates in time steps. The state of the network at time t1 is derived from the state of the entire network at time t0. This allows input (sensory) neurons to carry time signatures, e.g. a sequential firing of neurons A, then B, and then C could mean a different thing than A, then C, and then B. This also means that the output (motor) neurons can direct what it controls in sophisticated ways. The possibility of impulses traveling in closed loops in the brain is also set up.

#### Dynamic synaptic weights

Synaptic weights are reinforced when impulses pass through them, and decay exponentially with the passage of time. This sets up the possibility of short-term patterns of behavior given a repeated input, and also the possibility of forgetting the behavior when the input is removed for a sufficiently long time. 

#### Distances between neurons

This neural network has no predetermined layering or structure of connections between the neurons. Instead, each neuron is connected to roughly 50% of the other neurons with both a synaptic weight and a distance parameter. While the synaptic weight varies during a lifetime, the distance parameter evolves across lifetimes and reflects the connection structure of the neural network. When the brain reproduces, it produces an offspring with a slightly mutated distance matrix. 

