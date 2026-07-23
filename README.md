*This project has been created as part of the 42 curriculum by dievarga.*

## Description
**Codexion** is a real-time multi-threaded simulation modeling resource contention and process synchronization within a shared environment. In this project, multiple concurrent coder threads sit in a circularinclusive inclusive inclusiveinclusive inclusive inclusive workspace layout where they alternatively compile quantum code, debug, or refactor. 

To start a compilation phase, a coder must simultaneously claim exclusive ownership over exactly two shared physical hardware development dongles—specifically the one on their left and the one on their right. Since dongles are a scarce, limited resource shared between immediate desk neighbors, threads must carefully coordinate access via real-time min-heap prioritization queues following either FIFO (First In, First Out) or EDF (Earliest Deadline First) scheduling policies. The core objective is to execute the simulation concurrently, ensuring perfect data safety while preventing circular wait locks (deadlocks) and individual coder burnout thresholds.

## Instructions

### Compilation
The project compiles strictly into a production binary using a 42-compliant `Makefile` with strict compilation warning flags (`-Wall -Wextra -Werror -pthread`).

To cleanly compile the project source structures from scratch, run:
```bash
make re
```

### Execution
The generated executable must be launched from the terminal prompt using 8 mandatory numeric and text arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Running Example (FIFO Mode)
```bash
./codexion 4 800 200 200 200 5 100 fifo
```

#### Running Example (EDF Mode)
```bash
./codexion 4 800 200 200 200 5 100 edf
```

## Blocking Cases Handled
Our implementation aggressively counters classical concurrency design vulnerabilities and satisfies Coffman's dynamic interlocking conditions through the following technical designs:

*   **Deadlock Prevention (Breaking Coffman Conditions):** To violate the **Hold and Wait** and **Circular Wait** conditions, our architecture enforces a strict all-or-nothing acquisition gateway (`take_both_dongles`) [source: 2]. A thread claims its first resource via a lock-ordered pointer comparison (`first` address is smaller than `second` address) to prevent asymmetrical lock chasing [source: 2]. If the secondary resource is unavailable due to queue priority or usage states, the coder immediately releases its initial held resource (`release_dongle`), yields back to the scheduler, and rests before re-attempting [source: 2].
*   **Starvation Prevention:** Threads executing lifestyle state transformations voluntarily sleep (`usleep(1000)`) right at the end of their lifecycle loop pass [source: 7]. This active yielding forces aggressive thread context switching, preventing super-fast threads from instantly re-claiming dongles before sleeping neighbors can check parameters [source: 7].
*   **Cooldown Handling:** When a dongle is dropped, it is stamped with a precise future availability deadline (`current_time + dongle_cooldown`) [source: 1]. Arriving threads check this benchmark safely within an exclusive locked context [source: 2]. If the cooldown has not expired, the thread calculates the exact remaining delta and takes a microsecond nap via `usleep` before joining queue tree arrays, preventing empty polling waste [source: 2].
*   **Precise Burnout Detection:** A dedicated referee monitoring thread (`burnout_monitor`) sweeps across all coder objects at ultra-short intervals [source: 7]. By assessing real-world millisecond timestamps (`get_time() - last_compile_time > time_to_burnout`), it intercepts coder starvation, logs the precise failure state, and safely terminates the system within the strict 10ms deadline constraint required by the subject [0.1.10, source: 7].
*   **Log Serialization:** To fully guarantee that multi-threaded outputs are never broken apart, mixed up, or interleaved on the terminal console, all printing tasks pass through a single serialized gate locked by an isolated printing mutex (`print_lock`) [0.1.10, source: 8].

## Thread Synchronization Mechanisms
The project infrastructure isolates shared components through standard POSIX primitives and thread-safe data flow models:

*   **`pthread_mutex_t` (Mutual Exclusion):**
    *   **Dongle States:** Individual locks (`dongle->lock`) wrap around every single dongle structure, safeguarding changes made to `in_use` status variables, cooldown counters, and heap trees [source: 2, source: 4].
    *   **Output Serialization:** `print_lock` serializes stdout writes, forcing threads to wait sequentially so that logs print cleanly one full line at a time [source: 4, source: 8].
    *   **Simulation Control:** `stop_lock` isolates writes made to the global shutdown wire flag (`sim_stopped`), ensuring all threads discover a termination status instantly [source: 7, source: 8].
*   **`pthread_cond_t` (Condition Variables):**
    *   Condition blocks (`dongle->cond`) decouple threads from aggressive polling [source: 2, source: 4]. Instead of wasting processing cycles checking variables, threads calling `pthread_cond_wait` drop their mutexes and sleep at 0% CPU footprint [source: 2]. They are awakened precisely when neighbor threads signal an availability change or a cooldown expiration via `pthread_cond_broadcast` [source: 1, source: 2].
*   **Thread-Safe Communication Model:**
    When the monitor thread flags a burnout or verifies that all task compilation milestones are successfully reached, it locks `stop_lock`, changes `sim_stopped = 1`, and calls a broadcast function (`wake_all_dongles`) to buzz all sleeping condition gates [source: 7, source: 8]. The sleeping coders immediately wake up, read the true system status, drop their local heap nodes, and cleanly close out their threads without leaks [source: 2, source: 4, source: 7].

## Resources
*   *POSIX Threads Programming Guide* (Lawrence Livermore National Laboratory)
*   *Operating Systems: Three Easy Pieces* (Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau)
*   *The Open Group Base Specifications Issue 7 - pthread documentation*
*   **AI Usage Description:** AI was leveraged as an interactive pair-programming assistant during development. It was used strictly to optimize loop configurations under strict 42 Norm line count restrictions, verify binary min-heap array index equations (`heapify_up` / `heapify_down`), and help map mathematical tie-breaker priority formulas within standard C layouts [0.1.5, source: 6]. Every piece of generated validation structure was thoroughly reviewed, refactored, and tested manually to ensure complete conceptual understanding.
