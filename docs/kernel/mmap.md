## How memory is managed in CeanOS:

| Address          | Region            |
|------------------|-------------------|
| 0xFFFFFFFF       | User Space (top)  |
| ...              |                   |
| 0xC0000000       | Kernel Space      |
| ...              |                   |
| 0x00000000       | User Space (bottom)
|