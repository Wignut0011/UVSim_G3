<!-- language: lang-none -->
<pre>
 __ __  __ __  __ __       _____ ____  ___ ___  __ __  _       ____  ______   ___   ____  
|  T  T|  T  ||  T  T     / ___/l    j|   T   T|  T  T| T     /    T|      T /   \ |    \ 
|  |  ||  |  ||  |  |    (   \_  |  T | _   _ ||  |  || |    Y  o  ||      |Y     Y|  D  )
|  |  ||  |  ||  |  |     \__  T |  | |  \_/  ||  |  || l___ |     |l_j  l_j|  O  ||    / 
|  :  |l  :  !|  :  |     /  \ | |  | |   |   ||  :  ||     T|  _  |  |  |  |     ||    \ 
l     | \   / l     |     \    | j  l |   |   |l     ||     ||  |  |  |  |  l     !|  .  Y
 \__,_j  \_/   \__,_j      \___j|____jl___j___j \__,_jl_____jl__j__j  l__j   \___/ l__j\_j
                                                                                          

--------------------------------------------------------------------------------------------------
==================================================================================================
--------------------------------------------------------------------------------------------------

	|===================|
	|      TABLE        |
	|===================|
	|1: Functionality   |
	|   -Overview       |
	|   -Operation      |
	|   -Edit Mode      |
	|   -Execution Mode |
	|   -Instructions   |
	|___________________|

--------------------------------------------------------------------------------------------------
==================================================================================================
--------------------------------------------------------------------------------------------------
    
 1 |
___|

	OVERVIEW
	========
	
	UVSim is a Command Line based Machine Language Simulator developed as an educational
	tool for the purpose of learning Machine Language. UVSim contains 1 register nammed
	"Accumulator", 15 instructions, and 100 lines of memory. UVSim runs in 2 modes.
	Editing Mode and Execution Mode.
	

	OPERATION
	=========
	
	
	UVSim runs in 2 modes; Editing Mode and Execution Mode. When the simulator
	launches, you will be in Editing Mode. Here you will input your instructions (code)
	as signed 4 digit decimal numbers one line at a time. Please see section Editing
	Mode for further detail. When you are finished inputing your instructions, input
	-99999. This ends Editing Mode and starts Execution Mode. In Execution Mode, the 
	simulator will run the instructions you had previously input in Editing Mode then 
	display a memory dump after it's finished. 
	
	
	
	EDITING MODE
	=============
	
	
	  ________________
	  |UVUSim.exe    |
	  |--------------|
	  | 00 ? +1009   |
          | ...          |
          | 05 ? +3011_< |
            --   ----- 
            ^    ^^ ^
            1    23 4
		
	1: Address
	2: Interger sign
	3: Opcode
	4: Operand
	
	In Editing Mode, you will input your instrucions line by line starting at line 00 as shown 
	above. Each instruction contains an interger sign, opcode, and operand. The sign tells
	the simulator if the data is positive or negative. The opcode tells the simulator what
	instruction to do (a list of each opcode and a discription is listed below). The operand
	is dependent on the opcode, so refer to the instruction list for information on each
	purpose. Since the memory is limited to 100 lines, you will only be	able to input
	instructions until line 99. When you want the program to finish at a particular line, be
	sure to put a HALT instruction. To exit Editing Mode you can either fill all 99 lines
	or input -9999. Doing this will put UVSim into Execution Mode.
		
	Hint: You may want to reserve a block of lines for data, if so, you can use it as pure
	data, or you may want to leave it empty incase you want to save data during execution.
	
	CAUTION: Be aware of the arithmatic you are asking the simulator to perform. Since each
	word is limited to 4 digits, you may experience an overflow or underflow if the arithmatic
	results	in a higher/lower digit number. IE: Once it reaches the limit of +9999, it will
	wrap back to the other limit of -9999 and continue, resulting in a false answer.



	EXECUTION MODE
	==============
	
	
	In Execution Mode, the simulator will start to execute the instructions you had put into
	it's memory. Depending on your instructions, you may be prompted to input a number during
	it's runtime. Once a HALT instruction has been reached, or line 99 had been executed, 
	Execution Mode will end and a dump of the memory and each register will be printed on the
	screen. To use the simulator again, you will need to re-launch UVSim.
	
	
	LIST OF INSTRUCTIONS
	====================
	
	
	I/O Instructions:
		READ = 10               Read a word from the keyboard into a specific location in memory.
		WRITE = 11              Write a word from a specific location in memory to screen.
		
	Load/store Instructions:
		LOAD = 20               Load a word from a specific location in memory into the accumulator.
		STORE = 21              Store a word from the accumulator into a specific location in memory. 
		
	Arithmetic Instructions:
		Add = 30                Add a word from a specific location in memory to the word in the
	                                accumulator (leave the result in the accumulator)
		SUBTRACT = 31           Subtract a word from a specific location in memory from the word in the
		                        accumulator (leave the result in the accumulator)
		DIVIDE = 32             Divide the word in the accumulator by a word from a specific location in
		                        memory (leave the result in the accumulator).
		MULTIPLY = 33           multiply a word from a specific location in memory to the word in the
		                        accumulator (leave the result in the accumulator).
		
	Control Instructions:
		BRANCH = 40             Branch to a specific location in memory
		BRANCHNEG = 41          Branch to a specific location in memory if the accumulator is negative. 
		BRANCHZERO = 42         Branch to a specific location in memory if the accumulator is zero.
		HALT = 43               Pause the program
			
	
--------------------------------------------------------------------------------------------------
==================================================================================================
--------------------------------------------------------------------------------------------------
