*This project has been created as part of the 42 curriculum by dievarga.*

## Description

Codexion is a multithreaded simulation project written in C. In this project
we have coders who need two USB dongles to compile their code. Each coder sits
in a circle and shares one dongle with the coder on their left and one with the
coder on their right.

To compile, a coder needs to grab both of their dongles at the same time. After
compiling, they debug, then refactor, then try to compile again. If a coder goes
too long without compiling, they "burn out" and the whole simulation stops. The
goal is to write a program where every coder gets to compile regularly, using
threads and mutexes to share the dongles safely, without deadlocks and without
anyone starving.

## Instructions

To compile the project, just run:
```
make
```
This creates the `codexion` executable.

To run it, you need to give it 8 arguments:
```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:
```
./codexion 5 800 200 200 200 3 50 fifo
```

- `scheduler` must be either `fifo` or `edf`.
- All the numeric arguments must be positive whole numbers.
- Maximum number of coders is 200, because of system memory restrictions.

Other Makefile rules:
- `make clean` removes the object files.
- `make fclean` also removes the executable.
- `make re` rebuilds everything from scratch.

## Resources

- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait` — the
  base documentation for everything used in this project.
- General reading on deadlocks and the Coffman conditions, to understand what
  causes them and how to avoid them.

## How AI was used:
I used Claude mainly to help me understand the man documentation and to catch
bugs I couldn't find on my own.
Specially tricky timing bugs that only showed up sometimes. It helped me find
and understand a deadlock caused by every coder always grabbing their left
dongle before their right one, a bug where the simulation could hang forever
after it was supposed to stop, and a subtle bug in my priority queue where the
wrong entry could get removed from a dongle's waiting list, causing a coder to
get stuck forever and burn out for no real reason.

## Blocking cases handled

- **Deadlock prevention:** if every coder always grabbed their left dongle
  first, then their right one, all coders could end up holding one dongle each
  and waiting forever for the next one (this is the classic circular wait,
  hold/wait deadlock). 
  To avoid it, each coder always tries to grab whichever of
  their two dongles comes first in memory, no matter if it's their left or
  right one. This breaks the circular waiting pattern.
- **Starvation prevention:** dongles are handed out using a priority queue
  (a heap), either in FIFO order (first come, first served) or EDF order
  (whoever is closest to burning out goes first). This makes sure no coder
  gets stuck waiting forever behind other coders (mutual exclusion).
- **Cooldown handling:** every dongle remembers the time it was released, and
  nobody can take it again until the cooldown time has passed (preemption).
- **Precise burnout detection:** a separate monitor thread checks all coders
  very often (every fraction of a millisecond) so that a burnout is detected
  and logged almost immediately, well within the required time window.
- **Log serialization:** all print statements go through the same mutex, so
  two coders can never have their log lines mixed up together.

## Thread synchronization mechanisms

Each dongle has its own `pthread_mutex_t` and `pthread_cond_t`. The mutex
protects the dongle's state (whether it's in use, its cooldown, its waiting
list), and the condition variable is used so a coder can sleep while waiting
for a dongle instead of wasting CPU time checking over and over.

There are also two mutexes at the simulation level: one to protect all the
`printf` calls so logs never overlap, and one to protect the "simulation
stopped" flag (this one is also reused to protect the FIFO ticket counter).

When a coder is done with a dongle, or when the simulation stops, the coder
(or the monitor thread) broadcasts on the condition variable(s) so
that any thread sleeping and waiting wakes back up and checks again if it's
their turn, or if the simulation is over.
