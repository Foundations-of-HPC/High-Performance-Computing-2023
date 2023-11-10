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
  11              	.LC3:
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
  52 001e 83FF01   		cmp	edi, 1	# tmp272,
  53 0021 0F8F6602 		jg	.L2	#,
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
  69 003b 4989C5   		mov	r13, rax	# data, tmp274
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
  84 0048 4531E4   		xor	r12d, r12d	# ivtmp.55
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
  95 0051 C1EA0B   		shr	edx, 11	# tmp198,
  96 0054 01D0     		add	eax, edx	# tmp199, tmp198
  97 0056 25FFFF1F 		and	eax, 2097151	# tmp200,
  97      00
  98 005b 29D0     		sub	eax, edx	# tmp201, tmp198
  99              	# branchpred.c:74:     data[cc] = rand() % TOP;
 100              		.loc 1 74 14 discriminator 3
 101 005d 438944A5 		mov	DWORD PTR [r13+0+r12*4], eax	# MEM[(int *)data_73 + ivtmp.55_173 * 4], tmp201
 101      00
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 102              		.loc 1 73 29 is_stmt 1 discriminator 3
 103              	.LVL9:
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 104              		.loc 1 73 19 discriminator 3
 105 0062 49FFC4   		inc	r12	# ivtmp.55
 106              	.LVL10:
 107 0065 4439E3   		cmp	ebx, r12d	# SIZE, ivtmp.55
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
 113 006a 488D75C0 		lea	rsi, [rbp-64]	# tmp284,
 114 006e BF020000 		mov	edi, 2	#,
 114      00
 115 0073 E8000000 		call	clock_gettime	#
 115      00
 116              	.LVL11:
 117 0078 C5D057ED 		vxorps	xmm5, xmm5, xmm5	# tmp278
 118 007c C4E1D32A 		vcvtsi2sd	xmm0, xmm5, QWORD PTR [rbp-56]	# tmp279, tmp278, ts.tv_nsec
 118      45C8
 119 0082 89D8     		mov	eax, ebx	# bnd.8, SIZE
 120 0084 C1E803   		shr	eax, 3	# bnd.8,
 121 0087 FFC8     		dec	eax	# tmp208
 122 0089 C5FB10C8 		vmovsd	xmm1, xmm0, xmm0	# tmp203, tmp279
 123 008d C4E1D32A 		vcvtsi2sd	xmm0, xmm5, QWORD PTR [rbp-64]	# tmp280, tmp278, ts.tv_sec
 123      45C0
 124 0093 48C1E005 		sal	rax, 5	# tmp209,
 125 0097 89DF     		mov	edi, ebx	# niters_vector_mult_vf.9, SIZE
 126              	.LBB5:
 127              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  86:branchpred.c  ****   
  87:branchpred.c  ****   for (cc = 0; cc < 1000; cc++)
  88:branchpred.c  ****       {
  89:branchpred.c  **** 	sum = 0;
  90:branchpred.c  **** 
  91:branchpred.c  ****         for (ii = 0; ii < SIZE; ii++)
  92:branchpred.c  ****           {
  93:branchpred.c  **** #if !defined( BESMART ) && !defined( BESMART2 )
  94:branchpred.c  ****             if (data[ii] > PIVOT)
  95:branchpred.c  ****               sum += data[ii];
  96:branchpred.c  **** 
  97:branchpred.c  **** #elif defined( BESMART )
  98:branchpred.c  ****             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -1 is for the case d
 128              		.loc 1 98 48
 129 0099 C4E27958 		vpbroadcastd	xmm4, DWORD PTR .LC4[rip]	# tmp267,
 129      25000000 
 129      00
 130              	.LBE5:
 131              	# branchpred.c:85:   tstart = TCPU_TIME;
  85:branchpred.c  ****   
 132              		.loc 1 85 10
 133 00a2 C4E2F999 		vfmadd132sd	xmm1, xmm0, QWORD PTR .LC0[rip]	# tmp203, tmp204,
 133      0D000000 
 133      00
 134 00ab C4E27D58 		vpbroadcastd	ymm3, DWORD PTR .LC4[rip]	# tmp270,
 134      1D000000 
 134      00
 135 00b4 448D43FF 		lea	r8d, [rbx-1]	# _93,
 136 00b8 498D4C05 		lea	rcx, [r13+32+rax]	# _180,
 136      20
 137 00bd 83E7F8   		and	edi, -8	# niters_vector_mult_vf.9,
 138 00c0 C4C1F97E 		vmovq	r14, xmm1	# tstart, tmp203
 138      CE
 139              	.LVL12:
  87:branchpred.c  ****       {
 140              		.loc 1 87 3 is_stmt 1
  87:branchpred.c  ****       {
 141              		.loc 1 87 19
 142              	# branchpred.c:85:   tstart = TCPU_TIME;
  85:branchpred.c  ****   
 143              		.loc 1 85 10 is_stmt 0
 144 00c5 BEE80300 		mov	esi, 1000	# ivtmp_116,
 144      00
 145              	.LVL13:
 146 00ca 660F1F44 		.p2align 4,,10
 146      0000
 147              		.p2align 3
 148              	.L6:
  91:branchpred.c  ****           {
 149              		.loc 1 91 25 is_stmt 1
 150              	# branchpred.c:89: 	sum = 0;
  89:branchpred.c  **** 
 151              		.loc 1 89 6 is_stmt 0
 152 00d0 4531E4   		xor	r12d, r12d	# stmp_sum_55.23
 153              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 154              		.loc 1 91 25
 155 00d3 85DB     		test	ebx, ebx	# SIZE
 156 00d5 0F8E2801 		jle	.L14	#,
 156      0000
 157 00db 4183F806 		cmp	r8d, 6	# _93,
 158 00df 0F869C01 		jbe	.L15	#,
 158      0000
 159 00e5 4C89E8   		mov	rax, r13	# ivtmp.46, data
 160 00e8 C5F1EFC9 		vpxor	xmm1, xmm1, xmm1	# vect_sum_66.11
 161              	.LVL14:
 162 00ec 0F1F4000 		.p2align 4,,10
 163              		.p2align 3
 164              	.L8:
 165              	.LBB6:
 166              		.loc 1 98 13 is_stmt 1 discriminator 3
 167              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
 168              		.loc 1 98 35 is_stmt 0 discriminator 3
 169 00f0 C5FE6F10 		vmovdqu	ymm2, YMMWORD PTR [rax]	# MEM <vector(8) int> [(int *)_130], MEM <vector(8) int> [(int *)_
 170              	.LVL15:
  99:branchpred.c  ****             sum += ~t & data[ii];
 171              		.loc 1 99 13 is_stmt 1 discriminator 3
 172 00f4 4883C020 		add	rax, 32	# ivtmp.46,
 173              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 174              		.loc 1 98 48 is_stmt 0 discriminator 3
 175 00f8 C5EDFEC3 		vpaddd	ymm0, ymm2, ymm3	# vect__18.15, MEM <vector(8) int> [(int *)_130], tmp270
 176              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 177              		.loc 1 98 53 discriminator 3
 178 00fc C5FD72E0 		vpsrad	ymm0, ymm0, 31	# vect__19.16, vect__18.15,
 178      1F
 179              	# branchpred.c:99:             sum += ~t & data[ii];
 180              		.loc 1 99 23 discriminator 3
 181 0101 C5FDDFC2 		vpandn	ymm0, ymm0, ymm2	# vect__24.20, vect__19.16, MEM <vector(8) int> [(int *)_130]
 182 0105 C4E27D35 		vpmovzxdq	ymm2, xmm0	#, vect__24.20
 182      D0
 183 010a C4E37D39 		vextracti128	xmm0, ymm0, 0x1	# tmp219, vect__24.20
 183      C001
 184              	# branchpred.c:99:             sum += ~t & data[ii];
 185              		.loc 1 99 17 discriminator 3
 186 0110 C5EDD4C9 		vpaddq	ymm1, ymm2, ymm1	# vect_sum_55.22, vect__25.21, vect_sum_66.11
 187              	# branchpred.c:99:             sum += ~t & data[ii];
 188              		.loc 1 99 23 discriminator 3
 189 0114 C4E27D35 		vpmovzxdq	ymm0, xmm0	# vect__25.21, tmp219
 189      C0
 190              	# branchpred.c:99:             sum += ~t & data[ii];
 191              		.loc 1 99 17 discriminator 3
 192 0119 C5FDD4C9 		vpaddq	ymm1, ymm0, ymm1	# vect_sum_66.11, vect__25.21, vect_sum_55.22
 193              	.LBE6:
  91:branchpred.c  ****           {
 194              		.loc 1 91 35 is_stmt 1 discriminator 3
  91:branchpred.c  ****           {
 195              		.loc 1 91 25 discriminator 3
 196 011d 4839C8   		cmp	rax, rcx	# ivtmp.46, _180
 197 0120 75CE     		jne	.L8	#,
 198 0122 C5F96FC1 		vmovdqa	xmm0, xmm1	# tmp220, vect_sum_66.11
 199 0126 62F3FD28 		vextracti64x2	xmm1, ymm1, 0x1	# tmp221, vect_sum_66.11
 199      39C901
 200 012d C5F9D4C1 		vpaddq	xmm0, xmm0, xmm1	# _122, tmp220, tmp221
 201 0131 C5F173D8 		vpsrldq	xmm1, xmm0, 8	# tmp223, _122,
 201      08
 202 0136 C5F9D4C1 		vpaddq	xmm0, xmm0, xmm1	# tmp224, _122, tmp223
 203 013a C4C1F97E 		vmovq	r12, xmm0	# stmp_sum_55.23, tmp224
 203      C4
 204 013f 39DF     		cmp	edi, ebx	# niters_vector_mult_vf.9, SIZE
 205 0141 0F84BC00 		je	.L14	#,
 205      0000
 206              	.LBB7:
 207              	# branchpred.c:99:             sum += ~t & data[ii];
 208              		.loc 1 99 17 is_stmt 0
 209 0147 89F8     		mov	eax, edi	#, niters_vector_mult_vf.9
 210              	.LBE7:
 211              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 212              		.loc 1 91 17
 213 0149 89FA     		mov	edx, edi	# tmp.27, niters_vector_mult_vf.9
 214              	.L7:
 215 014b 4189D9   		mov	r9d, ebx	# niters.24, SIZE
 216 014e 4129C1   		sub	r9d, eax	# niters.24, _86
 217 0151 458D51FF 		lea	r10d, [r9-1]	# tmp225,
 218 0155 4183FA02 		cmp	r10d, 2	# tmp225,
 219 0159 7645     		jbe	.L10	#,
 220              	.LBB8:
  98:branchpred.c  ****             sum += ~t & data[ii];
 221              		.loc 1 98 13 is_stmt 1
 222              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 223              		.loc 1 98 35 is_stmt 0
 224 015b C4C17A6F 		vmovdqu	xmm1, XMMWORD PTR [r13+0+rax*4]	# MEM <vector(4) int> [(int *)vectp_data.30_158], MEM <vec
 224      4C8500
 225              		.loc 1 99 13 is_stmt 1
 226              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 227              		.loc 1 98 48 is_stmt 0
 228 0162 C5F1FEC4 		vpaddd	xmm0, xmm1, xmm4	# vect__104.32, MEM <vector(4) int> [(int *)vectp_data.30_158], tmp267
 229              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 230              		.loc 1 98 53
 231 0166 C5F972E0 		vpsrad	xmm0, xmm0, 31	# vect__103.33, vect__104.32,
 231      1F
 232              	# branchpred.c:99:             sum += ~t & data[ii];
 233              		.loc 1 99 23
 234 016b C5F9DFC1 		vpandn	xmm0, xmm0, xmm1	# vect__99.37, vect__103.33, MEM <vector(4) int> [(int *)vectp_data.30_158
 235 016f C4E27935 		vpmovzxdq	xmm1, xmm0	# vect__98.38, vect__99.37
 235      C8
 236 0174 C5F973D8 		vpsrldq	xmm0, xmm0, 8	# tmp234, vect__99.37,
 236      08
 237 0179 C4E27935 		vpmovzxdq	xmm0, xmm0	# vect__98.38, tmp234
 237      C0
 238              	# branchpred.c:99:             sum += ~t & data[ii];
 239              		.loc 1 99 17
 240 017e C5F1D4C0 		vpaddq	xmm0, xmm1, xmm0	# vect_sum_97.39, vect__98.38, vect__98.38
 241              	.LBE8:
  91:branchpred.c  ****           {
 242              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 243              		.loc 1 91 25
 244 0182 C5F173D8 		vpsrldq	xmm1, xmm0, 8	# tmp236, vect_sum_97.39,
 244      08
 245 0187 C5F9D4C1 		vpaddq	xmm0, xmm0, xmm1	# tmp237, vect_sum_97.39, tmp236
 246 018b C4E1F97E 		vmovq	rax, xmm0	# stmp_sum_97.40, tmp237
 246      C0
 247 0190 4901C4   		add	r12, rax	# stmp_sum_55.23, stmp_sum_97.40
 248 0193 4489C8   		mov	eax, r9d	# niters_vector_mult_vf.26, niters.24
 249 0196 83E0FC   		and	eax, -4	# niters_vector_mult_vf.26,
 250 0199 01C2     		add	edx, eax	# tmp.27, niters_vector_mult_vf.26
 251 019b 4139C1   		cmp	r9d, eax	# niters.24, niters_vector_mult_vf.26
 252 019e 7463     		je	.L14	#,
 253              	.L10:
 254              	.LVL16:
 255              	.LBB9:
  98:branchpred.c  ****             sum += ~t & data[ii];
 256              		.loc 1 98 13
 257              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 258              		.loc 1 98 35 is_stmt 0
 259 01a0 4863C2   		movsx	rax, edx	# tmp.27, tmp.27
 260 01a3 458B5485 		mov	r10d, DWORD PTR [r13+0+rax*4]	# _15, *_14
 260      00
 261              	.LVL17:
 262              		.loc 1 99 13 is_stmt 1
 263              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 264              		.loc 1 98 35 is_stmt 0
 265 01a8 4C8D0C85 		lea	r9, [0+rax*4]	# _68,
 265      00000000 
 266              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 267              		.loc 1 98 48
 268 01b0 418D82FF 		lea	eax, [r10-524289]	# tmp240,
 268      FFF7FF
 269              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 270              		.loc 1 98 53
 271 01b7 C1F81F   		sar	eax, 31	# t,
 272              	.LVL18:
 273              	# branchpred.c:99:             sum += ~t & data[ii];
 274              		.loc 1 99 23
 275 01ba C4C278F2 		andn	eax, eax, r10d	# tmp243, t, _15
 275      C2
 276              	.LVL19:
 277 01bf 89C0     		mov	eax, eax	# tmp244, tmp243
 278              	# branchpred.c:99:             sum += ~t & data[ii];
 279              		.loc 1 99 17
 280 01c1 4901C4   		add	r12, rax	# stmp_sum_55.23, tmp244
 281              	.LVL20:
 282              	.LBE9:
  91:branchpred.c  ****           {
 283              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 284              		.loc 1 91 25
 285              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 286              		.loc 1 91 35 is_stmt 0
 287 01c4 8D4201   		lea	eax, [rdx+1]	# ii,
 288              	.LVL21:
 289              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 290              		.loc 1 91 25
 291 01c7 39C3     		cmp	ebx, eax	# SIZE, ii
 292 01c9 7E38     		jle	.L14	#,
 293              	.LBB10:
  98:branchpred.c  ****             sum += ~t & data[ii];
 294              		.loc 1 98 13 is_stmt 1
 295              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 296              		.loc 1 98 35 is_stmt 0
 297 01cb 478B540D 		mov	r10d, DWORD PTR [r13+4+r9]	# _107, *_108
 297      04
 298              	.LVL22:
 299              		.loc 1 99 13 is_stmt 1
 300              	.LBE10:
 301              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 302              		.loc 1 91 35 is_stmt 0
 303 01d0 83C202   		add	edx, 2	# ii,
 304              	.LBB11:
 305              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 306              		.loc 1 98 48
 307 01d3 418D82FF 		lea	eax, [r10-524289]	# tmp246,
 307      FFF7FF
 308              	.LVL23:
 309              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 310              		.loc 1 98 53
 311 01da C1F81F   		sar	eax, 31	# t,
 312              	.LVL24:
 313              	# branchpred.c:99:             sum += ~t & data[ii];
 314              		.loc 1 99 23
 315 01dd C4C278F2 		andn	eax, eax, r10d	# tmp249, t, _107
 315      C2
 316              	.LVL25:
 317 01e2 89C0     		mov	eax, eax	# tmp250, tmp249
 318              	# branchpred.c:99:             sum += ~t & data[ii];
 319              		.loc 1 99 17
 320 01e4 4901C4   		add	r12, rax	# stmp_sum_55.23, tmp250
 321              	.LVL26:
 322              	.LBE11:
  91:branchpred.c  ****           {
 323              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 324              		.loc 1 91 25
 325 01e7 39D3     		cmp	ebx, edx	# SIZE, ii
 326 01e9 7E18     		jle	.L14	#,
 327              	.LBB12:
  98:branchpred.c  ****             sum += ~t & data[ii];
 328              		.loc 1 98 13
 329              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 330              		.loc 1 98 35 is_stmt 0
 331 01eb 438B540D 		mov	edx, DWORD PTR [r13+8+r9]	# _136, *_135
 331      08
 332              	.LVL27:
 333              		.loc 1 99 13 is_stmt 1
 334              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 335              		.loc 1 98 48 is_stmt 0
 336 01f0 8D82FFFF 		lea	eax, [rdx-524289]	# tmp252,
 336      F7FF
 337              	# branchpred.c:98:             unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -
  98:branchpred.c  ****             sum += ~t & data[ii];
 338              		.loc 1 98 53
 339 01f6 C1F81F   		sar	eax, 31	# t,
 340              	.LVL28:
 341              	# branchpred.c:99:             sum += ~t & data[ii];
 342              		.loc 1 99 23
 343 01f9 C4E278F2 		andn	eax, eax, edx	# tmp255, t, _136
 343      C2
 344              	.LVL29:
 345 01fe 89C0     		mov	eax, eax	# tmp256, tmp255
 346              	# branchpred.c:99:             sum += ~t & data[ii];
 347              		.loc 1 99 17
 348 0200 4901C4   		add	r12, rax	# stmp_sum_55.23, tmp256
 349              	.LVL30:
 350              	.LBE12:
  91:branchpred.c  ****           {
 351              		.loc 1 91 35 is_stmt 1
  91:branchpred.c  ****           {
 352              		.loc 1 91 25
 353              	.L14:
  87:branchpred.c  ****       {
 354              		.loc 1 87 29 discriminator 2
  87:branchpred.c  ****       {
 355              		.loc 1 87 19 discriminator 2
 356 0203 FFCE     		dec	esi	# ivtmp_116
 357 0205 0F85C5FE 		jne	.L6	#,
 357      FFFF
 100:branchpred.c  **** 
 101:branchpred.c  **** #elif defined( BESMART2 )
 102:branchpred.c  **** 	    //sum += (data[ii]>PIVOT)*data[ii];
 103:branchpred.c  **** 	    sum += (data[ii]>PIVOT? data[ii] : 0);
 104:branchpred.c  **** #endif
 105:branchpred.c  ****           }
 106:branchpred.c  ****       }
 107:branchpred.c  **** 
 108:branchpred.c  ****   tstop = TCPU_TIME;
 358              		.loc 1 108 3
 359              	# branchpred.c:108:   tstop = TCPU_TIME;
 360              		.loc 1 108 11 is_stmt 0
 361 020b 488D75C0 		lea	rsi, [rbp-64]	# tmp290,
 362 020f BF020000 		mov	edi, 2	#,
 362      00
 363 0214 C5F877   		vzeroupper
 364 0217 E8000000 		call	clock_gettime	#
 364      00
 365              	.LVL31:
 366 021c C5D057ED 		vxorps	xmm5, xmm5, xmm5	# tmp278
 367 0220 C4E1D32A 		vcvtsi2sd	xmm0, xmm5, QWORD PTR [rbp-56]	# tmp281, tmp278, ts.tv_nsec
 367      45C8
 368 0226 C4E1D32A 		vcvtsi2sd	xmm5, xmm5, QWORD PTR [rbp-64]	# tmp282, tmp278, ts.tv_sec
 368      6DC0
 369              	# branchpred.c:114:   free(data);
 109:branchpred.c  ****   
 110:branchpred.c  **** #ifdef WOW
 111:branchpred.c  ****   tot_tstop = TCPU_TIME;
 112:branchpred.c  **** #endif
 113:branchpred.c  ****   
 114:branchpred.c  ****   free(data);
 370              		.loc 1 114 3
 371 022c 4C89EF   		mov	rdi, r13	#, data
 372              	# branchpred.c:108:   tstop = TCPU_TIME;
 108:branchpred.c  ****   
 373              		.loc 1 108 9
 374 022f C4E2D199 		vfmadd132sd	xmm0, xmm5, QWORD PTR .LC0[rip]	# tstop, tmp259,
 374      05000000 
 374      00
 375 0238 C5FB1145 		vmovsd	QWORD PTR [rbp-72], xmm0	# %sfp, tstop
 375      B8
 376              	.LVL32:
 377              		.loc 1 114 3 is_stmt 1
 378 023d E8000000 		call	free	#
 378      00
 379              	.LVL33:
 115:branchpred.c  **** 
 116:branchpred.c  ****  #if !defined(WOW)
 117:branchpred.c  ****   printf("\nsum is %llu, elapsed seconds: %g\n", sum, tstop - tstart);
 380              		.loc 1 117 3
 381 0242 C5FB1045 		vmovsd	xmm0, QWORD PTR [rbp-72]	# tstop, %sfp
 381      B8
 382 0247 C4C1F96E 		vmovq	xmm6, r14	# tstart, tstart
 382      F6
 383 024c C5FB5CC6 		vsubsd	xmm0, xmm0, xmm6	# tmp261, tstop, tstart
 384 0250 4C89E6   		mov	rsi, r12	#, stmp_sum_55.23
 385 0253 BF000000 		mov	edi, OFFSET FLAT:.LC3	#,
 385      00
 386 0258 B8010000 		mov	eax, 1	#,
 386      00
 387 025d E8000000 		call	printf	#
 387      00
 388              	.LVL34:
 118:branchpred.c  **** 
 119:branchpred.c  **** #else
 120:branchpred.c  ****   double tot_time  = tot_tstop - tot_tstart;
 121:branchpred.c  ****   double loop_time = tstop - tstart;
 122:branchpred.c  ****   printf("\nsum is %llu, elapsed seconds: %g, %g in loop and %g in qsort\n",
 123:branchpred.c  **** 	 sum, tot_time, loop_time, tot_time - loop_time);
 124:branchpred.c  **** #endif
 125:branchpred.c  **** 
 126:branchpred.c  ****   printf("\n");
 389              		.loc 1 126 3
 390 0262 BF0A0000 		mov	edi, 10	#,
 390      00
 391 0267 E8000000 		call	putchar	#
 391      00
 392              	.LVL35:
 127:branchpred.c  ****   return 0;
 393              		.loc 1 127 3
 394              	# branchpred.c:128: }
 128:branchpred.c  **** }
 395              		.loc 1 128 1 is_stmt 0
 396 026c 4883C428 		add	rsp, 40	#,
 397 0270 5B       		pop	rbx	#
 398 0271 415A     		pop	r10	#
 399              		.cfi_remember_state
 400              		.cfi_def_cfa 10, 0
 401 0273 415C     		pop	r12	#
 402              	.LVL36:
 403 0275 415D     		pop	r13	#
 404              	.LVL37:
 405 0277 415E     		pop	r14	#
 406              	.LVL38:
 407 0279 31C0     		xor	eax, eax	#
 408 027b 5D       		pop	rbp	#
 409              	.LVL39:
 410 027c 498D62F8 		lea	rsp, [r10-8]	#,
 411              		.cfi_def_cfa 7, 8
 412 0280 C3       		ret	
 413              	.LVL40:
 414              	.L15:
 415              		.cfi_restore_state
 416              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 417              		.loc 1 91 25
 418 0281 31C0     		xor	eax, eax	#
 419              	# branchpred.c:89: 	sum = 0;
  89:branchpred.c  **** 
 420              		.loc 1 89 6
 421 0283 4531E4   		xor	r12d, r12d	# stmp_sum_55.23
 422              	# branchpred.c:91:         for (ii = 0; ii < SIZE; ii++)
  91:branchpred.c  ****           {
 423              		.loc 1 91 17
 424 0286 31D2     		xor	edx, edx	# tmp.27
 425 0288 E9BEFEFF 		jmp	.L7	#
 425      FF
 426              	.LVL41:
 427              	.L2:
  65:branchpred.c  ****   else
 428              		.loc 1 65 5 is_stmt 1
 429              	.LBB13:
 430              	.LBB14:
 431              		.file 2 "/usr/include/stdlib.h"
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
 432              		.loc 2 280 3
 433              	# /usr/include/stdlib.h:280:   return (int) strtol (__nptr, (char **) NULL, 10);
 434              		.loc 2 280 16 is_stmt 0
 435 028d 488B7E08 		mov	rdi, QWORD PTR [rsi+8]	# MEM[(char * *)argv_40(D) + 8B], MEM[(char * *)argv_40(D) + 8B]
 436              	.LVL42:
 437 0291 BA0A0000 		mov	edx, 10	#,
 437      00
 438 0296 31F6     		xor	esi, esi	#
 439              	.LVL43:
 440 0298 E8000000 		call	strtol	#
 440      00
 441              	.LVL44:
 442 029d 4989C4   		mov	r12, rax	# _60, tmp275
 443              	.LBE14:
 444              	.LBE13:
 445              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 446              		.loc 1 70 16
 447 02a0 4863F8   		movsx	rdi, eax	# SIZE, _60
 448 02a3 BE040000 		mov	esi, 4	#,
 448      00
 449              	.LBB16:
 450              	.LBB15:
 451              	# /usr/include/stdlib.h:280:   return (int) strtol (__nptr, (char **) NULL, 10);
 452              		.loc 2 280 10
 453 02a8 89C3     		mov	ebx, eax	# SIZE, _60
 454              	.LVL45:
 455              	.LBE15:
 456              	.LBE16:
  70:branchpred.c  ****   srand((int)(SIZE));
 457              		.loc 1 70 3 is_stmt 1
 458              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 459              		.loc 1 70 16 is_stmt 0
 460 02aa E8000000 		call	calloc	#
 460      00
 461              	.LVL46:
 462              	# branchpred.c:71:   srand((int)(SIZE));
  71:branchpred.c  ****   
 463              		.loc 1 71 3
 464 02af 4489E7   		mov	edi, r12d	#, SIZE
 465              	# branchpred.c:70:   data = (int*)calloc(SIZE, sizeof(int));
  70:branchpred.c  ****   srand((int)(SIZE));
 466              		.loc 1 70 16
 467 02b2 4989C5   		mov	r13, rax	# data, tmp276
 468              	.LVL47:
  71:branchpred.c  ****   
 469              		.loc 1 71 3 is_stmt 1
 470 02b5 E8000000 		call	srand	#
 470      00
 471              	.LVL48:
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 472              		.loc 1 73 3
  73:branchpred.c  ****     data[cc] = rand() % TOP;
 473              		.loc 1 73 19
 474 02ba 4585E4   		test	r12d, r12d	# _60
 475 02bd 0F8F85FD 		jg	.L3	#,
 475      FFFF
 476 02c3 E9A2FDFF 		jmp	.L4	#
 476      FF
 477              		.cfi_endproc
 478              	.LFE32:
 480              		.section	.rodata.cst8,"aM",@progbits,8
 481              		.align 8
 482              	.LC0:
 483 0000 95D626E8 		.long	-400107883
 484 0004 0B2E113E 		.long	1041313291
 485              		.section	.rodata.cst4,"aM",@progbits,4
 486              		.align 4
 487              	.LC4:
 488 0000 FFFFF7FF 		.long	-524289
 489              		.text
 490              	.Letext0:
 491              		.file 3 "/opt/cluster/spack/opt/spack/linux-centos7-haswell/gcc-4.8.5/gcc-11.2.0-35ynawgsywf7z6zq4
 492              		.file 4 "/usr/include/bits/types.h"
 493              		.file 5 "/usr/include/time.h"
 494              		.file 6 "/usr/include/stdio.h"
 495              		.file 7 "<built-in>"
