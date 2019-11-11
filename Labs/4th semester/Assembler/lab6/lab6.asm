AStack SEGMENT STACK
	dw 64 dup (?)
AStack ENDS
;-------------------------------------------------- 
CODE SEGMENT
			ASSUME CS:CODE,  DS:DATA,  SS:AStack
;-------------------------------------------------- 
DATA SEGMENT
			FREE_ERROR7		db	'Control memory block destroyed',0Dh, 0Ah,'$'
			FREE_ERROR8		db	'Not enough memory to perform the function',0Dh, 0Ah,'$'
			FREE_ERROR9		db	'Invalid memory address',0Dh, 0Ah,'$'

			KEEP_SS			dw ?
			KEEP_SP			dw ?

			PARAMETERS  	dw 0
							dd ?
							dd ?
							dd ?

			PATH			db 40h dup (0)

			EXEC_ERROR_1 	db 'Wrong function number.',0Dh, 0Ah,'$'	
			EXEC_ERROR_2	db 'File is not found.',0Dh, 0Ah,'$'
			EXEC_ERROR_5	db 'Disk error.',0Dh, 0Ah,'$'
			EXEC_ERROR_8	db 'Memory is not enough.',0Dh, 0Ah,'$'
			EXEC_ERROR_10	db 'Wrong environment line',0Dh, 0Ah,'$'
			EXEC_ERROR_11	db 'Wrong format.',0Dh, 0Ah,'$'
			
			EXEC_END		db 'Exit code is $'
			EXEC_END_0		db 'Normal termination.',0Dh, 0Ah,'$'
			EXEC_END_1		db 'Termination via Ctrl-Break.',0Dh, 0Ah,'$'
			EXEC_END_2		db 'Termination via device error.',0Dh, 0Ah,'$'
			EXEC_END_3		db 'Termination via resident.',0Dh, 0Ah,'$'
DATA ENDS
;-------------------------------------------------- 
TETR_TO_HEX PROC far
			and AL, 0Fh
			cmp AL, 09
			jbe NEXT
			add AL, 07
		NEXT: add AL, 30h
			ret
TETR_TO_HEX ENDP
;-------------------------------------------------- 
BYTE_TO_HEX PROC far
			push CX
			mov AH, AL
			call TETR_TO_HEX
			xchg AL, AH
			mov CL, 4
			shr AL, CL
			call TETR_TO_HEX
			pop CX
			ret
BYTE_TO_HEX ENDP
;-------------------------------------------------- 
WRD_TO_HEX PROC far
			push AX
			push BX
			mov BH, AH
			call BYTE_TO_HEX
			mov [DI],AH
			dec DI
			mov [DI],AL
			dec DI
			mov AL, BH
			call BYTE_TO_HEX
			mov [DI],AH
			dec DI
			mov [DI],AL
			pop BX
			pop AX
			ret
WRD_TO_HEX ENDP
;-------------------------------------------------- 
PRINT PROC far
			push AX
			mov AH, 09h
			int 21h
			pop AX
			ret
PRINT ENDP
;-------------------------------------------------- 
FREE_EXCESS PROC far
			push AX
			push BX
			push CX
			push DX
			
			mov BX, offset END_OF_PROGRAM
			mov AH, 4Ah
			int 21h
			jnc free_success

			call WRD_TO_HEX
			cmp AX, 7
			je error7
			cmp AX, 8
			je error8
			cmp AX, 9
			je error9

		error7:
			mov DX, offset FREE_ERROR7
			call PRINT
			jmp exit
		error8:
			mov DX, offset FREE_ERROR8
			call PRINT
			jmp exit
		error9:
			mov DX, offset FREE_ERROR9
			call PRINT
			jmp exit
		free_success:
			pop DX
			pop CX
			pop BX
			pop AX
			ret
FREE_EXCESS ENDP
;-------------------------------------------------- 
BLOCK_FORMATION PROC far
			push AX
			mov AX, ES
			mov [PARAMETERS+2], 80h
			mov [PARAMETERS+4], AX
			mov [PARAMETERS+6], 5Ch
			mov [PARAMETERS+8], AX
			mov [PARAMETERS+10], 6Ch
			mov [PARAMETERS+12], AX
			pop AX
			ret
BLOCK_FORMATION ENDP
;-------------------------------------------------- 
PREPARE_PATH PROC far
			push SI
			push BX
			push DI
			push DX
			xor SI, SI
			mov BX, ES:[2Ch]
			mov ES, BX
			
			env_loop:
				inc SI
				cmp WORD PTR ES:[SI], 0000h
				je gotopath
				cmp BYTE PTR ES:[SI], 00h
				je linenew
				jmp env_loop
			linenew:
				inc SI
				jmp env_loop
			
			gotopath:
				add SI, 4
				xor DI, DI
				path_loop:
					mov DL, ES:[SI]
					mov PATH[DI], DL
					inc SI
					inc DI
					cmp BYTE PTR ES:[SI], 00h
					jne path_loop
					
			sub DI, 8
			mov PATH[DI], 'l'
			mov PATH[DI+1], 'a'
			mov PATH[DI+2], 'b'
			mov PATH[DI+3], '2'
			mov PATH[DI+4], '.'
			mov PATH[DI+5], 'c'
			mov PATH[DI+6], 'o'
			mov PATH[DI+7], 'm'
					
			pop SI
			pop DX
			pop BX
			pop DI
			ret
PREPARE_PATH ENDP
;-------------------------------------------------- 
EXECUTION PROC far
			push DS
			mov KEEP_SS,SS
			mov KEEP_SP,SP
			
			mov AX, seg DATA
			mov ES, AX
			mov DX, offset PATH
			mov BX, offset PARAMETERS
			mov DS, AX
			mov AX,4B00h
			int 21h
			
			mov SS, KEEP_SS
			mov SP, KEEP_SP
			pop DS
			
			jnc exec_success
			call WRD_TO_HEX
			
			cmp AX, 1
			je err1
			cmp AX, 2
			je err2
			cmp AX, 5
			je err5
			cmp AX, 8
			je err8
			cmp AX, 10
			je err10
			cmp AX, 11
			je err11
			jmp exit

		err1:
			mov DX, offset EXEC_ERROR_1
			jmp prt
		err2:
			mov DX, offset EXEC_ERROR_2
			jmp prt
		err5:
			mov DX, offset EXEC_ERROR_5
			jmp prt
		err8:
			mov DX, offset EXEC_ERROR_8
			jmp prt
		err10:
			mov DX, offset EXEC_ERROR_10
			jmp prt
		err11:
			mov DX, offset EXEC_ERROR_11
			jmp prt
			
		exec_success:
			mov AH, 4Dh
			int 21h
			
			cmp AH, 0
			jmp out0
			cmp AH, 1
			jmp out1
			cmp AH, 2
			jmp out2
			cmp AH, 3
			jmp out3

		out0:
			mov DX, offset EXEC_END_0
			jmp prt
		out1:
			mov DX, offset EXEC_END_1
			jmp prt
		out2:
			mov DX, offset EXEC_END_2
			jmp prt
		out3:
			mov DX, offset EXEC_END_3

		prt:
			call PRINT
			mov DX, offset EXEC_END
			call PRINT
			call BYTE_TO_HEX
			push AX
			mov AH, 02h
			mov DL, AL
			int 21h
			pop AX
			xchg AH, AL
			mov AH, 02h
			mov DL, AL
			int 21h
			ret
EXECUTION ENDP

MAIN PROC far
			mov AX, seg DATA
			mov DS, AX

			call FREE_EXCESS
			call PREPARE_PATH
			call EXECUTION

		exit:	
			xor AL, AL
			mov AH, 4Ch
			int 21h
			ret
MAIN ENDP
END_OF_PROGRAM:
CODE ENDS

END MAIN 