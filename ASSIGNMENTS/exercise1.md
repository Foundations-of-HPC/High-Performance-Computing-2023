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

 - download and install the OSU benchmark available at this page:
 - select the blocking MPI collective operation you want to test
 - select a whole computational node, i.e. an epyc one
 - run several repetions of the programs and collect performance number, estimating the error, without specifiyng any particular algorithm.
 - repeat the previuos step using at least two different algorithms.
 - collect and compare numbers and try to understand/infer the performance model behind the algorithms you selected.
   
## how to selet the openMPI algorithms available

By means of the 'oompi_info'  we can see the detailed information about the openMPI implmentation we have 




