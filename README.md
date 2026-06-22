# CPU Scheduler and Cache Hierarchy Simulator

A C++ based simulator that demonstrates how an operating system schedules processes and how memory requests move through a multi-level cache hierarchy.

The project combines:

- Round Robin CPU Scheduling
- L1, L2, L3 Cache Simulation
- FIFO Cache Replacement Policy
- RAM Access Tracking
- Cycle-based Performance Analysis

---

## Project Overview

Every running program competes for two important resources:

1. CPU time
2. Memory access

This simulator models both:

- The **scheduler** decides which task gets CPU execution.
- The **cache hierarchy** decides how quickly memory requests are served.

The goal is to visualize how scheduling decisions and cache behavior affect total execution cycles.

---

# Features

## 1. CPU Scheduler

Implemented:

### Round Robin Scheduling

- Each task gets a fixed CPU time slice (quantum).
- After using its quantum, the task moves back into the ready queue if unfinished.
- Ensures fair CPU allocation between tasks.


## 2. Cache Hierarchy Simulation

The memory system contains:

| Level | Capacity | Latency |
|---|---|---|
| L1 Cache | 32 blocks | 4 cycles |
| L2 Cache | 128 blocks | 12 cycles |
| L3 Cache | 512 blocks | 40 cycles |
| RAM | Unlimited | 200 cycles |

---
If a block is found:

- Access latency of that cache level is added.
- Block is promoted back to L1.

---

## Cache Replacement Policy

Each cache uses:

### FIFO (First In First Out)

When a cache becomes full:

- Oldest block is removed.
- New block is inserted.

---

# Future Improvements


*LRU eviction 

*Multi-core 

*Multiple scheduling algorithms

*Write-back policy 

*ASCII live animation
