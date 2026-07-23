*This project has been created as part of the 42 curriculum by dievarga.*

## Description
Codexion is a multi-threaded simulation within a shared environment. In this project, multiple concurrent coders sit in a circular workspace where they compile code, debug, or refactor at individual specific times.

To start a compilation phase, a coder must simultaneously claim exclusive ownership over exactly two shared dongles, the one on their left and the one on their right. Since dongles are a shared resource between contiguous desk neighbors, threads must coordinate access via real-time heap prioritization following either FIFO (First In, First Out) or EDF (Earliest Deadline First). 
The core objective is to execute the simulation ensuring perfect data safety while preventing circular wait locks (deadlocks) and individual coder burnout (spent too long without coding).

## Instructions

### Compilation
The project compiles using a Makefile with strict compilation warning flags (-Wall -Wextra -Werror -pthread).

To cleanly compile the project run: make

### Execution
The executable must be launched from the terminal using 8 mandatory numeric and text arguments:
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>

#### Running Example (FIFO Mode)
./codexion 4 800 200 200 200 5 100 fifo

#### Running Example (EDF Mode)
./codexion 4 800 200 200 200 5 100 edf

## Blocking Cases Handled
My implementation counters classical concurrency design vulnerabilities and satisfies Coffman's dynamic interlocking conditions through the following technical designs:
- Deadlock Prevention (Breaking Coffman Conditions): To violate the Hold and Wait and Circular Wait conditions, our architecture enforces a strict all-or-nothing acquisition gateway (take_both_dongles()). A thread claims its first resource via a lock-ordered pointer comparison (first address is smaller than second address) to prevent asymmetrical lock chasing. If the secondary resource is unavailable due to queue priority or usage states, the coder immediately releases its initial held resource (release_dongle), yields back to the scheduler, and rests before re-attempting.
- Starvation Prevention: Threads executing lifestyle state transformations voluntarily sleep (usleep(1000)) right at the end of their lifecycle loop pass. This active yielding forces aggressive thread context switching, preventing super-fast threads from instantly re-claiming dongles before sleeping neighbors can check parameters.
- Cooldown Handling: When a dongle is dropped, it is stamped with a precise future availability deadline (current_time + dongle_cooldown). Arriving threads check this benchmark safely within an exclusive locked context. If the cooldown has not expired, the thread calculates the exact remaining delta and takes a microsecond nap via usleep() before joining queue tree arrays, preventing empty polling waste.
- Precise Burnout Detection: A dedicated monitoring thread (burnout_monitor) sweeps across all coder objects at ultra-short intervals. By assessing real-world millisecond timestamps (get_time() - last_compile_time > time_to_burnout), it intercepts coder starvation, logs the precise failure state, and safely terminates the system within the strict 10ms deadline constraint required by the subject.
- Log Serialization: To fully guarantee that multi-threaded outputs are never broken apart, mixed up, or interleaved on the terminal console, all printing tasks pass through a single serialized gate locked by an isolated printing mutex (print_lock).

## Thread Synchronization Mechanisms
The project infrastructure isolates shared components through standard POSIX primitives and thread-safe data flow models:

- pthread_mutex_t (Mutual Exclusion):
    - Dongle States: Individual locks (dongle->lock) wrap around every single dongle structure, safeguarding changes made to in_use status variables, cooldown counters, and heap trees.
    - Output Serialization: print_lock serializes stdout writes, forcing threads to wait sequentially so that logs print cleanly one full line at a time.
    - Simulation Control: stop_lock isolates writes made to the global shutdown flag (sim_stopped), ensuring all threads discover a termination status instantly.

- pthread_cond_t (Condition Variables):
    - Condition blocks (dongle->cond) keeps threads from aggressive polling. Instead of wasting processing cycles checking variables, threads calling pthread_cond_wait() drop their mutexes and sleep at 0% CPU footprint. They are awakened precisely when neighbor threads signal an availability change or a cooldown expiration via pthread_cond_broadcast().

- Thread-Safe Communication Model:
    When the monitor thread flags a burnout or verifies that all task compilation milestones are successfully reached, it locks stop_lock, changes sim_stopped = 1, and calls a broadcast function (wake_all_dongles) to transmit all sleeping condition gates. The sleeping coders immediately wake up, read the true system status, drop their local heap nodes, and cleanly close out their threads without leaks.

## Resources
-   POSIX Threads in OS: https://www.geeksforgeeks.org/operating-systems/posix-threads-in-os/
*   AI Usage Description: AI was used as to optimize loop configurations, to understand how OS time affects multithreading, to verify binary min-heap array index equations (heapify_up / heapify_down), and help map mathematical tie-breaker priority formulas.
