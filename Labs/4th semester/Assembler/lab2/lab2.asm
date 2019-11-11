TESTPC     SEGMENT
            ASSUME  CS:TESTPC, DS:TESTPC, ES:NOTHING, SS:NOTHING
            ORG     100H 
START:     JMP     BEGIN
;-------------------------------------------------- 
UNAVAILABLE_SEGMENT	db	'Segment adress of unavailable memory:          ',0DH,0AH,'$'
ENVIRONMENT_SEGMENT	db	'Segment adress of environment:            		',0DH,0AH,'$'
TAIL				db	'Tail is:$'
MISSING_TAIL		db	'Tail is missing in PSP',0DH,0AH,'$'
ENVIRONMENT_CONTENT	db	'Environment content:',0DH,0AH,'$'
PATH				db	'Path is: ',0DH,0AH,'$'
NEWLINE				db	0DH,0AH,'$'
;-------------------------------------------------- 
TETR_TO_HEX   PROC  near 
			and      AL,0Fh
			cmp      AL,09
			jbe      NEXT
			add      AL,07 
NEXT:  	    add      AL,30h
            ret 
TETR_TO_HEX   ENDP 
;-------------------------------------------------- 
BYTE_TO_HEX   PROC  near
            push     CX
            mov      AH,AL
            call     TETR_TO_HEX
            xchg     AL,AH
            mov      CL,4
            shr      AL,CL
            call     TETR_TO_HEX ; AL &
			pop      CX          ; AH &
            ret 
BYTE_TO_HEX  ENDP 
;-------------------------------------------------- 
WRD_TO_HEX   PROC  near ;  16 / 16-   ;  AX - , DI -
            push     BX
            mov      BH,AH
            call     BYTE_TO_HEX
            mov      [DI],AH
            dec      DI
            mov      [DI],AL
            dec      DI
            mov      AL,BH
            call     BYTE_TO_HEX
            mov      [DI],AH
            dec      DI
            mov      [DI],AL
            pop      BX
            ret 
WRD_TO_HEX ENDP 
;-------------------------------------------------- 
PRINT PROC near
	mov AH, 09h
	int 21h
	ret
PRINT ENDP
;-------------------------------------------------- 
FIND_U_SEGMENT PROC near
	mov AX, ES:[2]
	mov DI, OFFSET UNAVAILABLE_SEGMENT
	add DI, 41
	call WRD_TO_HEX
	mov DX, OFFSET UNAVAILABLE_SEGMENT
	call PRINT
	ret
FIND_U_SEGMENT  ENDP
;-------------------------------------------------- 
FIND_E_SEGMENT PROC near
	mov AX, ES:[2Ch]
	mov DI, OFFSET ENVIRONMENT_SEGMENT
	add DI, 34
	call WRD_TO_HEX
	mov DX, OFFSET ENVIRONMENT_SEGMENT
	call PRINT
	ret
FIND_E_SEGMENT  ENDP
;-------------------------------------------------- 
FIND_TAIL PROC near
	xor CX, CX
	mov CL, ES:[80h]
	cmp CL, 0
	je no_tail
	
	mov DX, OFFSET TAIL
	call PRINT
	xor SI, SI
	mov AH, 02h
	loop_t:
		mov DL, ES:[81h + SI]
		int 21h
		inc SI
		loop loop_t
	mov DX, OFFSET NEWLINE
	call PRINT
	ret
	
	no_tail:
		mov DX, OFFSET MISSING_TAIL
		call PRINT
		ret
FIND_TAIL ENDP
;-------------------------------------------------- 
PRINT_ENV_CONTENT PROC near
	mov DX, OFFSET ENVIRONMENT_CONTENT
	call PRINT
	xor SI, SI
	mov BX, ES:[2Ch]
	mov ES, BX
	mov AH, 02h
	
	env_loop:
		mov DL, ES:[SI]
		int 21h
		inc SI
		cmp WORD PTR ES:[SI], 0000h
		je gotopath
		cmp BYTE PTR ES:[SI], 00h
		je linenew
		jmp env_loop
		
	linenew:
		call PRINT_NEWLINE
		mov AH, 02h
		inc SI
		jmp env_loop
	
	gotopath:
		call PRINT_NEWLINE
		add SI, 4
		mov DX, OFFSET PATH
		call PRINT
		mov AH, 02h
		
		path_loop:
			mov DL, ES:[SI]
			int 21h
			inc SI
			cmp BYTE PTR ES:[SI], 00h
			jne path_loop
	ret
PRINT_ENV_CONTENT ENDP
;-------------------------------------------------- 
PRINT_NEWLINE PROC near
	mov DX, OFFSET NEWLINE
	call PRINT
	ret
PRINT_NEWLINE ENDP
;-------------------------------------------------- 
BEGIN:        
	call FIND_U_SEGMENT
	call FIND_E_SEGMENT
	call FIND_TAIL
	call PRINT_ENV_CONTENT
	xor AL,AL
	mov AH,4Ch
	int 21H 
	
TESTPC      ENDS            
			END     START
			
