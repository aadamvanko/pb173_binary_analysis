.global foo
foo:
	#mov $0x0, %rcx # <- ak to zakomentujem akosi to vybuchne
	retq

.global main
main:
	callq foo
	mov $0x1, %rcx
	retq
