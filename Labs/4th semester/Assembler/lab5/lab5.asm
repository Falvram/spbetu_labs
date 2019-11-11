AStack SEGMENT STACK
	dw 256 dup (?)
AStack ENDS
;------------------------------------------------
DATA SEGMENT
			PARAMETER			db ' /un'
			PARAMETER_LENG		db '    '
			CHECK				db 'USER'
			CHECK_ROUT			db 'Rout is already loaded.',0Dh, 0Ah,'$'
			S_LOAD_ROUT			db 'Rout is successfully loaded.',0Dh, 0Ah,'$'
			UNLOAD_ROUT_MESSAGE	db 'Rout is successfully unloaded.',0Dh, 0Ah,'$'
			ROUT_NOT_LOADED		db 'Rout is not loaded.',0Dh, 0Ah,'$'
DATA ENDS
;------------------------------------------------
CODE SEGMENT
			ASSUME CS:CODE, DS:DATA, SS:AStack
;------------------------------------------------
ROUT PROC far
			jmp begin
			KEEP_PSP	dw 0
			SIGNATURE	db 'USER'
			KEEP_IP		dw 0
			KEEP_CS		dw 0
			KEEP_AX		dw 0
			KEEP_SS		dw 0
			KEEP_SP		dw 0
			REQ_KEY_C	db 2Eh
			REQ_KEY_D	db 20h
			ROUT_STACK	dw 64 dup (?)
			STACK_PTR:
		
	begin:
			mov KEEP_AX, AX
			mov KEEP_SS, SS
			mov KEEP_SP, SP
			mov AX, seg ROUT_STACK
			mov SS, AX
			mov SP, offset STACK_PTR
			mov AX, KEEP_AX
			push AX 
			push DX
			push DS
			push ES
			
			in AL, 60h
			cmp AL, REQ_KEY_C
			je do_req_C
			cmp AL, REQ_KEY_D
			je do_req_D
			
			pushf
			call dword ptr CS:KEEP_IP 
			jmp end_r
		
	do_req_C:
			mov CL, 'C'
			jmp do_req
	do_req_D:
			mov CL, 'D'
			
	do_req: 
			push AX
			push ES
			in AL, 61h
			mov AH, AL
			or AL, 80h
			out 61h, AL
			xchg AH, AL
			out 61h, AL
			mov AL, 20h
			out 20h, AL
		
			mov	AX, 0040h
			mov ES, AX
			mov AX, ES:[17h]
			and AL, 00000011b
			cmp AL, 0
			je shift_0
			mov CH, 1
	shift_0:
			cmp CH, 0
			je 	no_changes
			mov CL, '|'
	no_changes:		
			pop ES
			pop AX
				
	print_sym:
			mov AH, 05h
			mov CH, 00h
			int 16h
			or AL, AL
			jz end_r
			
			mov AX, 0040h
			mov ES, AX
			mov AX, ES:[1Ah]
			mov ES:[1Ch], AX
			jmp print_sym

	end_r:	
			pop ES 
			pop DS
			pop DX
			pop AX 
			mov SS, KEEP_SS
			mov SP, KEEP_SP
			mov AL, 20h
			out 20h, AL
			iret
ROUT ENDP
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
LOAD_ROUT PROC near
			push AX
			push BX
			push DX
			push DS
			push ES
			
			mov AX, 3509h
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
			mov AX, 3509h
			int 21h
			mov KEEP_IP, BX
			mov KEEP_CS, ES
			pop ES
			mov DX, offset ROUT
			mov AX, seg ROUT
			mov DS, AX
			mov AX, 2509h
			int 21h
			
			pop ES
			pop DS
			mov DX, offset S_LOAD_ROUT
			call PRINT
			pop DX
			pop BX
			pop AX
			ret
LOAD_ROUT ENDP
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
			mov AX, 2509h
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
			mov AX, 3509h
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
			call LOAD_ROUT
			call SET_ROUT
		exit:
			mov AX, 4C00h
			int 21h
			ret
MAIN ENDP
			CODE ENDS
			END MAIN