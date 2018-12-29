	.file	"practice.c"
	.text
	.globl	_decode2
	.def	_decode2;	.scl	2;	.type	32;	.endef
_decode2:
LFB0:
	.cfi_startproc
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	16(%esp), %esi
	movl	24(%esp), %ecx
	movl	28(%esp), %ebx
	subl	32(%esp), %ecx
	sbbl	36(%esp), %ebx
	imull	%esi, %ebx
	movl	%ecx, %eax
	imull	20(%esp), %eax
	addl	%eax, %ebx
	movl	%esi, %eax
	mull	%ecx
	movl	%eax, %esi
	movl	%edx, %edi
	addl	%ebx, %edi
	movl	%ecx, %eax
	cltd
	xorl	%esi, %ecx
	xorl	%edi, %edx
	movl	%ecx, %eax
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE0:
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
