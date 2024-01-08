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

The openMPI libraries implements several algorithms to implement collective operations accordingly to many different parameters. The exercise consists in testing some of them  for a selected operation ( like for instance broadcast operation)  to estimate the latency and the bandwith of the selected operation.
The exercise does not require any programming effort: students are supposed to use a well known MPI benchmark: the  OSUone and they are supposed to run them on a single node of the cluster, choosing among epyc, thin and fat, using all the available cores.

## Steps to be performed:

 - download and install the OSU benchmark available at this page: https://mvapich.cse.ohio-state.edu/benchmarks/
 - select a whole computational node, i.e. an epyc one
 - select an additional tblocking MPI collective operation you want to test among one of the following five listed below.
 - familiarize with the `osu_bcast` and the additional collective operation you choose: run several repetions of the programs and collect performance number, estimating the error in order to have a baseline for the operation 
 - select for the two collective operation (bcast, mandatory for all, and the one you selected) at most three possible algorithms and perform the same set of measurements of the previous step.
 - collect and compare numbers among the baseline and the algorithms you choose.
 - try to understand/infer the performance model behind the algorithms you selected.\
 - report your result in a nice report and prepare a short presentation (no more that 10 slides)
   
## how to selet the openMPI algorithms available

By means of the 'oompi_info' we can see the detailed information about the openMPI implmentation we have on ORFEO cluster:
We focus here only on the  MCA coll tuned set of parameters: and we report the most important parameter accordingly to most important collective operations

  - barrier
  - broadcast
  - reduce
  - gather
  - scatter 

by means of the parameter listed below we are able to select different algorithms. In order to do this the following parameter is always needed: 
  
          
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

         
          MCA coll tuned: informational "coll_tuned_gather_algorithm_count"
                          (current value: "4", data source: default, level: 5
                          tuner/detail, type: int)
                          Number of gather algorithms available
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



We provide here an example: to select different algorithms for a specific operation (i.e. bcast) one should issue the following command:

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




