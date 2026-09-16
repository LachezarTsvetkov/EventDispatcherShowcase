## 📝 Description
[Explain what this PR is]

## ✅ Pre-Merge Checklist
*Check all the steps that have been completed:*
**Build & Run**
- [] Code compiles completely without warnings on MSVC/GCC.
- [] The Sandbox application runs as expected without crashing.

**Memory & Performance**
- [] **Cleanup:** Any pointers queued in the deferred systems are properly deleted to prevent memory leaks.
- [] Passed a memory profiler check (not tested) 

**Testing & Quality**
- [] Unit tests have been written or updated for this feature.
- [] All unit tests pass successfully.
- [] Checked for thread-safety issues (e.g., data races in the double-buffered queue).