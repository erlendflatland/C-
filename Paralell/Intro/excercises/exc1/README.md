# C++ Threading Exercises

## Exercise 1: Simulating and Resolving Deadlock

### 1. Implementation
- Create a program with two threads.
- Each thread needs to acquire two locks (mutex A and mutex B), but in a different order:
  - Thread 1 acquires `A` first, then `B`.
  - Thread 2 acquires `B` first, then `A`.

### 2. Observing the Problem
- Run the program to observe the deadlock scenario where both threads are waiting for each other to release a lock.

### 3. Instruction/Hint for Solution
- Modify the code so that both threads acquire the locks in the same order (either `A` then `B` or `B` then `A`) to prevent the deadlock.

### 4. Expected Outcome
- Initially, the threads should be in a deadlock state.
- After the change, the threads should be able to proceed without deadlock.

---

## Exercise 2: Using `std::scoped_lock` to Prevent Deadlock

### 1. Implementation
- Create a program with two mutexes, `mutex1` and `mutex2`.
- Implement two threads:
  - Thread 1 locks `mutex1` and then `mutex2`.
  - Thread 2 locks `mutex2` and then `mutex1`.

### 2. Observing the Problem
- Run the program and observe the deadlock scenario.

### 3. Instruction/Hint for Solution
- Replace the manual locking mechanism with `std::scoped_lock` to prevent the deadlock automatically.

### 4. Expected Outcome
- Initially, there should be a deadlock.
- After using `std::scoped_lock`, the deadlock should be resolved, and the program should run without blocking.

---

## Exercise 3: Using `try_lock` with Timeout

### 1. Implementation
- Create a program that contains one mutex and two threads.
- Thread 1 should try to lock the mutex using `try_lock()` and print `"Attempting to lock mutex..."` if it succeeds, otherwise print `"Mutex is locked by another thread"`.
- Thread 2 should lock the mutex and hold it for a short time (e.g., 3 seconds).
- Thread 1 should retry locking the mutex after waiting for 1 second if `try_lock()` fails. It should do this for a total of 5 attempts.

### 2. Observing the Problem
- Watch how Thread 1 behaves when `try_lock()` fails and retries after the specified time.

### 3. Instruction/Hint for Solution
- Compare the behavior of `try_lock` and `lock()` by replacing `try_lock` with `lock()` and observing the difference (i.e., what happens when you use `lock()` instead of `try_lock()`).

### 4. Expected Outcome
- Thread 1 should repeatedly attempt to lock the mutex and print failure messages while Thread 2 holds the lock.
- The program should exit when Thread 1 successfully locks the mutex.

---

## Exercise 4: Shared Mutex for Read/Write Locks

### 1. Implementation
- Create a shared resource that a thread will both read and write to.
- Use `std::shared_mutex` to ensure that:
  - Multiple threads can read the resource simultaneously.
  - Only one thread can write to the resource at a time.
- Write one thread that constantly reads the shared resource and another thread that writes to it every 3 seconds.

### 2. Observing the Problem
- Demonstrate that while the writer thread is writing, no reader threads can access the resource.

### 3. Instruction/Hint for Solution
- Use `std::unique_lock` for writing threads and `std::shared_lock` for reading threads to control access.

### 4. Expected Outcome
- Multiple threads should be able to read the resource concurrently.
- When the writer thread locks the resource, readers should be blocked until the writer releases the lock.

---

## Exercise 5: Simulating Starvation and Solving It

### 1. Implementation
- Create a program with two threads and one shared resource, protected by a mutex.
- Implement the first thread to continually lock the mutex and perform work, while the second thread should try to acquire the mutex to perform work but is unable to get it.

### 2. Observing the Problem
- Demonstrate starvation by making the second thread wait indefinitely while the first thread always locks the mutex.

### 3. Instruction/Hint for Solution
- Solve the starvation issue by introducing a fairness mechanism using `std::condition_variable` or a simple strategy (e.g., alternating lock attempts by the threads).

### 4. Expected Outcome
- Initially, Thread 2 should be starved and unable to acquire the lock.
- After introducing fairness (e.g., alternating or using a condition variable), both threads should have equal opportunity to access the shared resource.

---

## Exercise 6: Livelock Simulation and Solution

### 1. Implementation
- Write a program with two threads that each attempt to acquire a lock on a shared resource.
- Simulate a livelock where both threads continually release and reattempt to acquire the lock when the other thread also tries to acquire it.

### 2. Observing the Problem
- Demonstrate a livelock where both threads are always active but never make progress.

### 3. Instruction/Hint for Solution
- Introduce a back-off mechanism where each thread waits a random amount of time before retrying the lock, reducing the livelock and allowing progress.

### 4. Expected Outcome
- The initial implementation should demonstrate a livelock where both threads are continuously active but make no progress.
- After implementing the back-off strategy, the threads should eventually acquire the lock and make progress.

---

## Exercise 7: Abandoning a Lock

### 1. Implementation
- Create a program where a thread tries to acquire a lock using `try_lock()`.
- If the thread fails to acquire the lock, it should abandon the attempt after 2 seconds and print a message like `"Abandoning lock attempt"` and proceed with other tasks.
- Add a second thread that successfully locks the mutex after Thread 1 abandons the lock.

### 2. Observing the Problem
- Demonstrate how `try_lock()` differs from `lock()` in terms of blocking and abandonment.

### 3. Instruction/Hint for Solution
- `try_lock()` should be used to attempt locking the mutex without blocking.
- Use `std::this_thread::sleep_for()` to simulate the abandonment of the lock attempt.

### 4. Expected Outcome
- Thread 1 should attempt to lock the mutex and abandon the attempt if it fails within 2 seconds.
- Thread 2 should be able to acquire the lock after Thread 1 abandons it.
