# C++ Performant Event Dispatcher

## Overview
A standalone C++ Event Dispatcher, designed to be thread-safe, and potentially be used inside game engines. It supports both synchronous (blocking) and deffered (queued) event processing, that focuses on allocating as little memory as possible (preferrably zero at runtime).

## Goals
* **Strict Memory Constraints:** Once the project is initialized, it should process the events without allocating memory at all during runtime.
* **Synchronous and Deferred Modes:** Support both blocking and queued events (ones that are executed immediatelly, and others that are processed at a later time)
* ***(Potentially)*** **Lock-Free Deferred Queue:** A double-buffered architecture to allow worker threads to post events without heavy mutex contention on the main thread.

## Roadmap
- [✅] Phase 1: Core Event Class and Synchronous Dispatcher 
- [✅] Phase 2: Unit Test Setup
- [✅] Phase 3: Double Buffered Queue
- [✅] Phase 4: Custom Memory Allocator
- [] Phase 5: Thread-Safety Tests

## System Architecture
**Empty at the moment**

## Building and Running Steps
**Empty at the moment**