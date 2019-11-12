; Учебная программа  лабораторной работы №3 по дисциплине "Архитектура компьютера"
; 
; Стек  программы

AStack    SEGMENT  STACK
          DW 12 DUP(?)
AStack    ENDS

; Данные программы

DATA      SEGMENT

;  Директивы описания данных

a      DW    0
b      DW    0
i      DW    0
k      DW    0
i1     DW    0
i2     DW    0
res    DW    0

DATA      ENDS

; Код программы

CODE      SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:AStack

; Головная процедура
Main    PROC  FAR
        push  DS
        sub   AX,AX
        push  AX
        mov   AX,DATA
        mov   DS,AX

		mov ax,i
		cmp ax,b
		jle l1 
		;a>b
			;первая функция
			sal ax,1 
			sal ax,1 
			neg ax
			mov cx,ax
			sub ax,3
			mov i1,ax
			;вторая функция
			add cx,7
			mov i2,cx	
			jmp l1e
		l1:
		;a<=b
			;первая функция
		    sal ax,1 
			sal ax,1
			add ax,i
			add ax,i
			neg ax 
			mov cx,ax
			add ax,10
			neg ax
			mov i1,ax
			;вторая функция
			add cx,8
			mov i2, cx
		l1e:
		
		cmp k,0 ; третья функция
		jl l2
		;k>=0
			mov ax,i2
			cmp ax,0
			jns ns
				neg ax ;если i2<0
			ns:
			sub ax,3
			cmp ax,4
			jg max ; ax>4 - переходим на метку max
				mov ax,4 ; иначе кладем в ax 4
			max:
			jmp l2e
		l2:
		;k<0
			mov ax, i2
			cmp i2,0
			jns gi2
				neg ax ;если i2<0
			gi2:
			cmp i1,0
			jns gi1
				neg ax ;если i1<0 делаем i2<0
			gi1:
			add ax,i1
			cmp ax,0
			jns sum
				neg ax ;если i1+i2<0 
			sum:
			mov res,ax
		l2e:
		
		mov res,ax ;кладем значение третьей функции
		ret   
Main    ENDP
CODE    ENDS
        END Main
