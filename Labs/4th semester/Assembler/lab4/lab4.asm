AStack SEGMENT STACK
	dw 64 dup (?)
AStack ENDS
;------------------------------------------------
DATA SEGMENT
			PARAMETER			db ' /un'
			PARAMETER_LENG		db '    '
			CHECK				db 'USER'
			CHECK_ROUT			db 'ROUT is already loaded.',0Dh, 0Ah,'$'
			LOAD_ROUT			db 'Rout is successfully loaded.',0Dh, 0Ah,'$'
			UNLOAD_ROUT_MESSAGE	db 'Rout is successfully unloaded.',0Dh, 0Ah,'$'
			ROUT_NOT_LOADED		db 'Rout is not loaded.',0Dh, 0Ah,'$'
DATA ENDS 
;------------------------------------------------
CODE SEGMENT
	ASSUME CS:CODE,  DS:DATA,  SS:AStack
;------------------------------------------------
ROUT PROC
			jmp start
			KEEP_PSP		dw 0
			KEEP_SS			dw 0
			KEEP_SP			dw 0
			KEEP_CS 		dw 0
			KEEP_IP 		dw 0
			SIGNATURE  		db 'USER'
			COUNTER			dw 0
			COUNT_MESSAGE	db 'ROUT CALLED:      $'
			ROUT_STACK 		dw 64 dup (?)
			STACK_END:
		start:
			mov KEEP_SS, SS
			mov KEEP_SP, SP
			mov AX, seg ROUT_STACK
			mov SS, AX
			mov SP, offset STACK_END

			push AX
			push BX
			push CX
			push DX

			call getCurs
			push DX
			call setCurs

			push DS
			mov AX, seg COUNTER
			mov DS, AX
			mov AX, COUNTER
			inc AX
			mov COUNTER, AX
			push DI
			mov DI, offset COUNT_MESSAGE
			add DI, 17
			call WRD_TO_HEX
			pop DI
			pop DS
			push ES
			push BP
			mov AX, seg COUNT_MESSAGE
			mov ES, AX
			mov BP, offset COUNT_MESSAGE
			call outputBP
			pop BP
			pop ES

			pop DX
			mov AH, 02h
			mov BH, 0
			int 10h

			pop DX
			pop CX
			pop BX
			pop AX
			mov SP, KEEP_SP
			mov SS, KEEP_SS
			mov AL, 20h
			out 20h, AL
			iret	
ROUT ENDP
;------------------------------------------------
getCurs PROC
			push AX
			push BX
			push CX
			mov AH, 03h
			mov BH, 0
			int 10h
			pop CX
			pop BX
			pop AX
			ret
getCurs ENDP
;------------------------------------------------
setCurs PROC
			push AX
			push BX
			mov DX, 2200h
			mov AH, 02h
			mov BH, 0
			int 10h
			pop BX
			pop AX
			ret
setCurs ENDP
;------------------------------------------------
outputBP PROC
			push AX
			push BX
			push CX
			push DX
			mov AH, 13h
			mov AL, 1
			mov BH, 0
			mov DH, 0Ch
			mov DL, 1Eh
			mov CX, 12h
			int 10h
			pop DX
			pop CX
			pop BX
			pop AX
			ret
outputBP ENDP
;------------------------------------------------
TETR_TO_HEX PROC near
			and AL, 0Fh
			cmp AL, 09
			jbe next
			add AL, 07
		next: 
			add AL, 30h
			ret
TETR_TO_HEX ENDP
;------------------------------------------------
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
;------------------------------------------------
WRD_TO_HEX PROC near
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
			ret
WRD_TO_HEX ENDP
;------------------------------------------------
RESIDENT_END:
;------------------------------------------------
PRINT PROC near
			push AX
			mov AH, 09h
			int 21h
			pop AX
			ret
PRINT ENDP
;------------------------------------------------
SET_RESIDENT PROC near
			push AX
			push BX
			push DX
			push DS
			push ES
			
			mov AX, 351Ch
			int 21h
			cld
			mov CX, 4
			lea DI, ES:SIGNATURE
			lea SI, DS:CHECK
			repe cmpsb
			jne not_loaded
			mov DX, offset CHECK_ROUT
			call PRINT
			jmp exit
			
		not_loaded:
			push ES
			mov AX, 351Ch
			int 21h
			mov KEEP_IP, BX
			mov KEEP_CS, ES
			pop ES
			mov DX, offset ROUT
			mov AX, seg ROUT
			mov DS, AX
			mov AX, 251Ch
			int 21h
			
			pop ES
			pop DS
			mov DX, offset LOAD_ROUT
			call PRINT
			pop DX
			pop BX
			pop AX
			ret
SET_RESIDENT ENDP
;------------------------------------------------
SET_ROUT PROC near
			mov DX, offset RESIDENT_END
			mov CL, 04h
			shr DX, CL
			add DX, 100h
			mov AX, 3100h
			int 21h
			ret
SET_ROUT ENDP
;------------------------------------------------
UNLOAD_ROUT PROC near
			cli
			push DS
			mov AX, ES:KEEP_CS
			mov DX, ES:KEEP_IP
			mov DS, AX
			mov AX, 251Ch
			int 21h
			pop DS
			mov SI, offset KEEP_PSP
			mov AX, ES:[BX+SI]
			mov ES, AX
			mov AX, ES:[2Ch]
			push ES
			mov ES, AX
			mov AH, 49h
			int 21h
			pop ES
			int 21h
			sti
			mov DX, offset UNLOAD_ROUT_MESSAGE
			call PRINT
			jmp exit
			ret
UNLOAD_ROUT ENDP
;------------------------------------------------
CHECK_PARAMETER PROC near
			push AX
			push BX
			push CX
			push SI
			push DI
			mov CL, ES:[80h]
			cmp CL, 4
			jne no_parameter
			mov BX, 0
			mov SI, offset PARAMETER_LENG
		read:
			mov AL, ES:[81h+BX]
			mov [SI],AL
			inc SI
			inc BX
			loop read

			push ES
			cld
			mov CX, 4
			mov AX, seg DATA
			mov ES, AX
			lea DI, DS:PARAMETER
			lea SI, DS:PARAMETER_LENG
			repe cmpsb
			pop ES
			jne no_parameter
			push ES
			mov AX, 351Ch
			int 21h
			cld
			mov CX, 4
			lea DI, ES:SIGNATURE
			lea SI, DS:CHECK
			repe cmpsb
			jne r_not_loaded
			call UNLOAD_ROUT
			
		r_not_loaded:
			pop ES
			mov DX, offset ROUT_NOT_LOADED
			call PRINT
			jmp exit
			
		no_parameter:
			pop DI
			pop SI
			pop CX
			pop BX
			pop AX
			ret
CHECK_PARAMETER ENDP
;------------------------------------------------
MAIN PROC near
			mov KEEP_PSP, ES
			mov AX, seg DATA
			mov DS, AX

			call CHECK_PARAMETER
			call SET_RESIDENT
			call SET_ROUT
		exit:
			mov AX, 4C00h
			int 21h
			ret
MAIN ENDP
			CODE ENDS
			END MAIN