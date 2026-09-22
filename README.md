# C++ Performant Event Dispatcher

## Overview
A high-performance, standalone C++ Event Dispatcher engineered for strict real-time constraints. It supports both blocking (synchronous) and deferred (queued) event routing. The architecture utilizes custom memory arenas, placement new, and perfect forwarding to guarantee strictly zero OS-level heap allocations during the main loop, preventing frame-time spikes caused by memory fragmentation.

## Goals
* **Zero-Allocation Runtime**: Implements a custom LinearAllocator and EventFactory to bypass the OS heap, constructing deferred events directly into pre-allocated contiguous memory blocks.
* **MPSC Thread Safety**: Utilizes a Multi-Producer, Single-Consumer concurrency model. Background worker threads can aggressively queue events without triggering race conditions, while a scope-locked index swap allows the main thread to safely consume events without bottlenecking producer throughput.
* **O(1) Buffer Swapping**: Employs a double-buffered std::vector architecture. By alternating read/write indices, the system instantly recycles memory block offsets at the end of each frame without invoking standard destructors or memory reallocations.

## Roadmap
- [✅] Phase 1: Core Event Class and Synchronous Dispatcher 
- [✅] Phase 2: Unit Test Setup
- [✅] Phase 3: Double Buffered Queue
- [✅] Phase 4: Custom Memory Allocator
- [✅] Phase 5: Thread-Safety Integration and Tests

## System Architecture

The dispatcher relies on a carefully orchestrated pipeline to avoid OS-level heap allocations and thread locking during high-frequency event emission. The architecture is split into three core mechanics:

### 1. The Memory Pipeline (Linear Allocator)
Standard event systems rely on `std::make_unique` or `new`, which forces the operating system to search the global heap for free memory, resulting in cache misses and non-deterministic frame times. 

This system pre-allocates two contiguous byte buffers during engine initialization. When an event is queued, a custom `LinearAllocator` simply increments a pointer offset (bump allocation) to claim memory in just a few CPU instructions. 

### 2. Emplacement & Placement New
Instead of passing fully constructed objects or pointers into the queue, the dispatcher utilizes an emplace-style API (e.g., `QueueEvent<MouseMovedEvent>(x, y)`). 
* **Perfect Forwarding:** Variadic templates (`Args&&...`) capture the event parameters and forward them directly into the memory pool without temporary copies.
* **Placement New:** The internal `EventFactory` uses placement new (`new (memory) T(...)`) to execute the event's constructor at the specific address provided by the `LinearAllocator`, completely bypassing the standard OS allocation process.

### 3. Concurrency & The Double-Buffer (MPSC)
The system operates on a Multi-Producer, Single-Consumer (MPSC) concurrency model, supported by a double-buffered `std::vector` system.
* **Write Phase (Background Threads):** Multiple worker threads can safely push events into the active "Write" buffer simultaneously, protected by a lightweight `std::mutex`.
* **Read Phase (Main Thread):** When `ExecuteQueuedEvents` is invoked on the main loop, the system locks the mutex just long enough to swap the read/write integer index (an O(1) operation). 
* **Lock-Free Execution:** The main thread iterates through the newly designated "Read" buffer entirely lock-free, while background systems continue aggressively pushing data into the new "Write" buffer.

### 4. Custom Destruction
Because the objects reside in a raw, custom memory arena, they cannot be deleted through standard garbage collection or OS calls. The queue wraps every event in a custom `EventPtr` (a `std::unique_ptr` paired with a specialized `EventDeleter`). 

When the frame ends and the read buffer is cleared, the deleter manually invokes the event's virtual destructor (`event->~Event()`) to safely resolve complex internal data types. Finally, the allocator simply resets its integer offset to `0`, instantly recycling the raw memory block for the next frame without ever triggering a standard `delete`.

## Building and Running Steps

1. **Clone the repository:**
   ```bash
   git clone https://github.com/LachezarTsvetkov/EventDispatcherShowcase.git
   cd EventDispatcher
   ```
2. **Initialize and update submodules:**
    ```bash
    git submodule update --init --recursive
    ```
3. **Generate the Solution Files:**
Run the GenerateProjects.bat script located in the root directory. This uses Premake5 to generate the Visual Studio .sln and .vcxproj files tailored to your local environment.

4. **Build the Solution:**
Open the generated EventDispatcher.sln in Visual Studio. Select your desired configuration (Debug, Development, or Release) and build the entire solution (Ctrl + Shift + B).

5. **Run the Applications:**
The solution contains the core static library alongside two executable projects. To run them, right-click either SandboxApp or UnitTests in the Solution Explorer, select Set as Startup Project, and press F5 (or Ctrl + F5 to run without the debugger).