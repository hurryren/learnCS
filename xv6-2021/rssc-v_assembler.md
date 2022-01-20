# 1 指令

## 1.1 auipc

**AUIPC** (add upper immediate to *pc* register) is used to build pc-relative addresses and uses the U-type format. **AUIPC** forms a 32-bit offset from the U-immediate, fillling in the lowest 12 bits with zeros,  adds this offset to the address of the **AUIPC** instruction, then palces the result in register *rd*.

例如：

```assembly
auipc  ra,0x1
```

该指令执行时  pc=0x8000032e，则执行过后，ra的值等于 $pc+(0x1<<12)$



## 1.2 sext

符号扩展，将立即数扩展到32位，如果时有符号数则进行符号扩展，无符号数则进行无符号扩展。



## 1.3 beqz

**C.BEQZ** performs conditional control transfers. The offset is sign-extended added to the pc to form the branch target address. It can therefore target a $\pm256$ B range. **C.BEQZ** takes the branch if the value in register rs1 is zero. it expands to beq rs1, x0, offset.

**C.BEQZ** is defined analogously, but it takes the branch if rs1 contains a nonzero value. It expands to bne rs1, x0, offset.

等于0就跳，跳的位置是前后256。

## 1.4 jalr

The target address is obtained by adding the sign-extended 12-bit I-immediate to the register rs1, then setting the least-signeficant bit of the result to zero. The address of the instruction following the jump ( pc+4 )  is written to register rd. Register x0 can be used as the destination if the result is not required.

The **JAL** and **JALR** instructions will generate an instruction-address-misaligned exception if the target address is not aligned to a four-byte boundary.

例如：

```assembly
jalr 1702(ra)
```

跳转到 ra+1702的位置，并且将此时的pc值存到ra中。



## 1.5 LUI

**LUI** (load upper immediate) is used to build 32-bit constants and uses the U-type format. LUI places the 32-bit U-immediate value into the destination register rd, filling in the lowest 12 bits with zero.



将立即数存放到寄存中的高 20 bit 中，

例如：

```assembly
lui a5,0x10000
```

执行之前a5=0; 执行之后 a5=10000000



## 1.6  CSR

All CSR instructions atomically read-modify-write a single CSR.

The **CSRRW**  (Atomic Read/Write CSR) instruction atomically swaps values in the CSRs and integer registers. **CSRRW** reads the old value of the CSR, zero-extends the value to *XLEN* bits, then writes it to integer *rd*. The initial value in *rs1* is written to the CSR.