	.section __TEXT,__text,regular,pure_instructions
	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
.lcomm _save_vm.0,4,2
.section __TEXT,__text,regular,pure_instructions
	.align 2
	.align 2
.section __TEXT,__text,regular,pure_instructions
	.align 2
_interpret:
	mflr r0
	mfcr r2
	bl saveFP ; save f14-f31
"L00000000001$pb":
	stmw r13,-220(r1)
	stw r2,4(r1)
	stwu r1,-288(r1)
	mflr r31
	stw r3,312(r1)
	stw r4,316(r1)
	li r0,2048
	stw r0,772(r3)
	li r0,64
	lwz r2,312(r1)
	stw r0,776(r2)
	li r0,0
	stw r0,768(r2)
	bl L_setjmp$stub
	cmpwi cr7,r3,0
	bne+ cr7,L2
	addis r2,r31,ha16(_save_vm.0-"L00000000001$pb")
	lwz r9,312(r1)
	stw r9,lo16(_save_vm.0-"L00000000001$pb")(r2)
	li r3,0
	lwz r11,316(r1)
	addi r4,r11,4
	mr r5,r9
	lwz r12,0(r11)
	mtctr r12
	bctrl
	b L3
L2:
	addis r2,r31,ha16(_save_vm.0-"L00000000001$pb")
	lwz r2,lo16(_save_vm.0-"L00000000001$pb")(r2)
	stw r2,312(r1)
	lwz r9,772(r2)
	slwi r2,r9,2
	lwz r11,312(r1)
	add r2,r2,r11
	lwz r3,780(r2)
	addi r9,r9,1
	stw r9,772(r11)
L3:
	lwz r2,312(r1)
	lwz r0,768(r2)
	cmpwi cr7,r0,0
	beq+ cr7,L1
L5:
	lwz r9,312(r1)
	lwz r2,768(r9)
	lwz r9,0(r2)
	addi r2,r2,4
	lwz r11,312(r1)
	stw r2,768(r11)
	addi r4,r9,4
	lwz r5,312(r1)
	lwz r12,0(r9)
	mtctr r12
	bctrl
	b L5
L1:
	addi r1,r1,288
	lwz r11,4(r1)
	lmw r13,-220(r1)
	mtcrf 56,r11
	b restFP ; restore f14-f31
	.align 2
.section __TEXT,__text,regular,pure_instructions
	.align 2
_x_colon:
	mflr r0
	stmw r28,-16(r1)
	stw r0,8(r1)
	stwu r1,-80(r1)
	mr r28,r3
	mr r29,r4
	mr r30,r5
	lwz r0,776(r5)
	cmpwi cr7,r0,0
	bne+ cr7,L9
	mr r3,r5
	bl L_overflow$stub
L9:
	lwz r2,776(r30)
	addi r2,r2,-1
	stw r2,776(r30)
	slwi r2,r2,2
	add r2,r2,r30
	lwz r0,768(r30)
	stw r0,8972(r2)
	stw r29,768(r30)
	mr r3,r28
	lwz r0,88(r1)
	addi r1,r1,80
	mtlr r0
	lmw r28,-16(r1)
	blr
	.align 2
.section __TEXT,__text,regular,pure_instructions
	.align 2
_execute:
	mflr r0
	stmw r29,-12(r1)
	stw r0,8(r1)
	stwu r1,-80(r1)
	mr r30,r5
	mr r29,r3
	lwz r2,772(r5)
	addi r2,r2,1
	cmplwi cr7,r2,2047
	ble+ cr7,L12
	mr r3,r5
	bl L_underflow$stub
L12:
	lwz r2,772(r30)
	slwi r9,r2,2
	add r9,r9,r30
	addi r2,r2,1
	stw r2,772(r30)
	lwz r3,780(r9)
	addi r4,r29,4
	mr r5,r30
	lwz r12,0(r29)
	mtctr r12
	bctrl
	lwz r0,88(r1)
	addi r1,r1,80
	mtlr r0
	lmw r29,-12(r1)
	blr
.data
.cstring
	.align 2
LC0:
	.ascii "overflow\12\0"
.section __TEXT,__text,regular,pure_instructions
	.align 2
	.align 2
	.globl _overflow
.section __TEXT,__text,regular,pure_instructions
	.align 2
_overflow:
	mflr r0
	stw r31,-4(r1)
	stw r0,8(r1)
	stwu r1,-80(r1)
	bcl 20,31,"L00000000002$pb"
"L00000000002$pb":
	mflr r31
	addis r3,r31,ha16(LC0-"L00000000002$pb")
	la r3,lo16(LC0-"L00000000002$pb")(r3)
	li r4,1
	li r5,9
	addis r6,r31,ha16(L___sF$non_lazy_ptr-"L00000000002$pb")
	lwz r6,lo16(L___sF$non_lazy_ptr-"L00000000002$pb")(r6)
	addi r6,r6,176
	bl L_fwrite$stub
	bl L_abort$stub
.data
.cstring
	.align 2
LC1:
	.ascii "underflow\12\0"
.section __TEXT,__text,regular,pure_instructions
	.align 2
	.align 2
	.globl _underflow
.section __TEXT,__text,regular,pure_instructions
	.align 2
_underflow:
	mflr r0
	stw r31,-4(r1)
	stw r0,8(r1)
	stwu r1,-80(r1)
	bcl 20,31,"L00000000003$pb"
"L00000000003$pb":
	mflr r31
	addis r3,r31,ha16(LC1-"L00000000003$pb")
	la r3,lo16(LC1-"L00000000003$pb")(r3)
	li r4,1
	li r5,10
	addis r6,r31,ha16(L___sF$non_lazy_ptr-"L00000000003$pb")
	lwz r6,lo16(L___sF$non_lazy_ptr-"L00000000003$pb")(r6)
	addi r6,r6,176
	bl L_fwrite$stub
	bl L_abort$stub
.data
.cstring
	.align 2
LC2:
	.ascii "QUIT\0"
.section __TEXT,__text,regular,pure_instructions
	.align 2
	.align 2
	.globl _main
.section __TEXT,__text,regular,pure_instructions
	.align 2
_main:
	mflr r0
	stw r31,-4(r1)
	stw r0,8(r1)
	stwu r1,-80(r1)
	bcl 20,31,"L00000000004$pb"
"L00000000004$pb":
	mflr r31
	addis r3,r31,ha16(LC2-"L00000000004$pb")
	la r3,lo16(LC2-"L00000000004$pb")(r3)
	bl L_lookup$stub
	mr r4,r3
	addis r3,r31,ha16(_vmstate-"L00000000004$pb")
	la r3,lo16(_vmstate-"L00000000004$pb")(r3)
	bl _interpret
	li r3,0
	lwz r0,88(r1)
	addi r1,r1,80
	mtlr r0
	lwz r31,-4(r1)
	blr
.lcomm _vmstate,9228,2
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_lookup$stub:
	.indirect_symbol _lookup
	mflr r0
	bcl 20,31,L0$_lookup
L0$_lookup:
	mflr r11
	addis r11,r11,ha16(L_lookup$lazy_ptr-L0$_lookup)
	mtlr r0
	lwzu r12,lo16(L_lookup$lazy_ptr-L0$_lookup)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_lookup$lazy_ptr:
	.indirect_symbol _lookup
	.long dyld_stub_binding_helper
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_abort$stub:
	.indirect_symbol _abort
	mflr r0
	bcl 20,31,L0$_abort
L0$_abort:
	mflr r11
	addis r11,r11,ha16(L_abort$lazy_ptr-L0$_abort)
	mtlr r0
	lwzu r12,lo16(L_abort$lazy_ptr-L0$_abort)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_abort$lazy_ptr:
	.indirect_symbol _abort
	.long dyld_stub_binding_helper
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_fwrite$stub:
	.indirect_symbol _fwrite
	mflr r0
	bcl 20,31,L0$_fwrite
L0$_fwrite:
	mflr r11
	addis r11,r11,ha16(L_fwrite$lazy_ptr-L0$_fwrite)
	mtlr r0
	lwzu r12,lo16(L_fwrite$lazy_ptr-L0$_fwrite)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_fwrite$lazy_ptr:
	.indirect_symbol _fwrite
	.long dyld_stub_binding_helper
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_underflow$stub:
	.indirect_symbol _underflow
	mflr r0
	bcl 20,31,L0$_underflow
L0$_underflow:
	mflr r11
	addis r11,r11,ha16(L_underflow$lazy_ptr-L0$_underflow)
	mtlr r0
	lwzu r12,lo16(L_underflow$lazy_ptr-L0$_underflow)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_underflow$lazy_ptr:
	.indirect_symbol _underflow
	.long dyld_stub_binding_helper
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_overflow$stub:
	.indirect_symbol _overflow
	mflr r0
	bcl 20,31,L0$_overflow
L0$_overflow:
	mflr r11
	addis r11,r11,ha16(L_overflow$lazy_ptr-L0$_overflow)
	mtlr r0
	lwzu r12,lo16(L_overflow$lazy_ptr-L0$_overflow)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_overflow$lazy_ptr:
	.indirect_symbol _overflow
	.long dyld_stub_binding_helper
.data
.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
	.align 2
L_setjmp$stub:
	.indirect_symbol _setjmp
	mflr r0
	bcl 20,31,L0$_setjmp
L0$_setjmp:
	mflr r11
	addis r11,r11,ha16(L_setjmp$lazy_ptr-L0$_setjmp)
	mtlr r0
	lwzu r12,lo16(L_setjmp$lazy_ptr-L0$_setjmp)(r11)
	mtctr r12
	bctr
.data
.lazy_symbol_pointer
L_setjmp$lazy_ptr:
	.indirect_symbol _setjmp
	.long dyld_stub_binding_helper
.data
.non_lazy_symbol_pointer
L___sF$non_lazy_ptr:
	.indirect_symbol ___sF
	.long	0
