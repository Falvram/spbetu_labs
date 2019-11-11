OVERLAY SEGMENT
	ASSUME CS:OVERLAY,  ES:NOTHING,  DS:NOTHING,  SS:NOTHING 

MAIN PROC far
	push AX
	push DX
	push DI
	push DS

	mov DS, AX
	mov DI, offset ADDRESS
	add DI, 33
	mov AX, CS
	call WRD_TO_HEX
	mov DX, offset ADDRESS
	call PRINT

	pop DS
	pop DI
	pop DX
	pop AX
	retf
MAIN ENDP

TETR_TO_HEX PROC near
	and AL, 0Fh
	cmp AL, 09
	jbe NEXT
	add AL, 07
NEXT: 
	add AL, 30h

	ret
TETR_TO_HEX ENDP

BYTE_TO_HEX PROC near
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
 
WRD_TO_HEX PROC near
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

PRINT PROC near
	push AX
	mov AH, 09h
	int 21h
	pop AX
	ret
PRINT ENDP

ADDRESS		db 'Segment address of overlay 1:     ',0Dh, 0Ah,'$'

OVERLAY ENDS
END MAIN 