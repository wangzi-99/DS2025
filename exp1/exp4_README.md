# exp3 vs exp4 Implementation Differences

## Original Implementation (exp3.cpp)
- Uses an optimized approach with jump pointers
- Sets `l[0] = -1` and `r[n-1] = n` as boundary conditions
- Uses the technique where `l[i]` stores the leftmost index that is lower than `h[i]`
- Implements optimization by jumping using stored values: `while (t >= 0 && h[t] >= h[i]) t = l[t];`
- This approach can potentially cause issues with custom vector implementations due to negative index access

## Modified Implementation (exp4.cpp) 
- Uses a standard stack-based approach for finding boundaries
- Maintains the same time complexity O(n) but with clearer logic
- Uses explicit stack (implemented as array) to find left and right boundaries
- Avoids potential negative index access issues
- More robust with custom vector implementations
- Functionally equivalent but structurally different

## Key Algorithm Changes
1. **Left boundary calculation**: Stack-based instead of jump-based
2. **Right boundary calculation**: Stack-based instead of jump-based  
3. **Memory management**: Uses arrays to simulate stack to avoid vector issues
4. **Safety**: Eliminates potential access to negative indices

## Functionality
Both implementations calculate the largest rectangular area in a histogram correctly.
The time complexity remains O(n) and space complexity is O(n).