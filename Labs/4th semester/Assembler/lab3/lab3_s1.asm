TESTPC     SEGMENT
            ASSUME  CS:TESTPC, DS:TESTPC, ES:NOTHING, SS:NOTHING
            ORG     100H 
START:     JMP     BEGIN
;-------------------------------------------------- 
MCB_CHAIN 			db 'Address | Type | PSP owner | Size | Name',0DH,0AH,'$'
MCB_LINE			db '             h         h                            ',0DH,0AH,'$'
AVAIL				db 'AVAILABLE MEMORY IS        B$'
EXTENDED		 	db 'EXTENDED MEMORY IS       KB$'
PSP_ADDRESS			db 'PSP ADDRESS IS $'
SITE_SIZE			db 'SIZE IS    $'
ERROR_S				db 'MEMORY ALLOCATION ERROR',0DH,0AH,'$'
NEWLINE				db 0DH,0AH,'$'
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
BYTE_TO_HEX PROC  near
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
WRD_TO_HEX  PROC  near ;  16 / 16-   ;  AX - , DI -
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
WRD_TO_DEC PROC near
			push CX
			push DX
			mov CX,10
		loop_bd: div CX
			or DL,30h
			mov [SI],DL
			dec SI
			xor DX,DX
			cmp AX,10
			jae loop_bd
			cmp AL,00h
			je end_l
			or AL,30h
			mov [SI],AL
		end_l: pop DX
			pop CX
			ret
WRD_TO_DEC ENDP
;-------------------------------------------------- 
PRINT PROC near
			push AX
			mov AH, 09h
			int 21h
			pop AX
			ret
PRINT ENDP
;-------------------------------------------------- 
AVAILABLE_MEMORY PROC near
			mov ah, 4Ah
			mov bx, 0ffffh
			int 21h
			xor	DX, DX
			mov AX, BX
			mov CX, 16
			mul CX
			mov SI, offset AVAIL + 25
			call WRD_TO_DEC
			mov	DX, offset AVAIL
			call PRINT
			mov	DX, offset NEWLINE
			call PRINT
			ret
AVAILABLE_MEMORY ENDP
;-------------------------------------------------- 
EXTENDED_MEMORY PROC near
			xor DX, DX
			mov	AL, 30h
			out	70h, AL
			in	AL, 71h
			mov	BL, AL 
			mov	AL, 31h
			out	70h, AL
			in 	AL, 71h
			mov	AH, AL
			mov	AL, BL
			mov SI, offset EXTENDED + 23
			call WRD_TO_DEC
			mov DX, offset EXTENDED
			call PRINT
			mov DX, offset NEWLINE
			call PRINT
			ret
EXTENDED_MEMORY ENDP
;-------------------------------------------------- 
BLOCK_CHAIN PROC near
			mov AH, 52h
			int 21h
			mov ES, ES:[BX-2]
			mov DX, offset MCB_CHAIN
			call PRINT
		mcb:
			mov AX, ES
			mov DI, offset MCB_LINE + 5
			call WRD_TO_HEX
			
			mov AX, ES:[0000h]
			mov DI, offset MCB_LINE + 11
			call BYTE_TO_HEX
			
			mov [di], AL
			inc di
			mov [di], AH
			
			mov AX, ES:[0001h]
			mov DI, offset MCB_LINE + 22
			call WRD_TO_HEX
			
			mov AX,ES:[0003h]
			mov BX, 16
			mul BX
			mov SI, offset MCB_LINE + 32
			call WRD_TO_DEC
			
			mov CX,8
			mov BX,0
			mov DI, offset MCB_LINE + 35
		name_loop:
			mov AX, ES:[08h+BX]
			mov [DI+BX], AX
			inc BX
			loop name_loop
			
			mov DX, offset MCB_LINE
			call PRINT
			
			mov BL, ES:[0000h]
			mov AX, ES
			add AX, ES:[0003h]
			inc AX
			mov ES, AX
			cmp BL, 4Dh
			je mcb
			ret
BLOCK_CHAIN ENDP
;-------------------------------------------------- 
BEGIN:        
			call AVAILABLE_MEMORY
			call EXTENDED_MEMORY
			call BLOCK_CHAIN
			xor AL,AL
			mov AH,4Ch
			int 21H 
			
TESTPC      ENDS            
			END     START
			
