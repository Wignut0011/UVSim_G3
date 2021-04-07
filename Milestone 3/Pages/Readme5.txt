=======================================================================================================================
|     _   ___   __  ___ ___ __  __                                                                                    |
|    | | | \ \ / / / __|_ _|  \/  |          ====================                                                     |
|    | |_| |\ V /  \__ \| || |\/| |          List of Instructions                                                     |
|     \___/  \_/   |___|___|_|  |_|          ====================                                                     |
|       I/O Instructions:                                                                                             |
|               READ = 10               Read a word from the keyboard into a specific location in memory.             |
|               WRITE = 11              Write a word from a specific location in memory to screen.                    |
|       Load/store Instructions:                                                                                      |
|               LOAD = 20               Load a word from a specific location in memory into the accumulator.          |
|               STORE = 21              Store a word from the accumulator into a specific location in memory.         |
|       Arithmetic Instructions:                                                                                      |
|               Add = 30                Add a word from a specific location in memory to the word in the              |
|                                       accumulator (leave the result in the accumulator)                             |
|               SUBTRACT = 31           Subtract a word from a specific location in memory from the word in the       |
|                                       accumulator (leave the result in the accumulator)                             |
|               DIVIDE = 32             Divide the word in the accumulator by a word from a specific location in      |
|                                       memory (leave the result in the accumulator).                                 |
|               MULTIPLY = 33           multiply a word from a specific location in memory to the word in the         |
|                                       accumulator (leave the result in the accumulator).                            |
|       Control Instructions:                                                                                         |
|               BRANCH = 40             Branch to a specific location in memory                                       |
|               BRANCHNEG = 41          Branch to a specific location in memory if the accumulator is negative.       |
|               BRANCHZERO = 42         Branch to a specific location in memory if the accumulator is zero.           |
|               HALT = 43               Pause the program                                                             |
|     Choose an option:                                                                                               |
|     1: Main Menu                                                                                                    |
|     2: Previous Page                                                                                                |
=======================================================================================================================