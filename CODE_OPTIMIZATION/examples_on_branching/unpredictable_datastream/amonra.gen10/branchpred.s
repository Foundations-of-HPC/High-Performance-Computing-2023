   1              		.file	"branchpred.c"
   2              		.intel_syntax noprefix
   3              	# GNU C17 (Spack GCC) version 11.2.0 (x86_64-pc-linux-gnu)
   4              	#	compiled by GNU C version 11.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl v
   5              	# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
   6              	# options passed: -march=skylake-avx512 -mmmx -mpopcnt -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.
   7              		.text
   8              	.Ltext0:
   9              		.section	.rodata.str1.8,"aMS",@progbits,1
  10              		.align 8
  11              	.LC2:
  12 0000 0A73756D 		.string	"\nsum is %llu, elapsed seconds: %g\n"
  12      20697320 
  12      256C6C75 
  12      2C20656C 
  12      61707365 
  13              		.section	.text.startup,"ax",@progbits
  14              		.p2align 4
  15              		.globl	main
  17              	main:
  18              	.LFB32:
  19              		.file 1 "branchpred.c"
   1:branchpred.c  **** /*
   2:branchpred.c  ****  * This file is part of the exercises for the Lectures on 
   3:branchpred.c  ****  *   "Foundations of High Performance Computing"
   4:branchpred.c  ****  * given at 
   5:branchpred.c  ****  *   Master in HPC and 
   6:branchpred.c  ****  *   Master in Data Science and Scientific Computing
   7:branchpred.c  ****  * @ SISSA, ICTP and University of Trieste
   8:branchpred.c  ****  *
   9:branchpred.c  ****  * contact: luca.tornatore@inaf.it
  10:branchpred.c  ****  *
  11:branchpred.c  ****  *     This is free software; you can redistribute it and/or modify
  12:branchpred.c  ****  *     it under the terms of the GNU General Public License as published by
  13:branchpred.c  ****  *     the Free Software Foundation; either version 3 of the License, or
  14:branchpred.c  ****  *     (at your option) any later version.
  15:branchpred.c  ****  *     This code is distributed in the hope that it will be useful,
  16:branchpred.c  ****  *     but WITHOUT ANY WARRANTY; without even the implied warranty of
  17:branchpred.c  ****  *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  18:branchpred.c  ****  *     GNU General Public License for more details.
  19:branchpred.c  ****  *
  20:branchpred.c  ****  *     You should have received a copy of the GNU General Public License 
  21:branchpred.c  ****  *     along with this program.  If not, see <http://www.gnu.org/licenses/>
  22:branchpred.c  ****  */
  23:branchpred.c  **** 
  24:branchpred.c  **** 
  25:branchpred.c  **** 
  26:branchpred.c  **** 
  27:branchpred.c  **** #include <stdlib.h>
  28:branchpred.c  **** #include <stdio.h>
  29:branchpred.c  **** #include <string.h>
  30:branchpred.c  **** #include <time.h>
  31:branchpred.c  **** 
  32:branchpred.c  **** 
  33:branchpred.c  **** #define SIZE_DEFAULT 1000000
  34:branchpred.c  **** #define TOP (2 << 20)
  35:branchpred.c  **** #define PIVOT (TOP >> 2)
  36:branchpred.c  **** 
  37:branchpred.c  **** 
  38:branchpred.c  **** #define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
  39:branchpred.c  **** 		   (double)ts.tv_nsec * 1e-9)
  40:branchpred.c  **** 
  41:branchpred.c  **** 
  42:branchpred.c  **** #ifdef WOW  
  43:branchpred.c  **** int compare(const void *A, const void *B)
  44:branchpred.c  **** {
  45:branchpred.c  ****   return *(int*)A - *(int*)B;
  46:branchpred.c  **** }
  47:branchpred.c  **** #endif
  48:branchpred.c  **** 
  49:branchpred.c  **** int main(int argc, char **argv)
  50:branchpred.c  **** {
  20              		.loc 1 50 1
  21              		.cfi_startproc
  22              	.LVL0:
  51:branchpred.c  ****   int  SIZE;
  23              		.loc 1 51 3
  52:branchpred.c  ****   int *data;
  24              		.loc 1 52 3
  53:branchpred.c  ****   int  cc, ii;
  25              		.loc 1 53 3
  54:branchpred.c  **** 
  55:branchpred.c  **** #ifdef WOW
  56:branchpred.c  ****   double tot_tstart, tot_tstop;
  57:branchpred.c  **** #endif
  58:branchpred.c  **** 
  59:branchpred.c  ****   long long sum = 0;
  26              		.loc 1 59 3
  60:branchpred.c  **** 
  61:branchpred.c  ****   struct timespec ts;
  27              		.loc 1 61 3
  62:branchpred.c  ****   double tstart, tstop;
  28              		.loc 1 62 3
  63:branchpred.c  ****   
  64:branchpred.c  ****   if(argc > 1)
  29              		.loc 1 64 3
  30              	# branchpred.c:50: {
  50:branchpred.c  ****   int  SIZE;
  31              		.loc 1 50 1 is_stmt 0
  32 0000 4C8D5424 		lea	r10, [rsp+8]	#,
  32      08
  33              		.cfi_def_cfa 10, 0
  34 0005 4883E4E0 		and	rsp, -32	#,
  35 0009 41FF72F8 		push	QWORD PTR [r10-8]	#
  36 000d 55       		push	rbp	#
  37 000e 4889E5   		mov	rbp, rsp	#,
  38              		.cfi_escape 0x10,0x6,0x2,0x76,0
  39 0011 4156     		push	r14	#
  40 0013 4155     		push	r13	#
  41 0015 4154     		push	r12	#
  42 0017 4152     		push	r10	#
  43              		.cfi_escape 0xf,0x3,0x76,0x60,0x6
  44              		.cfi_escape 0x10,0xe,0x2,0x76,0x78
  45              		.cfi_escape 0x10,0xd,0x2,0x76,0x70
  46              		.cfi_escape 0x10,0xc,0x2,0x76,0x68
  47 0019 53       		push	rbx	#
  48 001a 4883EC28 		sub	rsp, 40	#,
  49              		.cfi_escape 0x10,0x3,0x2,0x76,0x58
  50              	# branchpred.c:64:   if(argc > 1)
  51              		.loc 1 64 5
  52 001e 83FF01   		cmp	edi, 1	# tmp242,
  53 0021 0F8F7502 		jg	.L2	#,
  53      0000
  54              	.LVL1:
  65:branchpred.c  ****     SIZE = atoi( *(argv+1) );
  66:branchpred.c  ****   else
  67:branchpred.c  ****     SIZE = SIZE_DEFAULT;
  68:branchpred.c  **** 
  69:branchpred.c  ****   // Generate data
  70:branchpred.c  ****   data = (int*)calloc(SIZE, sizeof(int));
  55              		.loc 1 70 3 is_stmt 1
  56              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  57              		.loc 1 70 16 is_stmt 0
  58 0027 BE040000 		mov	esi, 4	#,
  58      00
  59              	.LVL2:
  60 002c BF40420F 		mov	edi, 1000000	#,
  60      00
  61              	.LVL3:
  62 0031 E8000000 		call	calloc	#
  62      00
  63              	.LVL4:
  64              	# branchpred.c:71:   srand((int)(SIZE));
  71:branchpred.c  ****   srand((int)(SIZE));
  65              		.loc 1 71 3
  66 0036 BF40420F 		mov	edi, 1000000	#,
  66      00
  67              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
  68              		.loc 1 70 16
  69 003b 4989C5   		mov	r13, rax	# data, tmp244
  70              	.LVL5:
  71              		.loc 1 71 3 is_stmt 1
  72              	# branchpred.c:67:     SIZE = SIZE_DEFAULT;
  67:branchpred.c  **** 
  73              		.loc 1 67 10 is_stmt 0
  74 003e BB40420F 		mov	ebx, 1000000	# SIZE,
  74      00
  75              	# branchpred.c:71:   srand((int)(SIZE));
  76              		.loc 1 71 3
  77 0043 E8000000 		call	srand	#
  77      00
  78              	.LVL6:
  72:branchpred.c  ****   
  73:branchpred.c  ****   for (cc = 0; cc < SIZE; cc++)
  79              		.loc 1 73 3 is_stmt 1
  80              		.loc 1 73 19
  81              	.L3:
  82              	# branchpred.c:67:     SIZE = SIZE_DEFAULT;
  67:branchpred.c  **** 
  83              		.loc 1 67 10 is_stmt 0
  84 0048 4531E4   		xor	r12d, r12d	# ivtmp.34
  85              	.LVL7:
  86              	.L5:
  74:branchpred.c  ****     data[cc] = rand() % TOP;
  87              		.loc 1 74 5 is_stmt 1 discriminator 3
  88              	# branchpred.c:74:     data[cc] = rand() % TOP;
  89              		.loc 1 74 16 is_stmt 0 discriminator 3
  90 004b E8000000 		call	rand	#
  90      00
  91              	.LVL8:
  92              	# branchpred.c:74:     data[cc] = rand() % TOP;
  93              		.loc 1 74 23 discriminator 3
  94 0050 99       		cdq
  95 0051 C1EA0B   		shr	edx, 11	# tmp174,
  96 0054 01D0     		add	eax, edx	# tmp175, tmp174
  97 0056 25FFFF1F 		and	eax, 2097151	# tmp176,
  97      00
  98 005b 29D0     		sub	eax, edx	# tmp177, tmp174
  99              	# branchpred.c:74:     data[cc] = rand() % TOP;
 100              		.loc 1 74 14 discriminator 3
 101 005d 438944A5 		mov	DWORD PTR [r13+0+r12*4], eax	# MEM[(int *)data_68 + ivtmp.34_89 * 4], tmp177
 101      00
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 102              		.loc 1 73 29 is_stmt 1 discriminator 3
 103              	.LVL9:
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 104              		.loc 1 73 19 discriminator 3
 105 0062 49FFC4   		inc	r12	# ivtmp.34
 106              	.LVL10:
 107 0065 4439E3   		cmp	ebx, r12d	# SIZE, ivtmp.34
 108 0068 7FE1     		jg	.L5	#,
 109              	.L4:
  75:branchpred.c  **** 
  76:branchpred.c  ****    
  77:branchpred.c  ****   
  78:branchpred.c  **** #ifdef WOW
  79:branchpred.c  ****   tot_tstart = TCPU_TIME;
  80:branchpred.c  ****   // !!! With this, the next loop runs faster
  81:branchpred.c  ****   qsort(data, SIZE, sizeof(int), compare);
  82:branchpred.c  **** #endif
  83:branchpred.c  ****   
  84:branchpred.c  **** 
  85:branchpred.c  ****   tstart = TCPU_TIME;
 110              		.loc 1 85 3
 111              	# branchpred.c:85:   tstart = TCPU_TIME;
 112              		.loc 1 85 12 is_stmt 0
 113 006a BF020000 		mov	edi, 2	#,
 113      00
 114 006f 488D75C0 		lea	rsi, [rbp-64]	# tmp254,
 115 0073 E8000000 		call	clock_gettime	#
 115      00
 116              	.LVL11:
 117 0078 C5D857E4 		vxorps	xmm4, xmm4, xmm4	# tmp248
 118 007c C4E1DB2A 		vcvtsi2sd	xmm0, xmm4, QWORD PTR [rbp-56]	# tmp249, tmp248, ts.tv_nsec
 118      45C8
 119 0082 89D8     		mov	eax, ebx	# bnd.9, SIZE
 120 0084 C1E803   		shr	eax, 3	# bnd.9,
 121 0087 FFC8     		dec	eax	# tmp184
 122 0089 C5FB10C8 		vmovsd	xmm1, xmm0, xmm0	# tmp179, tmp249
 123 008d C4E1DB2A 		vcvtsi2sd	xmm0, xmm4, QWORD PTR [rbp-64]	# tmp250, tmp248, ts.tv_sec
 123      45C0
 124 0093 48C1E005 		sal	rax, 5	# tmp185,
 125 0097 4189D8   		mov	r8d, ebx	# niters_vector_mult_vf.10, SIZE
 126 009a C4E27D58 		vpbroadcastd	ymm3, DWORD PTR .LC3[rip]	# tmp240,
 126      1D000000 
 126      00
 127              	# branchpred.c:85:   tstart = TCPU_TIME;
 128              		.loc 1 85 10
 129 00a3 C4E2F999 		vfmadd132sd	xmm1, xmm0, QWORD PTR .LC0[rip]	# tmp179, tmp180,
 129      0D000000 
 129      00
 130 00ac 448D4BFF 		lea	r9d, [rbx-1]	# _105,
 131 00b0 498D5405 		lea	rdx, [r13+32+rax]	# _50,
 131      20
 132 00b5 4183E0F8 		and	r8d, -8	# niters_vector_mult_vf.10,
 133 00b9 BFE80300 		mov	edi, 1000	# ivtmp_70,
 133      00
 134 00be C4C1F97E 		vmovq	r14, xmm1	# tstart, tmp179
 134      CE
 135              	.LVL12:
  86:branchpred.c  ****   
  87:branchpred.c  ****   for (cc = 0; cc < 1000; cc++)
 136              		.loc 1 87 3 is_stmt 1
 137              		.loc 1 87 19
 138              		.p2align 4,,10
 139 00c3 0F1F4400 		.p2align 3
 139      00
 140              	.L6:
  88:branchpred.c  ****       {
  89:branchpred.c  **** 	sum = 0;
  90:branchpred.c  **** 
  91:branchpred.c  ****         for (ii = 0; ii < SIZE; ii++)
 141              		.loc 1 91 25
 142              	# branchpred.c:89: 	sum = 0;
  89:branchpred.c  **** 
 143              		.loc 1 89 6 is_stmt 0
 144 00c8 4531E4   		xor	r12d, r12d	# stmp_sum_31.19
 145              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
 146              		.loc 1 91 25
 147 00cb 85DB     		test	ebx, ebx	# SIZE
 148 00cd 0F8E4101 		jle	.L20	#,
 148      0000
 149 00d3 4183F906 		cmp	r9d, 6	# _105,
 150 00d7 0F86B501 		jbe	.L21	#,
 150      0000
 151 00dd 4C89E8   		mov	rax, r13	# ivtmp.23, data
 152 00e0 C5F1EFC9 		vpxor	xmm1, xmm1, xmm1	# vect_sum_62.12
 153              	.LVL13:
 154              		.p2align 4,,10
 155 00e4 0F1F4000 		.p2align 3
 156              	.L8:
  92:branchpred.c  ****           {
  93:branchpred.c  **** #if !defined( BESMART ) && !defined( BESMART2 )
  94:branchpred.c  ****             if (data[ii] > PIVOT)
 157              		.loc 1 94 13 is_stmt 1
 158              	# branchpred.c:94:             if (data[ii] > PIVOT)
 159              		.loc 1 94 21 is_stmt 0
 160 00e8 C5FE6F10 		vmovdqu	ymm2, YMMWORD PTR [rax]	# MEM <vector(8) int> [(int *)_66], MEM <vector(8) int> [(int *)_6
  95:branchpred.c  ****               sum += data[ii];
 161              		.loc 1 95 15 is_stmt 1
 162 00ec 4883C020 		add	rax, 32	# ivtmp.23,
 163 00f0 C5ED66C3 		vpcmpgtd	ymm0, ymm2, ymm3	# tmp189, MEM <vector(8) int> [(int *)_66], tmp240
 164 00f4 C5FDDBC2 		vpand	ymm0, ymm0, ymm2	# vect_patt_81.16, tmp189, MEM <vector(8) int> [(int *)_66]
 165 00f8 C4E27D25 		vpmovsxdq	ymm2, xmm0	#, vect_patt_81.16
 165      D0
 166 00fd C4E37D39 		vextracti128	xmm0, ymm0, 0x1	# tmp193, vect_patt_81.16
 166      C001
 167 0103 C5EDD4C9 		vpaddq	ymm1, ymm2, ymm1	# vect_sum_31.18, vect_patt_82.17, vect_sum_62.12
 168 0107 C4E27D25 		vpmovsxdq	ymm0, xmm0	# vect_patt_82.17, tmp193
 168      C0
 169 010c C5FDD4C9 		vpaddq	ymm1, ymm0, ymm1	# vect_sum_62.12, vect_patt_82.17, vect_sum_31.18
  91:branchpred.c  ****           {
 170              		.loc 1 91 35
  91:branchpred.c  ****           {
 171              		.loc 1 91 25
 172 0110 4839C2   		cmp	rdx, rax	# _50, ivtmp.23
 173 0113 75D3     		jne	.L8	#,
 174 0115 C5F96FC1 		vmovdqa	xmm0, xmm1	# tmp194, vect_sum_62.12
 175 0119 62F3FD28 		vextracti64x2	xmm1, ymm1, 0x1	# tmp195, vect_sum_62.12
 175      39C901
 176 0120 C5F9D4C1 		vpaddq	xmm0, xmm0, xmm1	# _127, tmp194, tmp195
 177 0124 C5F173D8 		vpsrldq	xmm1, xmm0, 8	# tmp197, _127,
 177      08
 178 0129 C5F9D4C1 		vpaddq	xmm0, xmm0, xmm1	# tmp198, _127, tmp197
 179 012d C4C1F97E 		vmovq	r12, xmm0	# stmp_sum_31.19, tmp198
 179      C4
 180 0132 4439C3   		cmp	ebx, r8d	# SIZE, niters_vector_mult_vf.10
 181 0135 0F84D900 		je	.L20	#,
 181      0000
 182              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 183              		.loc 1 91 17 is_stmt 0
 184 013b 4489C0   		mov	eax, r8d	# ii, niters_vector_mult_vf.10
 185              	.L7:
 186              	.LVL14:
  94:branchpred.c  ****               sum += data[ii];
 187              		.loc 1 94 13 is_stmt 1
 188              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 189              		.loc 1 94 21 is_stmt 0
 190 013e 4863F0   		movsx	rsi, eax	# ii, ii
 191 0141 488D0CB5 		lea	rcx, [0+rsi*4]	# _62,
 191      00000000 
 192 0149 496374B5 		movsx	rsi, DWORD PTR [r13+0+rsi*4]	#, *_64
 192      00
 193 014e 4989F2   		mov	r10, rsi	#,
 194              	# branchpred.c:95:               sum += data[ii];
 195              		.loc 1 95 19
 196 0151 4C01E6   		add	rsi, r12	# tmp224, stmp_sum_31.19
 197 0154 4181FA00 		cmp	r10d, 524288	# _14,
 197      000800
 198 015b 4C0F4FE6 		cmovg	r12, rsi	# tmp224,, stmp_sum_31.19
 199              	.LVL15:
  91:branchpred.c  ****           {
 200              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 201              		.loc 1 91 25
 202              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 203              		.loc 1 91 35 is_stmt 0
 204 015f 8D7001   		lea	esi, [rax+1]	# ii,
 205              	.LVL16:
 206              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 207              		.loc 1 91 25
 208 0162 39F3     		cmp	ebx, esi	# SIZE, ii
 209 0164 0F8EAA00 		jle	.L20	#,
 209      0000
  94:branchpred.c  ****               sum += data[ii];
 210              		.loc 1 94 13 is_stmt 1
 211              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 212              		.loc 1 94 21 is_stmt 0
 213 016a 4963740D 		movsx	rsi, DWORD PTR [r13+4+rcx]	#, *_73
 213      04
 214              	.LVL17:
 215 016f 4989F2   		mov	r10, rsi	#,
 216              	# branchpred.c:95:               sum += data[ii];
 217              		.loc 1 95 19
 218 0172 4C01E6   		add	rsi, r12	# tmp226, stmp_sum_31.19
 219 0175 4181FA00 		cmp	r10d, 524288	# _90,
 219      000800
 220 017c 4C0F4FE6 		cmovg	r12, rsi	# tmp226,, stmp_sum_31.19
 221              	.LVL18:
  91:branchpred.c  ****           {
 222              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 223              		.loc 1 91 25
 224              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 225              		.loc 1 91 35 is_stmt 0
 226 0180 8D7002   		lea	esi, [rax+2]	# ii,
 227              	.LVL19:
 228              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 229              		.loc 1 91 25
 230 0183 39F3     		cmp	ebx, esi	# SIZE, ii
 231 0185 0F8E8900 		jle	.L20	#,
 231      0000
  94:branchpred.c  ****               sum += data[ii];
 232              		.loc 1 94 13 is_stmt 1
 233              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 234              		.loc 1 94 21 is_stmt 0
 235 018b 4963740D 		movsx	rsi, DWORD PTR [r13+8+rcx]	#, *_30
 235      08
 236              	.LVL20:
 237 0190 4989F2   		mov	r10, rsi	#,
 238              	# branchpred.c:95:               sum += data[ii];
 239              		.loc 1 95 19
 240 0193 4C01E6   		add	rsi, r12	# tmp228, stmp_sum_31.19
 241 0196 4181FA00 		cmp	r10d, 524288	# _27,
 241      000800
 242 019d 4C0F4FE6 		cmovg	r12, rsi	# tmp228,, stmp_sum_31.19
 243              	.LVL21:
  91:branchpred.c  ****           {
 244              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 245              		.loc 1 91 25
 246              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 247              		.loc 1 91 35 is_stmt 0
 248 01a1 8D7003   		lea	esi, [rax+3]	# ii,
 249              	.LVL22:
 250              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 251              		.loc 1 91 25
 252 01a4 39F3     		cmp	ebx, esi	# SIZE, ii
 253 01a6 7E6C     		jle	.L20	#,
  94:branchpred.c  ****               sum += data[ii];
 254              		.loc 1 94 13 is_stmt 1
 255              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 256              		.loc 1 94 21 is_stmt 0
 257 01a8 4963740D 		movsx	rsi, DWORD PTR [r13+12+rcx]	#, *_86
 257      0C
 258              	.LVL23:
 259 01ad 4989F2   		mov	r10, rsi	#,
 260              	# branchpred.c:95:               sum += data[ii];
 261              		.loc 1 95 19
 262 01b0 4C01E6   		add	rsi, r12	# tmp230, stmp_sum_31.19
 263 01b3 4181FA00 		cmp	r10d, 524288	# _85,
 263      000800
 264 01ba 4C0F4FE6 		cmovg	r12, rsi	# tmp230,, stmp_sum_31.19
 265              	.LVL24:
  91:branchpred.c  ****           {
 266              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 267              		.loc 1 91 25
 268              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 269              		.loc 1 91 35 is_stmt 0
 270 01be 8D7004   		lea	esi, [rax+4]	# ii,
 271              	.LVL25:
 272              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 273              		.loc 1 91 25
 274 01c1 39F3     		cmp	ebx, esi	# SIZE, ii
 275 01c3 7E4F     		jle	.L20	#,
  94:branchpred.c  ****               sum += data[ii];
 276              		.loc 1 94 13 is_stmt 1
 277              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 278              		.loc 1 94 21 is_stmt 0
 279 01c5 4963740D 		movsx	rsi, DWORD PTR [r13+16+rcx]	#, *_139
 279      10
 280              	.LVL26:
 281 01ca 4989F2   		mov	r10, rsi	#,
 282              	# branchpred.c:95:               sum += data[ii];
 283              		.loc 1 95 19
 284 01cd 4C01E6   		add	rsi, r12	# tmp232, stmp_sum_31.19
 285 01d0 4181FA00 		cmp	r10d, 524288	# _140,
 285      000800
 286 01d7 4C0F4FE6 		cmovg	r12, rsi	# tmp232,, stmp_sum_31.19
 287              	.LVL27:
  91:branchpred.c  ****           {
 288              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 289              		.loc 1 91 25
 290              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 291              		.loc 1 91 35 is_stmt 0
 292 01db 8D7005   		lea	esi, [rax+5]	# ii,
 293              	.LVL28:
 294              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 295              		.loc 1 91 25
 296 01de 39F3     		cmp	ebx, esi	# SIZE, ii
 297 01e0 7E32     		jle	.L20	#,
  94:branchpred.c  ****               sum += data[ii];
 298              		.loc 1 94 13 is_stmt 1
 299              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 300              		.loc 1 94 21 is_stmt 0
 301 01e2 4963740D 		movsx	rsi, DWORD PTR [r13+20+rcx]	#, *_149
 301      14
 302              	.LVL29:
 303 01e7 4989F2   		mov	r10, rsi	#,
 304              	# branchpred.c:95:               sum += data[ii];
 305              		.loc 1 95 19
 306 01ea 4C01E6   		add	rsi, r12	# tmp234, stmp_sum_31.19
 307 01ed 4181FA00 		cmp	r10d, 524288	# _150,
 307      000800
 308 01f4 4C0F4FE6 		cmovg	r12, rsi	# tmp234,, stmp_sum_31.19
 309              	.LVL30:
  91:branchpred.c  ****           {
 310              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 311              		.loc 1 91 25
 312              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 313              		.loc 1 91 35 is_stmt 0
 314 01f8 83C006   		add	eax, 6	# ii,
 315              	.LVL31:
 316              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 317              		.loc 1 91 25
 318 01fb 39C3     		cmp	ebx, eax	# SIZE, ii
 319 01fd 7E15     		jle	.L20	#,
  94:branchpred.c  ****               sum += data[ii];
 320              		.loc 1 94 13 is_stmt 1
 321              	# branchpred.c:94:             if (data[ii] > PIVOT)
  94:branchpred.c  ****               sum += data[ii];
 322              		.loc 1 94 21 is_stmt 0
 323 01ff 4963440D 		movsx	rax, DWORD PTR [r13+24+rcx]	#, *_97
 323      18
 324              	.LVL32:
 325 0204 4889C1   		mov	rcx, rax	#,
 326              	# branchpred.c:95:               sum += data[ii];
 327              		.loc 1 95 19
 328 0207 4C01E0   		add	rax, r12	# tmp236, stmp_sum_31.19
 329 020a 81F90000 		cmp	ecx, 524288	# _98,
 329      0800
 330 0210 4C0F4FE0 		cmovg	r12, rax	# tmp236,, stmp_sum_31.19
 331              	.LVL33:
 332              	.L20:
  87:branchpred.c  ****       {
 333              		.loc 1 87 29 is_stmt 1 discriminator 2
  87:branchpred.c  ****       {
 334              		.loc 1 87 19 discriminator 2
 335 0214 FFCF     		dec	edi	# ivtmp_70
 336 0216 0F85ACFE 		jne	.L6	#,
 336      FFFF
  96:branchpred.c  **** 
  97:branchpred.c  **** #elif defined( BESMART )
  98:branchpred.c  ****             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -1 is for the case d
  99:branchpred.c  ****             sum += ~t & data[ii];
 100:branchpred.c  **** 
 101:branchpred.c  **** #elif defined( BESMART2 )
 102:branchpred.c  **** 	    //sum += (data[ii]>PIVOT)*data[ii];
 103:branchpred.c  **** 	    sum += (data[ii]>PIVOT? data[ii] : 0);
 104:branchpred.c  **** #endif
 105:branchpred.c  ****           }
 106:branchpred.c  ****       }
 107:branchpred.c  **** 
 108:branchpred.c  ****   tstop = TCPU_TIME;
 337              		.loc 1 108 3
 338              	# branchpred.c:108:   tstop = TCPU_TIME;
 339              		.loc 1 108 11 is_stmt 0
 340 021c 488D75C0 		lea	rsi, [rbp-64]	# tmp258,
 341 0220 BF020000 		mov	edi, 2	#,
 341      00
 342 0225 C5F877   		vzeroupper
 343 0228 E8000000 		call	clock_gettime	#
 343      00
 344              	.LVL34:
 345 022d C5D857E4 		vxorps	xmm4, xmm4, xmm4	# tmp248
 346 0231 C4E1DB2A 		vcvtsi2sd	xmm0, xmm4, QWORD PTR [rbp-56]	# tmp251, tmp248, ts.tv_nsec
 346      45C8
 347 0237 C4E1DB2A 		vcvtsi2sd	xmm4, xmm4, QWORD PTR [rbp-64]	# tmp252, tmp248, ts.tv_sec
 347      65C0
 348              	# branchpred.c:114:   free(data);
 109:branchpred.c  ****   
 110:branchpred.c  **** #ifdef WOW
 111:branchpred.c  ****   tot_tstop = TCPU_TIME;
 112:branchpred.c  **** #endif
 113:branchpred.c  ****   
 114:branchpred.c  ****   free(data);
 349              		.loc 1 114 3
 350 023d 4C89EF   		mov	rdi, r13	#, data
 351              	# branchpred.c:108:   tstop = TCPU_TIME;
 108:branchpred.c  ****   
 352              		.loc 1 108 9
 353 0240 C4E2D999 		vfmadd132sd	xmm0, xmm4, QWORD PTR .LC0[rip]	# tstop, tmp215,
 353      05000000 
 353      00
 354 0249 C5FB1145 		vmovsd	QWORD PTR [rbp-72], xmm0	# %sfp, tstop
 354      B8
 355              	.LVL35:
 356              		.loc 1 114 3 is_stmt 1
 357 024e E8000000 		call	free	#
 357      00
 358              	.LVL36:
 115:branchpred.c  **** 
 116:branchpred.c  ****  #if !defined(WOW)
 117:branchpred.c  ****   printf("\nsum is %llu, elapsed seconds: %g\n", sum, tstop - tstart);
 359              		.loc 1 117 3
 360 0253 C5FB1045 		vmovsd	xmm0, QWORD PTR [rbp-72]	# tstop, %sfp
 360      B8
 361 0258 C4C1F96E 		vmovq	xmm5, r14	# tstart, tstart
 361      EE
 362 025d C5FB5CC5 		vsubsd	xmm0, xmm0, xmm5	# tmp217, tstop, tstart
 363 0261 4C89E6   		mov	rsi, r12	#, stmp_sum_31.19
 364 0264 BF000000 		mov	edi, OFFSET FLAT:.LC2	#,
 364      00
 365 0269 B8010000 		mov	eax, 1	#,
 365      00
 366 026e E8000000 		call	printf	#
 366      00
 367              	.LVL37:
 118:branchpred.c  **** 
 119:branchpred.c  **** #else
 120:branchpred.c  ****   double tot_time  = tot_tstop - tot_tstart;
 121:branchpred.c  ****   double loop_time = tstop - tstart;
 122:branchpred.c  ****   printf("\nsum is %llu, elapsed seconds: %g, %g in loop and %g in qsort\n",
 123:branchpred.c  **** 	 sum, tot_time, loop_time, tot_time - loop_time);
 124:branchpred.c  **** #endif
 125:branchpred.c  **** 
 126:branchpred.c  ****   printf("\n");
 368              		.loc 1 126 3
 369 0273 BF0A0000 		mov	edi, 10	#,
 369      00
 370 0278 E8000000 		call	putchar	#
 370      00
 371              	.LVL38:
 127:branchpred.c  ****   return 0;
 372              		.loc 1 127 3
 373              	# branchpred.c:128: }
 128:branchpred.c  **** }
 374              		.loc 1 128 1 is_stmt 0
 375 027d 4883C428 		add	rsp, 40	#,
 376 0281 5B       		pop	rbx	#
 377 0282 415A     		pop	r10	#
 378              		.cfi_remember_state
 379              		.cfi_def_cfa 10, 0
 380 0284 415C     		pop	r12	#
 381              	.LVL39:
 382 0286 415D     		pop	r13	#
 383              	.LVL40:
 384 0288 415E     		pop	r14	#
 385              	.LVL41:
 386 028a 31C0     		xor	eax, eax	#
 387 028c 5D       		pop	rbp	#
 388              	.LVL42:
 389 028d 498D62F8 		lea	rsp, [r10-8]	#,
 390              		.cfi_def_cfa 7, 8
 391 0291 C3       		ret	
 392              	.LVL43:
 393              	.L21:
 394              		.cfi_restore_state
 395              	# branchpred.c:89: 	sum = 0;
  89:branchpred.c  **** 
 396              		.loc 1 89 6
 397 0292 4531E4   		xor	r12d, r12d	# stmp_sum_31.19
 398              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 399              		.loc 1 91 17
 400 0295 31C0     		xor	eax, eax	# ii
 401 0297 E9A2FEFF 		jmp	.L7	#
 401      FF
 402              	.LVL44:
 403              	.L2:
  65:branchpred.c  ****   else
 404              		.loc 1 65 5 is_stmt 1
 405              	.LBB4:
 406              	.LBB5:
 407              		.file 2 "/usr/include/stdlib.h"
   1:/usr/include/stdlib.h **** /* Copyright (C) 1991-2007, 2009-2011, 2012 Free Software Foundation, Inc.
   2:/usr/include/stdlib.h ****    This file is part of the GNU C Library.
   3:/usr/include/stdlib.h **** 
   4:/usr/include/stdlib.h ****    The GNU C Library is free software; you can redistribute it and/or
   5:/usr/include/stdlib.h ****    modify it under the terms of the GNU Lesser General Public
   6:/usr/include/stdlib.h ****    License as published by the Free Software Foundation; either
   7:/usr/include/stdlib.h ****    version 2.1 of the License, or (at your option) any later version.
   8:/usr/include/stdlib.h **** 
   9:/usr/include/stdlib.h ****    The GNU C Library is distributed in the hope that it will be useful,
  10:/usr/include/stdlib.h ****    but WITHOUT ANY WARRANTY; without even the implied warranty of
  11:/usr/include/stdlib.h ****    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  12:/usr/include/stdlib.h ****    Lesser General Public License for more details.
  13:/usr/include/stdlib.h **** 
  14:/usr/include/stdlib.h ****    You should have received a copy of the GNU Lesser General Public
  15:/usr/include/stdlib.h ****    License along with the GNU C Library; if not, see
  16:/usr/include/stdlib.h ****    <http://www.gnu.org/licenses/>.  */
  17:/usr/include/stdlib.h **** 
  18:/usr/include/stdlib.h **** /*
  19:/usr/include/stdlib.h ****  *	ISO C99 Standard: 7.20 General utilities	<stdlib.h>
  20:/usr/include/stdlib.h ****  */
  21:/usr/include/stdlib.h **** 
  22:/usr/include/stdlib.h **** #ifndef	_STDLIB_H
  23:/usr/include/stdlib.h **** 
  24:/usr/include/stdlib.h **** #include <features.h>
  25:/usr/include/stdlib.h **** 
  26:/usr/include/stdlib.h **** /* Get size_t, wchar_t and NULL from <stddef.h>.  */
  27:/usr/include/stdlib.h **** #define		__need_size_t
  28:/usr/include/stdlib.h **** #ifndef __need_malloc_and_calloc
  29:/usr/include/stdlib.h **** # define	__need_wchar_t
  30:/usr/include/stdlib.h **** # define	__need_NULL
  31:/usr/include/stdlib.h **** #endif
  32:/usr/include/stdlib.h **** #include <stddef.h>
  33:/usr/include/stdlib.h **** 
  34:/usr/include/stdlib.h **** __BEGIN_DECLS
  35:/usr/include/stdlib.h **** 
  36:/usr/include/stdlib.h **** #ifndef __need_malloc_and_calloc
  37:/usr/include/stdlib.h **** #define	_STDLIB_H	1
  38:/usr/include/stdlib.h **** 
  39:/usr/include/stdlib.h **** #if (defined __USE_XOPEN || defined __USE_XOPEN2K8) && !defined _SYS_WAIT_H
  40:/usr/include/stdlib.h **** /* XPG requires a few symbols from <sys/wait.h> being defined.  */
  41:/usr/include/stdlib.h **** # include <bits/waitflags.h>
  42:/usr/include/stdlib.h **** # include <bits/waitstatus.h>
  43:/usr/include/stdlib.h **** 
  44:/usr/include/stdlib.h **** # ifdef __USE_BSD
  45:/usr/include/stdlib.h **** 
  46:/usr/include/stdlib.h **** /* Lots of hair to allow traditional BSD use of `union wait'
  47:/usr/include/stdlib.h ****    as well as POSIX.1 use of `int' for the status word.  */
  48:/usr/include/stdlib.h **** 
  49:/usr/include/stdlib.h **** #  if defined __GNUC__ && !defined __cplusplus
  50:/usr/include/stdlib.h **** #   define __WAIT_INT(status) \
  51:/usr/include/stdlib.h ****   (__extension__ (((union { __typeof(status) __in; int __i; }) \
  52:/usr/include/stdlib.h **** 		   { .__in = (status) }).__i))
  53:/usr/include/stdlib.h **** #  else
  54:/usr/include/stdlib.h **** #   define __WAIT_INT(status)	(*(int *) &(status))
  55:/usr/include/stdlib.h **** #  endif
  56:/usr/include/stdlib.h **** 
  57:/usr/include/stdlib.h **** /* This is the type of the argument to `wait'.  The funky union
  58:/usr/include/stdlib.h ****    causes redeclarations with either `int *' or `union wait *' to be
  59:/usr/include/stdlib.h ****    allowed without complaint.  __WAIT_STATUS_DEFN is the type used in
  60:/usr/include/stdlib.h ****    the actual function definitions.  */
  61:/usr/include/stdlib.h **** 
  62:/usr/include/stdlib.h **** #  if !defined __GNUC__ || __GNUC__ < 2 || defined __cplusplus
  63:/usr/include/stdlib.h **** #   define __WAIT_STATUS	void *
  64:/usr/include/stdlib.h **** #   define __WAIT_STATUS_DEFN	void *
  65:/usr/include/stdlib.h **** #  else
  66:/usr/include/stdlib.h **** /* This works in GCC 2.6.1 and later.  */
  67:/usr/include/stdlib.h **** typedef union
  68:/usr/include/stdlib.h ****   {
  69:/usr/include/stdlib.h ****     union wait *__uptr;
  70:/usr/include/stdlib.h ****     int *__iptr;
  71:/usr/include/stdlib.h ****   } __WAIT_STATUS __attribute__ ((__transparent_union__));
  72:/usr/include/stdlib.h **** #   define __WAIT_STATUS_DEFN	int *
  73:/usr/include/stdlib.h **** #  endif
  74:/usr/include/stdlib.h **** 
  75:/usr/include/stdlib.h **** # else /* Don't use BSD.  */
  76:/usr/include/stdlib.h **** 
  77:/usr/include/stdlib.h **** #  define __WAIT_INT(status)	(status)
  78:/usr/include/stdlib.h **** #  define __WAIT_STATUS		int *
  79:/usr/include/stdlib.h **** #  define __WAIT_STATUS_DEFN	int *
  80:/usr/include/stdlib.h **** 
  81:/usr/include/stdlib.h **** # endif /* Use BSD.  */
  82:/usr/include/stdlib.h **** 
  83:/usr/include/stdlib.h **** /* Define the macros <sys/wait.h> also would define this way.  */
  84:/usr/include/stdlib.h **** # define WEXITSTATUS(status)	__WEXITSTATUS (__WAIT_INT (status))
  85:/usr/include/stdlib.h **** # define WTERMSIG(status)	__WTERMSIG (__WAIT_INT (status))
  86:/usr/include/stdlib.h **** # define WSTOPSIG(status)	__WSTOPSIG (__WAIT_INT (status))
  87:/usr/include/stdlib.h **** # define WIFEXITED(status)	__WIFEXITED (__WAIT_INT (status))
  88:/usr/include/stdlib.h **** # define WIFSIGNALED(status)	__WIFSIGNALED (__WAIT_INT (status))
  89:/usr/include/stdlib.h **** # define WIFSTOPPED(status)	__WIFSTOPPED (__WAIT_INT (status))
  90:/usr/include/stdlib.h **** # ifdef __WIFCONTINUED
  91:/usr/include/stdlib.h **** #  define WIFCONTINUED(status)	__WIFCONTINUED (__WAIT_INT (status))
  92:/usr/include/stdlib.h **** # endif
  93:/usr/include/stdlib.h **** #endif	/* X/Open or XPG7 and <sys/wait.h> not included.  */
  94:/usr/include/stdlib.h **** 
  95:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_STD
  96:/usr/include/stdlib.h **** /* Returned by `div'.  */
  97:/usr/include/stdlib.h **** typedef struct
  98:/usr/include/stdlib.h ****   {
  99:/usr/include/stdlib.h ****     int quot;			/* Quotient.  */
 100:/usr/include/stdlib.h ****     int rem;			/* Remainder.  */
 101:/usr/include/stdlib.h ****   } div_t;
 102:/usr/include/stdlib.h **** 
 103:/usr/include/stdlib.h **** /* Returned by `ldiv'.  */
 104:/usr/include/stdlib.h **** #ifndef __ldiv_t_defined
 105:/usr/include/stdlib.h **** typedef struct
 106:/usr/include/stdlib.h ****   {
 107:/usr/include/stdlib.h ****     long int quot;		/* Quotient.  */
 108:/usr/include/stdlib.h ****     long int rem;		/* Remainder.  */
 109:/usr/include/stdlib.h ****   } ldiv_t;
 110:/usr/include/stdlib.h **** # define __ldiv_t_defined	1
 111:/usr/include/stdlib.h **** #endif
 112:/usr/include/stdlib.h **** __END_NAMESPACE_STD
 113:/usr/include/stdlib.h **** 
 114:/usr/include/stdlib.h **** #if defined __USE_ISOC99 && !defined __lldiv_t_defined
 115:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_C99
 116:/usr/include/stdlib.h **** /* Returned by `lldiv'.  */
 117:/usr/include/stdlib.h **** __extension__ typedef struct
 118:/usr/include/stdlib.h ****   {
 119:/usr/include/stdlib.h ****     long long int quot;		/* Quotient.  */
 120:/usr/include/stdlib.h ****     long long int rem;		/* Remainder.  */
 121:/usr/include/stdlib.h ****   } lldiv_t;
 122:/usr/include/stdlib.h **** # define __lldiv_t_defined	1
 123:/usr/include/stdlib.h **** __END_NAMESPACE_C99
 124:/usr/include/stdlib.h **** #endif
 125:/usr/include/stdlib.h **** 
 126:/usr/include/stdlib.h **** 
 127:/usr/include/stdlib.h **** /* The largest number rand will return (same as INT_MAX).  */
 128:/usr/include/stdlib.h **** #define	RAND_MAX	2147483647
 129:/usr/include/stdlib.h **** 
 130:/usr/include/stdlib.h **** 
 131:/usr/include/stdlib.h **** /* We define these the same for all machines.
 132:/usr/include/stdlib.h ****    Changes from this to the outside world should be done in `_exit'.  */
 133:/usr/include/stdlib.h **** #define	EXIT_FAILURE	1	/* Failing exit status.  */
 134:/usr/include/stdlib.h **** #define	EXIT_SUCCESS	0	/* Successful exit status.  */
 135:/usr/include/stdlib.h **** 
 136:/usr/include/stdlib.h **** 
 137:/usr/include/stdlib.h **** /* Maximum length of a multibyte character in the current locale.  */
 138:/usr/include/stdlib.h **** #define	MB_CUR_MAX	(__ctype_get_mb_cur_max ())
 139:/usr/include/stdlib.h **** extern size_t __ctype_get_mb_cur_max (void) __THROW __wur;
 140:/usr/include/stdlib.h **** 
 141:/usr/include/stdlib.h **** 
 142:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_STD
 143:/usr/include/stdlib.h **** /* Convert a string to a floating-point number.  */
 144:/usr/include/stdlib.h **** extern double atof (const char *__nptr)
 145:/usr/include/stdlib.h ****      __THROW __attribute_pure__ __nonnull ((1)) __wur;
 146:/usr/include/stdlib.h **** /* Convert a string to an integer.  */
 147:/usr/include/stdlib.h **** extern int atoi (const char *__nptr)
 148:/usr/include/stdlib.h ****      __THROW __attribute_pure__ __nonnull ((1)) __wur;
 149:/usr/include/stdlib.h **** /* Convert a string to a long integer.  */
 150:/usr/include/stdlib.h **** extern long int atol (const char *__nptr)
 151:/usr/include/stdlib.h ****      __THROW __attribute_pure__ __nonnull ((1)) __wur;
 152:/usr/include/stdlib.h **** __END_NAMESPACE_STD
 153:/usr/include/stdlib.h **** 
 154:/usr/include/stdlib.h **** #if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
 155:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_C99
 156:/usr/include/stdlib.h **** /* Convert a string to a long long integer.  */
 157:/usr/include/stdlib.h **** __extension__ extern long long int atoll (const char *__nptr)
 158:/usr/include/stdlib.h ****      __THROW __attribute_pure__ __nonnull ((1)) __wur;
 159:/usr/include/stdlib.h **** __END_NAMESPACE_C99
 160:/usr/include/stdlib.h **** #endif
 161:/usr/include/stdlib.h **** 
 162:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_STD
 163:/usr/include/stdlib.h **** /* Convert a string to a floating-point number.  */
 164:/usr/include/stdlib.h **** extern double strtod (const char *__restrict __nptr,
 165:/usr/include/stdlib.h **** 		      char **__restrict __endptr)
 166:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 167:/usr/include/stdlib.h **** __END_NAMESPACE_STD
 168:/usr/include/stdlib.h **** 
 169:/usr/include/stdlib.h **** #ifdef	__USE_ISOC99
 170:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_C99
 171:/usr/include/stdlib.h **** /* Likewise for `float' and `long double' sizes of floating-point numbers.  */
 172:/usr/include/stdlib.h **** extern float strtof (const char *__restrict __nptr,
 173:/usr/include/stdlib.h **** 		     char **__restrict __endptr) __THROW __nonnull ((1));
 174:/usr/include/stdlib.h **** 
 175:/usr/include/stdlib.h **** extern long double strtold (const char *__restrict __nptr,
 176:/usr/include/stdlib.h **** 			    char **__restrict __endptr)
 177:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 178:/usr/include/stdlib.h **** __END_NAMESPACE_C99
 179:/usr/include/stdlib.h **** #endif
 180:/usr/include/stdlib.h **** 
 181:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_STD
 182:/usr/include/stdlib.h **** /* Convert a string to a long integer.  */
 183:/usr/include/stdlib.h **** extern long int strtol (const char *__restrict __nptr,
 184:/usr/include/stdlib.h **** 			char **__restrict __endptr, int __base)
 185:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 186:/usr/include/stdlib.h **** /* Convert a string to an unsigned long integer.  */
 187:/usr/include/stdlib.h **** extern unsigned long int strtoul (const char *__restrict __nptr,
 188:/usr/include/stdlib.h **** 				  char **__restrict __endptr, int __base)
 189:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 190:/usr/include/stdlib.h **** __END_NAMESPACE_STD
 191:/usr/include/stdlib.h **** 
 192:/usr/include/stdlib.h **** #if defined __GLIBC_HAVE_LONG_LONG && defined __USE_BSD
 193:/usr/include/stdlib.h **** /* Convert a string to a quadword integer.  */
 194:/usr/include/stdlib.h **** __extension__
 195:/usr/include/stdlib.h **** extern long long int strtoq (const char *__restrict __nptr,
 196:/usr/include/stdlib.h **** 			     char **__restrict __endptr, int __base)
 197:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 198:/usr/include/stdlib.h **** /* Convert a string to an unsigned quadword integer.  */
 199:/usr/include/stdlib.h **** __extension__
 200:/usr/include/stdlib.h **** extern unsigned long long int strtouq (const char *__restrict __nptr,
 201:/usr/include/stdlib.h **** 				       char **__restrict __endptr, int __base)
 202:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 203:/usr/include/stdlib.h **** #endif /* GCC and use BSD.  */
 204:/usr/include/stdlib.h **** 
 205:/usr/include/stdlib.h **** #if defined __USE_ISOC99 || (defined __GLIBC_HAVE_LONG_LONG && defined __USE_MISC)
 206:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_C99
 207:/usr/include/stdlib.h **** /* Convert a string to a quadword integer.  */
 208:/usr/include/stdlib.h **** __extension__
 209:/usr/include/stdlib.h **** extern long long int strtoll (const char *__restrict __nptr,
 210:/usr/include/stdlib.h **** 			      char **__restrict __endptr, int __base)
 211:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 212:/usr/include/stdlib.h **** /* Convert a string to an unsigned quadword integer.  */
 213:/usr/include/stdlib.h **** __extension__
 214:/usr/include/stdlib.h **** extern unsigned long long int strtoull (const char *__restrict __nptr,
 215:/usr/include/stdlib.h **** 					char **__restrict __endptr, int __base)
 216:/usr/include/stdlib.h ****      __THROW __nonnull ((1));
 217:/usr/include/stdlib.h **** __END_NAMESPACE_C99
 218:/usr/include/stdlib.h **** #endif /* ISO C99 or GCC and use MISC.  */
 219:/usr/include/stdlib.h **** 
 220:/usr/include/stdlib.h **** 
 221:/usr/include/stdlib.h **** #ifdef __USE_GNU
 222:/usr/include/stdlib.h **** /* The concept of one static locale per category is not very well
 223:/usr/include/stdlib.h ****    thought out.  Many applications will need to process its data using
 224:/usr/include/stdlib.h ****    information from several different locales.  Another problem is
 225:/usr/include/stdlib.h ****    the implementation of the internationalization handling in the
 226:/usr/include/stdlib.h ****    ISO C++ standard library.  To support this another set of
 227:/usr/include/stdlib.h ****    the functions using locale data exist which take an additional
 228:/usr/include/stdlib.h ****    argument.
 229:/usr/include/stdlib.h **** 
 230:/usr/include/stdlib.h ****    Attention: even though several *_l interfaces are part of POSIX:2008,
 231:/usr/include/stdlib.h ****    these are not.  */
 232:/usr/include/stdlib.h **** 
 233:/usr/include/stdlib.h **** /* Structure for reentrant locale using functions.  This is an
 234:/usr/include/stdlib.h ****    (almost) opaque type for the user level programs.  */
 235:/usr/include/stdlib.h **** # include <xlocale.h>
 236:/usr/include/stdlib.h **** 
 237:/usr/include/stdlib.h **** /* Special versions of the functions above which take the locale to
 238:/usr/include/stdlib.h ****    use as an additional parameter.  */
 239:/usr/include/stdlib.h **** extern long int strtol_l (const char *__restrict __nptr,
 240:/usr/include/stdlib.h **** 			  char **__restrict __endptr, int __base,
 241:/usr/include/stdlib.h **** 			  __locale_t __loc) __THROW __nonnull ((1, 4));
 242:/usr/include/stdlib.h **** 
 243:/usr/include/stdlib.h **** extern unsigned long int strtoul_l (const char *__restrict __nptr,
 244:/usr/include/stdlib.h **** 				    char **__restrict __endptr,
 245:/usr/include/stdlib.h **** 				    int __base, __locale_t __loc)
 246:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 4));
 247:/usr/include/stdlib.h **** 
 248:/usr/include/stdlib.h **** __extension__
 249:/usr/include/stdlib.h **** extern long long int strtoll_l (const char *__restrict __nptr,
 250:/usr/include/stdlib.h **** 				char **__restrict __endptr, int __base,
 251:/usr/include/stdlib.h **** 				__locale_t __loc)
 252:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 4));
 253:/usr/include/stdlib.h **** 
 254:/usr/include/stdlib.h **** __extension__
 255:/usr/include/stdlib.h **** extern unsigned long long int strtoull_l (const char *__restrict __nptr,
 256:/usr/include/stdlib.h **** 					  char **__restrict __endptr,
 257:/usr/include/stdlib.h **** 					  int __base, __locale_t __loc)
 258:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 4));
 259:/usr/include/stdlib.h **** 
 260:/usr/include/stdlib.h **** extern double strtod_l (const char *__restrict __nptr,
 261:/usr/include/stdlib.h **** 			char **__restrict __endptr, __locale_t __loc)
 262:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 3));
 263:/usr/include/stdlib.h **** 
 264:/usr/include/stdlib.h **** extern float strtof_l (const char *__restrict __nptr,
 265:/usr/include/stdlib.h **** 		       char **__restrict __endptr, __locale_t __loc)
 266:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 3));
 267:/usr/include/stdlib.h **** 
 268:/usr/include/stdlib.h **** extern long double strtold_l (const char *__restrict __nptr,
 269:/usr/include/stdlib.h **** 			      char **__restrict __endptr,
 270:/usr/include/stdlib.h **** 			      __locale_t __loc)
 271:/usr/include/stdlib.h ****      __THROW __nonnull ((1, 3));
 272:/usr/include/stdlib.h **** #endif /* GNU */
 273:/usr/include/stdlib.h **** 
 274:/usr/include/stdlib.h **** 
 275:/usr/include/stdlib.h **** #ifdef __USE_EXTERN_INLINES
 276:/usr/include/stdlib.h **** __BEGIN_NAMESPACE_STD
 277:/usr/include/stdlib.h **** __extern_inline int
 278:/usr/include/stdlib.h **** __NTH (atoi (const char *__nptr))
 279:/usr/include/stdlib.h **** {
 280:/usr/include/stdlib.h ****   return (int) strtol (__nptr, (char **) NULL, 10);
 408              		.loc 2 280 3
 409              	# /usr/include/stdlib.h:280:   return (int) strtol (__nptr, (char **) NULL, 10);
 410              		.loc 2 280 16 is_stmt 0
 411 029c 488B7E08 		mov	rdi, QWORD PTR [rsi+8]	# MEM[(char * *)argv_36(D) + 8B], MEM[(char * *)argv_36(D) + 8B]
 412              	.LVL45:
 413 02a0 BA0A0000 		mov	edx, 10	#,
 413      00
 414 02a5 31F6     		xor	esi, esi	#
 415              	.LVL46:
 416 02a7 E8000000 		call	strtol	#
 416      00
 417              	.LVL47:
 418 02ac 4989C4   		mov	r12, rax	# _55, tmp245
 419              	.LBE5:
 420              	.LBE4:
 421              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 422              		.loc 1 70 16
 423 02af 4863F8   		movsx	rdi, eax	# SIZE, _55
 424 02b2 BE040000 		mov	esi, 4	#,
 424      00
 425              	.LBB7:
 426              	.LBB6:
 427              	# /usr/include/stdlib.h:280:   return (int) strtol (__nptr, (char **) NULL, 10);
 428              		.loc 2 280 10
 429 02b7 89C3     		mov	ebx, eax	# SIZE, _55
 430              	.LVL48:
 431              	.LBE6:
 432              	.LBE7:
  70:branchpred.c  ****   srand((int)(SIZE));
 433              		.loc 1 70 3 is_stmt 1
 434              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 435              		.loc 1 70 16 is_stmt 0
 436 02b9 E8000000 		call	calloc	#
 436      00
 437              	.LVL49:
 438              	# branchpred.c:71:   srand((int)(SIZE));
  71:branchpred.c  ****   
 439              		.loc 1 71 3
 440 02be 4489E7   		mov	edi, r12d	#, SIZE
 441              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 442              		.loc 1 70 16
 443 02c1 4989C5   		mov	r13, rax	# data, tmp246
 444              	.LVL50:
  71:branchpred.c  ****   
 445              		.loc 1 71 3 is_stmt 1
 446 02c4 E8000000 		call	srand	#
 446      00
 447              	.LVL51:
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 448              		.loc 1 73 3
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 449              		.loc 1 73 19
 450 02c9 4585E4   		test	r12d, r12d	# _55
 451 02cc 0F8F76FD 		jg	.L3	#,
 451      FFFF
 452 02d2 E993FDFF 		jmp	.L4	#
 452      FF
 453              		.cfi_endproc
 454              	.LFE32:
 456              		.section	.rodata.cst8,"aM",@progbits,8
 457              		.align 8
 458              	.LC0:
 459 0000 95D626E8 		.long	-400107883
 460 0004 0B2E113E 		.long	1041313291
 461              		.section	.rodata.cst4,"aM",@progbits,4
 462              		.align 4
 463              	.LC3:
 464 0000 00000800 		.long	524288
 465              		.text
 466              	.Letext0:
 467              		.file 3 "/opt/cluster/spack/opt/spack/linux-centos7-haswell/gcc-4.8.5/gcc-11.2.0-35ynawgsywf7z6zq4
 468              		.file 4 "/usr/include/bits/types.h"
 469              		.file 5 "/usr/include/time.h"
 470              		.file 6 "/usr/include/stdio.h"
 471              		.file 7 "<built-in>"
