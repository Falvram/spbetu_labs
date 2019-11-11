AStack SEGMENT STACK
	dw 64 dup (?)
AStack ENDS
;-------------------------------------------------- 
CODE SEGMENT
	ASSUME CS:CODE,   DS:DATA,   SS:AStack
;-------------------------------------------------- 
DATA SEGMENT
	FREE_ERROR7		db	'Control memory block destroyed',0Dh,  0Ah,'$'
	FREE_ERROR8		db	'Not enough memory to perform the function',0Dh,  0Ah,'$'
	FREE_ERROR9		db	'Invalid memory address',0Dh,  0Ah,'$'

	KEEP_IND		dw ?
	KEEP_PSP		dw ?
	ADDRESS_CALL    dd ?

	PATH			db 128 dup (0)
	DTA				db 43 dup (?),'$'
	LOAD_EPB		dw ?

	SIZE_ERR		db 'Size definition error:     ',0Dh, 0Ah,'$'
	SIZE_ERR_2		db 'Overlay file is not found.',0Dh, 0Ah,'$'
	SIZE_ERR_3		db 'Overlay path is not found.',0Dh, 0Ah,'$'
	SIZE_ERR_12		db 'There was no overlay file found.',0Dh, 0Ah,'$'
	
	EXEC_ERROR		db 'EXEC error is     ',0Dh, 0Ah,'$'
	EXEC_ERROR_1 	db 'Wrong function number.',0Dh, 0Ah,'$'	
	EXEC_ERROR_2	db 'File is not found.',0Dh, 0Ah,'$'
	EXEC_ERROR_3	db 'Path is not found.',0Dh, 0Ah,'$'
	EXEC_ERROR_4	db 'Too many opened files.',0Dh, 0Ah,'$'
	EXEC_ERROR_5	db 'Permission denied.',0Dh, 0Ah,'$'
	EXEC_ERROR_8	db 'Memory is not enough.',0Dh, 0Ah,'$'
	EXEC_ERROR_10	db 'Wrong environment.',0Dh, 0Ah,'$'
DATA ENDS
;-------------------------------------------------- 
TETR_TO_HEX PROC far
	and AL,  0Fh
	cmp AL,  09
	jbe NEXT
	add AL,  07
NEXT: add AL,  30h
	ret
TETR_TO_HEX ENDP
;-------------------------------------------------- 
BYTE_TO_HEX PROC far
	push CX
	mov AH,  AL
	call TETR_TO_HEX
	xchg AL,  AH
	mov CL,  4
	shr AL,  CL
	call TETR_TO_HEX
	pop CX
	ret
BYTE_TO_HEX ENDP
;-------------------------------------------------- 
WRD_TO_HEX PROC far
	push AX
	push BX
	mov BH,  AH
	call BYTE_TO_HEX
	mov [DI],AH
	dec DI
	mov [DI],AL
	dec DI
	mov AL,  BH
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
	mov AH,  09h
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

	mov BX,  offset END_OF_PROGRAM
	mov AH,  4Ah
	int 21h
	jnc free_success

	call WRD_TO_HEX
	cmp AX,  7
	je error7
	cmp AX,  8
	je error8
	cmp AX,  9
	je error9
			
error7:
	mov DX,  offset FREE_ERROR7
	call PRINT
	jmp exit
error8:
	mov DX,  offset FREE_ERROR8
	call PRINT
	jmp exit
error9:
	mov DX,  offset FREE_ERROR9
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
PREPARE_PATH PROC far
	push SI
	push BX
	push DI
	push DX
	xor SI,  SI
	mov BX,  ES:[2Ch]
	mov ES,  BX
			
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
		add SI,  4
		xor DI,  DI
		path_loop:
			mov DL,  ES:[SI]
			mov PATH[DI], DL
			inc SI
			inc DI
			cmp BYTE PTR ES:[SI], 00h
			jne path_loop

	sub DI,  8
	mov PATH[DI], 'o'
	mov PATH[DI+1], 'v'
	mov PATH[DI+2], 'l'
	mov PATH[DI+3], '1'
	mov PATH[DI+4], '.'
	mov PATH[DI+5], 'o'
	mov PATH[DI+6], 'v'
	mov PATH[DI+7], 'l'
	add DI, 3
	mov KEEP_IND,  DI
					

	pop DX
	pop DI
	pop BX
	pop SI
	ret
PREPARE_PATH ENDP
;-------------------------------------------------- 
OVL_SIZE PROC near
	push AX
	push BX
	push CX
	push DX
	push DI
	push ES

	push DX
	mov AX, 1A00h
	mov DX, offset DTA
	int 21h

	pop DX
	xor CX, CX
	mov AX, 4E00h
	int 21h
	jnc no_err

	cmp AX, 02h
	jne err_size_3
	mov DX, offset SIZE_ERR_2
	call PRINT
	jmp err_pr
err_size_3:
	cmp AX, 03h
	jne err_size_12
	mov DX, offset SIZE_ERR_3
	call PRINT
err_size_12:
	cmp AX, 12h
	jne err_pr
	mov DX, offset SIZE_ERR_12
	call PRINT
err_pr:
	mov DI, offset SIZE_ERR
	add DI, 26
	call WRD_TO_HEX
	mov DX, offset SIZE_ERR
	call PRINT
	jmp exit

no_err:
	mov DI, offset DTA
	add DI, 1Ah
	mov BX,[DI]
	add BX, 0Fh
	shr BX, 04h
	add DI, 2
	mov AX,[DI]
	sal AX, 0Ch
	add BX, AX

	mov AX, 4800h
	int 21h

	mov DI, offset LOAD_EPB
	mov [DI],AX
	
	pop ES
	pop DI
	pop DX
	pop CX
	pop BX
	pop AX
	ret
OVL_SIZE ENDP
;-------------------------------------------------- 
OVL_EXECUTION PROC near
	push AX
	push BX
	push DX
	push DI
	push ES
	
	mov AX, seg DATA
	mov DS, AX
	mov DX, offset PATH

	mov AX, seg DATA
	mov ES, AX
	mov BX, offset LOAD_EPB

	push SP
	push SS
	mov AX, 4B03h
	int 21h

	jnc exec_success
	mov DI, offset EXEC_ERROR
	add DI, 15
	call WRD_TO_HEX
	mov DX, offset EXEC_ERROR
	call PRINT
	
	cmp AX, 1
	je err_ex_1
	cmp AX, 2
	je err_ex_2
	cmp AX, 3
	je err_ex_3
	cmp AX, 4
	je err_ex_4
	cmp AX, 5
	je err_ex_5
	cmp AX, 8
	je err_ex_8
	cmp AX, 10
	je err_ex_10
	
err_ex_1:
	mov DX, offset EXEC_ERROR_1
	jmp err_prt
err_ex_2:
	mov DX, offset EXEC_ERROR_2
	jmp err_prt
err_ex_3:
	mov DX, offset EXEC_ERROR_3
	jmp err_prt
err_ex_4:
	mov DX, offset EXEC_ERROR_4
	jmp err_prt
err_ex_5:
	mov DX, offset EXEC_ERROR_5
	jmp err_prt
err_ex_8:
	mov DX, offset EXEC_ERROR_8
	jmp err_prt
err_ex_10:
	mov DX, offset EXEC_ERROR_10
	jmp err_prt
err_prt:
	call PRINT
	pop ES
	pop DI
	pop DX
	pop BX
	pop AX
	jmp exit
	
exec_success:
	mov AX, LOAD_EPB
	mov WORD ptr ADDRESS_CALL+2, AX
	call ADDRESS_CALL

	mov AX, LOAD_EPB
	mov ES, AX
	mov AX, 4900h
	int 21h

	pop SS
	pop SP
	pop ES
	pop DI
	pop DX
	pop BX
	pop AX
	ret
OVL_EXECUTION ENDP
;-------------------------------------------------- 
MAIN PROC far
	mov KEEP_PSP, ES
	mov AX, seg DATA
	mov DS, AX

	call FREE_EXCESS
	call PREPARE_PATH

	mov DX, offset PATH
	call OVL_SIZE
	call OVL_EXECUTION

	mov DI, KEEP_IND
	mov PATH[DI], '2'
	mov DX, offset PATH
	call OVL_SIZE
	call OVL_EXECUTION

exit:	
	xor AL, AL
	mov AH, 4Ch
	int 21h
	ret
MAIN ENDP
;-------------------------------------------------- 
END_OF_PROGRAM:
CODE ENDS

END MAIN 