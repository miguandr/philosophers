# Philosophers

A concurrent simulation of the classic **Dining Philosophers Problem**, implemented in C using POSIX threads and mutexes. Built as part of the 42 School core curriculum.

---

## What is this?

Five philosophers sit around a table. Between each pair is a single fork. A philosopher needs **two forks** to eat. After eating, they sleep. After sleeping, they think. If a philosopher goes too long without eating, they die.

The challenge is purely about **concurrency**: preventing deadlocks (everyone waits forever), starvation (one philosopher never gets to eat), and data races (shared state accessed without synchronization).

---

## Build & Run

```bash
git clone https://github.com/miguandr/philosophers.git
cd philosophers

make
./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [num_times_to_eat]
```

All time values are in **milliseconds**.

**Examples:**

```bash
# 5 philosophers, no one should die
./philo 5 800 200 200

# 3 philosophers, constrained timing — tests race condition handling
./philo 3 610 200 100

# Simulation ends after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: single philosopher always dies (only one fork available)
./philo 1 800 200 200
```

**Constraints enforced by the validator:**
- `num_philosophers`: 1 – 200
- All time values: ≥ 60ms (below this, OS scheduling variance makes correctness impossible to guarantee)

**Makefile targets:**

| Target   | Description                        |
|----------|------------------------------------|
| `make`   | Compile the binary (`philo`)       |
| `make clean` | Remove object files (`obj/`)   |
| `make fclean` | Remove objects + binary       |
| `make re` | Full recompile from scratch       |

---

## Architecture

```
main.c              entry point, argument validation, lifecycle
├── check.c         input validation (types, ranges)
├── init.c          data structure setup, fork assignment, mutex init
├── simulation.c    philosopher thread logic (eat / sleep / think loop)
├── simulation_utils.c  get_time(), ft_usleep(), is_dead(), print_status()
├── observer.c      dedicated monitor thread (death detection, meal counting)
├── handle_mutex_thread.c  mutex/thread wrappers with full error handling
├── error.c         error message registry
└── utils.c         ft_atol(), ft_putstr_fd()
```

### Core data model

```
t_data                          — shared simulation state
  ├── t_philo[]                 — per-philosopher state (thread, meals, last_meal)
  ├── t_mtx forks[]             — one mutex per fork
  ├── t_mtx dead_lock           — guards dead_flag (read/written from multiple threads)
  └── t_mtx print_lock          — serializes stdout output

t_philo
  ├── right_fork / left_fork    — pointers into the shared forks array
  ├── philo_mtx                 — guards last_meal and meals_eaten
  └── eating flag               — prevents observer from declaring death mid-eat
```

### Concurrency model

Each philosopher runs in its own `pthread`. A separate **observer thread** monitors all philosophers for death or completion — it never prints anything itself, it only sets `dead_flag` and signals the end.

**Deadlock prevention:** philosophers with an even `id` lock the right fork first; odd ones lock left first. This breaks the circular wait that would otherwise form.

**Race condition mitigation in `dinner_simulation`:** after sleeping, each thread waits an additional `think_time` before competing for forks again:

```c
think_time = (time_to_die - time_to_eat - time_to_sleep) / 2;
```

This gives other philosophers a window to acquire forks, distributing access more evenly under tight timing constraints.

**Thread-safe printing:** `print_status()` locks `print_lock` and rechecks `dead_flag` before writing — this prevents death messages from printing out of order after the simulation has already ended.

---

## My Role

I built this project solo from scratch. Every design decision, the observer pattern, the per-philosopher mutex, the fork-ordering strategy for deadlock prevention, the `think_time` heuristic was researched, implemented, and debugged independently.

- Getting `last_meal` initialization right so the observer doesn't falsely trigger at t=0
- Ensuring the single-philosopher case (`./philo 1 ...`) correctly waits and dies without hanging
- Tuning the observer's polling interval (10ms) to be responsive without burning CPU

---

## Skills Demonstrated

- **POSIX threads** — `pthread_create`, `pthread_join`, thread lifecycle management
- **Mutex synchronization** — fine-grained locking to protect shared state without over-locking
- **Deadlock analysis** — identifying and breaking circular wait conditions
- **Race condition reasoning** — knowing *what* to protect and *why*, not just wrapping everything in locks
- **Custom timing** — `ft_usleep` built on `gettimeofday` for sub-millisecond accuracy on top of a busy-wait loop
- **Defensive error handling** — every `pthread_*` and `pthread_mutex_*` call checked and surfaced through a unified error registry
- **C without stdlib conveniences** — custom `ft_atol`, no dynamic allocation beyond what's needed
