# Exercise 1 for the course High Performance Computing.

This is the exercise for prof. Cozzini section of the 2023/2024 HPC course. It consists of several possible exercises: please read carefully and decide which one to take. 

version 1.0: this document can be modified several times in the next few days in order to improve the clearness of the information to provide a better understanding of what we are asking.

## Rules:

- exercise should be done individually: no group please ! 
- Materials (code/scripts/pictures and final report)  should be prepared on a github repository, starting by this one and sharing it with the teachers.
- a report  should be sent by e-mail to the teachers at least five days in advanced: name of the file should YOURSURNAME_report.pdf
- results and numbers of the exercises should be presented (also with the help of slides) in a max ten minutes presentation: this will be part of the exam. A few more questions on the topic of the courses will be asked at the end of the presentation.
  


***deadlines***

You should send us the e-mail at least one week before the exam.
For the first two  scheduled "appelli" this means:

 - exam scheduled at xx.02.2024 ***deadline xx.02.2023 at midnight*** 
 - exam scheduled at xx.02.2024  ***deadline xx.02.2023 at midnight***

The report should clearly explain which software stack we should use to compile the codes and run all the programs you used in your exercises. Providing well-done Makefiles/scripts to automatize the work  is highly appreciated.

# THE EXERCISE: compare different openMPI algorithms for collective operations.

## Introduction

The openMPI libraries implements several algorithms to perform collective operations accordingly to many different parameters. The exercise consists in evalutation some of them  for two different collective operations:
  - broadcast operation: mandatory for all
  - a collective operation at your choice among the following four: gather, scatter, barrier, reduce

You are supposed to estimate the latency of default openMPI implementation, variyng the number of processes and the size of the messages exchanged and then compare this latter with the values you obtain using different algorithms. 
The exercise does not require any programming effort: students are supposed to use a well known MPI benchmark: the  OSUone and they are supposed to run them on at least two nodes of the ORFEO cluster, choosing among epyc, thin and fat, using all the available cores on a single node.


## Steps to be performed:

 - download and install the OSU benchmark available at this page: https://mvapich.cse.ohio-state.edu/benchmarks/
 - select a two whole computational node, i.e. two  epyc nodes
 - select an additional tblocking MPI collective operation you want to test among one of the following four listed above.
 - familiarize with the `osu_bcast` and the additional collective operation you choose: run several repetions of the programs and collect performance number, estimating the error in order to have a baseline for the two operations. 
 - select for the two collective operation (bcast, mandatory for all, and the one you selected) at most three possible algorithms and perform the same set of measurements of the previous step.
 - collect and compare numbers among the baseline and the algorithms you choose.
 - try to understand/infer the performance model behind the algorithms you selected.
 - report your result in a nice report and prepare a short presentation (no more that 10 slides)
   
## how to select the openMPI algorithms available

Open MPI architecture is based on software components, plugged into the library kernel. A component provides functionality with specific implementation features. For instance, a collective component known as *Tuned* implements different algorithms for each collective operation defined in MPI as a sequence of point-to-point transmissions between the involved processes.
By means of the 'oompi_info' we can see the detailed information about the openMPI implementation and parameter that one can choose in order to select different algorithms. In the following we report the parameter you neeed to choose to select different algorithms for the following collective operations:

  - barrier
  - broadcast
  - reduce
  - gather
  - scatter 

In order to enable this choice the following parameter must be specify: 
  
        
          MCA coll tuned: parameter "coll_tuned_use_dynamic_rules" (current
                          value: "false", data source: default, level: 6
                          tuner/all, type: bool)
                          Switch used to decide if we use static (compiled/if
                          statements) or dynamic (built at runtime) decision
                          function rules
                          Valid values: 0: f|false|disabled|no|n, 1:
                          t|true|enabled|yes|y

The parameters for the five operation we focus on are the following.
 
- barrier algorithms: 

          MCA coll tuned: parameter "coll_tuned_barrier_algorithm" (current
                          value: "ignore", data source: default, level: 5
                          tuner/detail, type: int)
                          Which barrier algorithm is used. Can be locked down
                          to choice of: 0 ignore, 1 linear, 2 double ring, 3:
                          recursive doubling 4: bruck, 5: two proc only, 6:
                          tree. Only relevant if coll_tuned_use_dynamic_rules
                          is true.
                          Valid values: 0:"ignore", 1:"linear",
                          2:"double_ring", 3:"recursive_doubling", 4:"bruck",
                          5:"two_proc", 6:"tree"

- bcast algorithms: Number of bcast algorithms available

                          
          MCA coll tuned: parameter "coll_tuned_bcast_algorithm" (current
                          value: "ignore", data source: default, level: 5
                          tuner/detail, type: int)
                          Which bcast algorithm is used. Can be locked down
                          to choice of: 0 ignore, 1 basic linear, 2 chain, 3:
                          pipeline, 4: split binary tree, 5: binary tree, 6:
                          binomial tree, 7: knomial tree, 8:
                          scatter_allgather, 9: scatter_allgather_ring. Only
                          relevant if coll_tuned_use_dynamic_rules is true.
                          Valid values: 0:"ignore", 1:"basic_linear",
                          2:"chain", 3:"pipeline", 4:"split_binary_tree",
                          5:"binary_tree", 6:"binomial", 7:"knomial",
                          8:"scatter_allgather", 9:"scatter_allgather_ring"

-reduce algorithms:

          
          MCA coll tuned: parameter "coll_tuned_reduce_algorithm" (current
                          value: "ignore", data source: default, level: 5
                          tuner/detail, type: int)
                          Which reduce algorithm is used. Can be locked down
                          to choice of: 0 ignore, 1 linear, 2 chain, 3
                          pipeline, 4 binary, 5 binomial, 6 in-order binary,
                          7 rabenseifner. Only relevant if
                          coll_tuned_use_dynamic_rules is true.
                          Valid values: 0:"ignore", 1:"linear", 2:"chain",
                          3:"pipeline", 4:"binary", 5:"binomial",
                          6:"in-order_binary", 7:"rabenseifner"
                          
-gather algorithms:
         
          MCA coll tuned: parameter "coll_tuned_gather_algorithm" (current
                          value: "ignore", data source: default, level: 5
                          tuner/detail, type: int)
                          Which gather algorithm is used. Can be locked down
                          to choice of: 0 ignore, 1 basic linear, 2 binomial,
                          3 linear with synchronization. Only relevant if
                          coll_tuned_use_dynamic_rules is true.
                          Valid values: 0:"ignore", 1:"basic_linear",
                          2:"binomial", 3:"linear_sync"
                          
-scatter algorithms: 
          
          MCA coll tuned: parameter "coll_tuned_scatter_algorithm" (current
                          value: "ignore", data source: default, level: 5
                          tuner/detail, type: int)
                          Which scatter algorithm is used. Can be locked down
                          to choice of: 0 ignore, 1 basic linear, 2 binomial,
                          3 non-blocking linear. Only relevant if
                          coll_tuned_use_dynamic_rules is true.
                          Valid values: 0:"ignore", 1:"basic_linear",
                          2:"binomial", 3:"linear_nb"



We provide here an example how to select different algorithms for a specific operation (i.e. bcast) one should issue the following command:

    mpirun  --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 0 osu_bcast

that provides the following output:

```
# OSU MPI Broadcast Latency Test v7.3
# Datatype: MPI_CHAR.
# Size       Avg Latency(us)
1                      13.52
2                      14.06
4                      14.21
8                      14.23
16                     14.65
32                     15.71
64                     15.47
128                    54.24
256                    53.93
512                    51.28
1024                   51.87
2048                   52.75
4096                   57.64
8192                   55.57
16384                  75.01
32768                  83.51
65536                 107.96
131072                132.61
262144                222.99
524288                409.06
1048576               986.45
```

The program gives the latency on 128 processor with the default algorithm choosen automatically by openMPI. One can now play with other algorithms, for instance number 3 (pipeline): 

 ```
 mpirun  --mca coll_tuned_use_dynamic_rules true --mca coll_tuned_bcast_algorithm 3 osu_bcast


# OSU MPI Broadcast Latency Test v7.3
# Datatype: MPI_CHAR.
# Size       Avg Latency(us)
1                      28.93
2                      26.80
4                      27.49
8                      28.29
16                     30.38
32                     29.78
64                     30.04
128                    34.72
256                    31.95
512                    33.91
1024                   40.92
2048                   47.01
4096                   61.56
8192                   92.10
16384                 295.37
32768                 432.01
65536                 746.98
131072               1375.86
262144               2710.71
524288               5466.00
1048576             11033.92
```

as one can notice the difference is remarkable.

## Details on broadcast algorithms

Reference 1 below reported discusses in details a few algorithms implemented in the openMPI MPI_Bcast routine; we report here a full section of the paper to help understanding better the way they  work and to infer a possible performance model for each of them.

In the broadcast operation (MPI_Bcast) a process called root sends a message with the same data to all processes in the communicator. Messages can be segmented in transmissions. Segmentation of messages is a common technique used for increasing the communication parallelism by avoiding the rendezvous protocol, and hence, improving the performance. It consists of dividing up the message into smaller fragments called segments and sending them in sequence.

Every algorithm implementing the broadcast in the *Tuned* component defines a communication graph with a specific topology between the P ranks in the communicator. Ranks are the nodes in the graph, and they are mapped to the processes of the parallel machine. The features and topology of the broadcast algorithms implemented in Open MPI Tuned component are listed below:

- *Flat tree algorithm*. The algorithm employs a single level tree topology shown in Fig. 3a where the root node has P-1 
 children. The message is transmitted to child nodes without segmentation.

- *Chain tree algorithm*. Each internal node in the topology has one child (see Fig. 3b). The message is split into segments and transmission of segments continues in a pipeline until the last node gets the broadcast message. ith process receives the message from the (i-1)th process, and sends it to (i+1)th process.

- *Binary tree algorithm*. Unlike the chain tree, each internal process has two children, and hence data is transmitted from each node to both children (Fig. 3c). Segmentation technique is employed in this algorithm. For simplicity we assume that the binary tree is complete, then  P=2powered to H -1 where H is the height of the tree, i.e H=log(P-1) 

- *Split binary tree algorithm*. The split binary tree algorithm employs the same virtual topology as the binary tree (Fig. 3c). As the name implies, the difference from the binary tree algorithm is that the message is split into two halves before transmission. After splitting the message, the right and left halves of the message are pushed down into the right and left sub-trees respectively. In an additional last phase, the left and right nodes exchange in pairs their halves of the message to complete the broadcast operation.

- *K-Chain tree algorithm*. The K-Chain virtual topology is employed in the algorithm (Fig. 3d). The root broadcasts the message using segmentation to the child processes, and then the child processes broadcast the message to their children in parallel. As the name implies, the virtual topology consists of K chain tree virtual topology each of which is connected to root. The height of K-chain tree is estimated as H = (P-1)/K . Last process must wait for H(k-chain) steps until it gets the broadcast message.

- *Binomial tree algorithm*. The binomial tree topology is determined according to the binomial tree definition [20]. The algorithm employs balanced binomial tree (Fig. 3e). Unlike the binary tree, the maximum nodal degree of the binomial tree decreases from the root down to the leaves as follows: logP, logP -1, logP -2 ....   The height of the binomial tree is the order of the tree:H=logP 






