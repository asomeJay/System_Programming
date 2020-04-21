<h3>[컴파일 방법]</h3>
<h5>Makefile을 이용했습니다.
이 프로그램이 의존하고 있는 코드는 크게 20150038, Basic, Memory, Opcode, Assemble입니다.
그러므로 이들을 모두 컴파일해야 제가 원하는 sicsim 프로그램을 만들 수 있습니다. 

20150038.out: 20150038.c Basic.c Memory.c Opcode.c Assemble.c
			gcc -Wall -g -o 20150038.out Basic.c Memory.c  20150038.c Opcode.c Assemble.c -lm</h5>

<h3>[Code Clean]</h3>

<h5>컴파일 과정에서 중간파일을 만들지 않기 때문에 20150038.out와 *.lst *.obj만 제거하면 됩니다

clean:
			rm 20150038.out *.lst *.obj</h5>

<h3>[실행방법]</h3>
<h5>./20150038.out 으로 실행할 수 있습니다
그러면 shell에
"sicsim>"이 뜰텐데, 이제 내가 원하는 명령어를 입력하면 됩니다. 
이상한 명령어는 무시하고 실행하지 않습니다.(종료되지 않습니다.)
띄어쓰기와 반점 등 형식을 엄격히 지켜주십시오. 그렇지 않을 경우 실행하지 않습니다.
quit을 입력하면 종료할 수 있습니다. </h5>
