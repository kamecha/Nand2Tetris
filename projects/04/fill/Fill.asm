// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

//while true
//	if(key) {
//		screen: black
//	} else {
//		screen: white
//	}

(LOOP)
	@i
	M=0
	@KBD
	D=M
	@BLACK
	D;JNE

(WHITE)
	@i
	D=M
	@8192
	D=A-D		//while i < 32*256
	@WHITEEND
	D;JLE
	@i
	D=M
	@SCREEN
	A=A+D
	M=0			//M[@SCREEN+i] = white
	@i
	M=M+1		//i++
	@WHITE
	0;JMP
(WHITEEND)
	@LOOP
	0;JMP

(BLACK)
	@i
	D=M
	@8192
	D=A-D		//while i < 32*256
	@BLACKEND
	D;JLE
	@i
	D=M
	@SCREEN
	A=A+D
	M=-1		//M[@SCREEN+i] = black
	@i
	M=M+1		//i++
	@BLACK
	0;JMP
(BLACKEND)
	@LOOP
	0;JMP
