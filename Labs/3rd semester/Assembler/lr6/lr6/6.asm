.386p
.MODEL FLAT;, C
.CODE

PUBLIC func1 ; C func1		
func1 PROC C array:DWORD, amount:DWORD, counter:DWORD, Xmin:DWORD

MOV EDI, array				   ;Адрес массива случайных чисел              
MOV ESI, counter			   ;Адрес массива счетчика чисел 
MOV ECX, amount				   ;Длина массива случайных чисел  
MOV EAX, Xmin			                                  
        
CYCLE:
	MOV EBX, [EDI]		           ;Извлечение случайного числа N
	SUB EBX, EAX	               ;Вычесть левую границу диапазона
	ADD DWORD PTR[ESI+4*EBX], 1;   ;Увеличение счетчика числа на 1
	ADD EDI, 4		               ;Переход к следующему числу
LOOP CYCLE		

RET 
func1 ENDP



PUBLIC func2	
func2 PROC C counter:DWORD, RightBorder:DWORD, InterDif:DWORD, Border:DWORD, Xmin:DWORD

MOV EDI, RightBorder     ;Адрес массива правых границ
MOV ESI, counter		 ;Адрес массива счетчика чисел
MOV EAX, InterDif		 ;Адрес массива заданных интервалов
MOV ECX, Border			 ;Количество разбиений (интервалов)
MOV EBX, XMIN   


XOR EDX, EDX		   

CYCLE:
	CMP EBX, [EDI]	       
	JG NEXT_RANGE	      ;Переход, если число больше текущ. границы
	ADD EDX, [ESI]        ;Накопление 
	INC EBX               ;Переход к следующему числу 
	ADD ESI, 4            ;Переход к след. эл. распр. чисел с ед. диапазоном
	JMP CYCLE
NEXT_RANGE:			      ;Достигнута правая граница интервала
	MOV [EAX], EDX        ;Помещаем в массив с зад. распр. накопленное значение
	XOR EDX, EDX          ;Обнуляем значение
	ADD EAX, 4	          ;Переход к следующем элементу массива
	ADD EDI, 4            ;Переход к следующей границе
LOOP CYCLE

RET
func2 ENDP
END