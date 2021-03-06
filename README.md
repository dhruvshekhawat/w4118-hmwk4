Implementation of a Linux RR scheduler and an Android Linux group based scheduler.
==
Copyright (C) 2014 V. Atlidakis, G. Koloventzos, A. Papancea

COMS-W4118 Columbia University

## MODIFIED/ADDED FILES:

- flo-kernel/arch/arm/configs/flo_defconfig
  defined CONFIG_GRR and CONFIG_GRR_GROUPS flags

- flo-kernel/arch/arm/configs/goldfish_armv7_defconfig
  defined CONFIG_GRR and CONFIG_GRR_GROUPS flags

- flo-kernel/arch/arm/include/asm/unistd.h
  defined sched_set_CPUgroup syscall at position 378

- flo-kernel/arch/arm/kernel/calls.S
  added syscall to syscall table at position 378

- flo-kernel/include/linux/init_task.h
  modified init task to be scheduled using the GRR policy

- flo-kernel/include/linux/sched.h
  modified sched.h for various headers and structs used
  within the assignment; primarily, we defined SCHED_GRR
  policy, sched_grr_entity

- flo-kernel/include/linux/syscalls.h
  defined the syscall sched_set_CPUgroup

- flo-kernel/init/Kconfig
  more setup for the CONFIG_GRR and CONFIG_GRR_GROUPS flags

- flo-kernel/kernel/kthread.c
  added sched_setscheduler_nocheck() function for the GRR policy

- flo-kernel/kernel/sched/Makefile
  added grr.c binary to the kernel compilation

- flo-kernel/kernel/sched/core.c
  implementation of the sched_set_CPUgroup syscall, as well as
  matching relevant code to the other scheduling policies;
  added timer for the GRR load balancer

- flo-kernel/kernel/sched/debug.c
  added print_grr_rq function for GRR debugging

- flo-kernel/kernel/sched/grr.c
  GRR class implementation

- flo-kernel/kernel/sched/rt.c
  added grr_sched_class as the next scheduling class
  after the realtime scheduling class

- flo-kernel/kernel/sched/sched.h
  modified sched.h for various headers and structs used
  within the assignment; primarily, we defined SCHED_GRR
  policy, sched_grr_entity

- test/Makefile
  Makefile for the test program

- test/set_sched_policy.c
  test program that auto-schedules itself using the GRR class

- test/test.c
  test program that assigns cores to groups

## BRANCHES AND EXERCISE PARTS

uniprocessor_rr -> Here you can find the round robin algorithm for a 
uniprocessor. We have left 2 print statements in pick_next_task and enqueue
in order to see that the RR algorithm takes place.

smp_devel -> Here you can find the part 2 of our assignment.

## RESOURCES USED:

1. Linux Cross Reference
   http://lxr.free-electrons.com

2. Linux Kernel Development (3rd Edition)

3. Operating Systems: Principles and Practice (2nd Edition)

