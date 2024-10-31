# C Program to Calculate ln(1 + x) Using the Mercator Series

This C program computes the value of ln(1 + x) using the Mercator series. It utilizes multiple processes to parallelize the computation, demonstrating both a version that employs semaphores for synchronization and a version without them for performance comparison.

## Table of Contents

- [Overview](#overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Performance Comparison](#performance-comparison)
- [License](#license)

## Overview

The program computes the sum of the first `SERIES_MEMBER_COUNT` members of the Mercator series, which can be described as:

ln(1 + x) = Σ [(-1)^n * x^(n+1)] / (n + 1) from n = 0 to ∞

### Features

- Multi-process computation to improve performance.
- Uses shared memory for communication between processes.
- Synchronization managed with semaphores (in the semaphore version).

## Requirements

- GCC or any C compiler.
- Linux-based operating system for process management and IPC.
- The `math.h` library for logarithmic calculations.

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/Hamibubu/MercatorWSemaphores.git
   cd MercatorWSemaphores
   ```

2. Compile the program:
   ```bash
   make
   ```

## Usage

To run the program, execute the compiled binary:

```bash
./sems OR ./nosems
```

The program will output the computed value of ln(1 + x) and the execution time.

### Example Output

```
------------------------------------------------------------------------
| [i] The count of ln(1 + x) members of the Mercator series is 200000 |
| [i] The value of the argument x is 1.000000                         |
------------------------------------------------------------------------

--- [!] Time: 2 seconds
--- [+] The result: 0.69314718
--- [+] Calling function ln(1 + 1.000000) = 0.69314718
```

## Performance Comparison

The program has two versions: one that uses semaphores for process synchronization and another that does not.

- **Semaphore Version**: Ensures that processes synchronize their start and completion, which may introduce some overhead but is necessary for certain applications requiring synchronization.
  
- **No Semaphore Version**: Attempts to minimize overhead by removing semaphore usage, potentially leading to faster execution times but may result in race conditions if not carefully managed.