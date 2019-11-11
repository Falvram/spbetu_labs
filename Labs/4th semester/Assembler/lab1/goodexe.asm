AStack SEGMENT STACK
	DW 10h DUP(?)
AStack ENDS

DATA SEGMENT
PC_TYPE		db		'PC type is ','$'
OS_VERSION 	db		'OS Version is  . ',0DH,0AH,'$'
OEM_NUMBER 	db		'OEM number is    ',0DH,0AH,'$'
USER_NUMBER	db		'User number is       ','$'

PC			db 		'PC',0DH,0AH,'$'
PCXT		db		'PC/XT',0DH,0AH,'$'
AT0			db		'AT',0DH,0AH,'$'
PS2_30		db		'PS2 model 30',0DH,0AH,'$'
PS2_80		db		'PS2 model 80',0DH,0AH,'$'
PCjr		db		'PCjr',0DH,0AH,'$'
PC_CONVERT	db		'PC Convertible',0DH,0AH,'$'

NEWLINE		db		0DH,0AH,'$'
DATA ENDS
;----------------------------------------------------- 

CODE SEGMENT
	ASSUME CS:CODE, DS:DATA, SS:AStack
	
TETR_TO_HEX   PROC  FAR 
			and      AL,0Fh
			cmp      AL,09
			jbe      NEXT
			add      AL,07 
NEXT:  	    add      AL,30h
            ret 
TETR_TO_HEX   ENDP 
;------------------------------- 
BYTE_TO_HEX   PROC  FAR
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
;------------------------------- 
WRD_TO_HEX   PROC  FAR ;  16 / 16-   ;  AX - , DI -
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
BYTE_TO_DEC   PROC  FAR ;   10/, SI -   &%  
            push     CX
            push     DX
            xor      AH,AH
            xor      DX,DX
            mov      CX,10 
loop_bd:    div      CX
            or       DL,30h
            mov      [SI],DL
            dec      SI
            xor      DX,DX
            cmp      AX,10
            jae      loop_bd
            cmp      AL,00h
            je       end_l
            or       AL,30h
            mov      [SI],AL 
end_l:      pop      DX
            pop      CX
            ret
BYTE_TO_DEC    ENDP 

PRINT PROC FAR
	mov AH, 09h
	int 21h
	ret
PRINT ENDP

FIND_OS_VERSION PROC FAR
	xor AX, AX
	mov AH,30h
	int 21h
	
	mov SI,OFFSET OS_VERSION
	add SI, 14
	push AX
	call BYTE_TO_DEC

	pop AX
	mov AL,AH
	add SI, 3
	call BYTE_TO_DEC
	
	mov DX, OFFSET OS_VERSION
	call PRINT
	
	mov SI,OFFSET OEM_NUMBER
	add SI, 16
	mov AL,BH
	call BYTE_TO_DEC
	
	mov DX, OFFSET OEM_NUMBER
	call PRINT
	
	mov AL,BL
	call BYTE_TO_HEX
	mov DI,OFFSET USER_NUMBER
	add DI,16
	mov [DI],AH
	dec DI
	mov [DI],AL
	
	mov AX,CX
	mov DI,OFFSET USER_NUMBER
	add DI,20
	call WRD_TO_HEX
	
	mov DX, OFFSET USER_NUMBER
	call PRINT
	ret
FIND_OS_VERSION  ENDP

FIND_PC_TYPE PROC FAR
		mov BX, 0f000h
		mov ES, BX
		mov AX, ES:0fffeh
		
		mov DX, OFFSET PC_TYPE
		call PRINT
		
		cmp AL, 0FFh
		je PC_
		cmp AL, 0FEh
		je PCXT_
		cmp AL, 0FBh
		je PCXT_
		cmp AL, 0FCh
		je AT_
		cmp AL, 0FAh
		je PS2_30_
		cmp AL, 0F8h
		je PS2_80_
		cmp AL, 0FDh
		je PCjr_
		cmp AL, 0F9h
		je PC_CONVERT_
		call BYTE_TO_HEX
		mov BX, AX
		mov DL, BL
		mov AH, 02h
		int 21h
		mov DL, BH
		int 21h
		
		mov DX, OFFSET NEWLINE
		mov AH, 09h
		int 21h
		ret
		
		PC_:
			mov DX, OFFSET PC
			call PRINT
			ret
		PCXT_:
			mov DX, OFFSET PCXT
			call PRINT
			ret
		AT_:
			mov DX, OFFSET AT0
			call PRINT
			ret
		PS2_30_:
			mov DX, OFFSET PS2_30
			call PRINT
			ret
		PS2_80_:
			mov DX, OFFSET PS2_80
			call PRINT
			ret
		PCjr_:
			mov DX, OFFSET PCjr
			call PRINT
			ret
		PC_CONVERT_:
			mov DX, OFFSET PC_CONVERT
			call PRINT
			ret		
FIND_PC_TYPE ENDP			
	
BEGIN PROC FAR   
	mov AX,DATA
	mov DS,AX     
	call FIND_PC_TYPE
	call FIND_OS_VERSION
	xor AL,AL
	mov AH,4Ch
	int 21H 
	ret
	
BEGIN ENDP
CODE ENDS
END BEGIN
			
