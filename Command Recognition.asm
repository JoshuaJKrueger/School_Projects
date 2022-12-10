.ORIG x3000
; --------------- Storage that top part of program uses ------------
PROMPT        .STRINGZ "Enter instruction: "    ; Prompt to the user
OADD  .STRINGZ "0001\n"
OAND  .STRINGZ "0101\n"
OBR   .STRINGZ "0000\n"
CHAR_SET      .STRINGZ "ABDEIJLMNOPRST\n\r"     ; Possible characters
CHAR_SET_CON  .STRINGZ "QU"
BRnzp #1 ; Skip next line
STRT .FILL x303F ; Should point to S__
LEA R1, CHAR_SET ; Get address of the start of the charset
LEA R6, CHAR_SET_CON
LD R5, STRT      ; Register to restart program

S__ LEA R0, PROMPT
    PUTS
    JSR GET_C
    LDR R2, R1, #0  ;- A
    JSR CHECK
    BRz S_A

    LDR R2, R1, #1  ;- B
    JSR CHECK
    BRz S_B

    LDR R2, R1, #5  ;- J
    JSR CHECK
    BRz S_J

    LDR R2, R1, #6  ;- L
    JSR CHECK
    BRz S_L

    LDR R2, R1, #8  ;- N
    JSR CHECK
    BRz S_N

    LDR R2, R1, #11 ;- R
    JSR CHECK
    BRz S_R

    LDR R2, R1, #12 ;- S
    JSR CHECK
    BRz S_S

    LDR R2, R1, #13 ;- T
    JSR CHECK
    BRz S_T

    LDR R2, R6, #0  ;- Q
    JSR CHECK
    BRz S_Q

    BRnzp S_OVERFLOW

S_A JSR GET_C
    LDR R2, R1, #2  ;- D
    JSR CHECK
    BRz S_AD

    LDR R2, R1, #8  ;- N
    JSR CHECK
    BRz S_AN

    BRnzp S_OVERFLOW

    S_AD JSR GET_C
        LDR R2, R1, #2  ;- D
        JSR CHECK
        BRz S_ADD

        BRnzp S_OVERFLOW

        S_ADD JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OADD ; Print opcode for ADD
            PUTS         ; ↑
            JMP R5       ; Restart

            BRnzp S_OVERFLOW

    S_AN JSR GET_C
        LDR R2, R1, #2  ;- D
        JSR CHECK
        BRz S_AND

        BRnzp S_OVERFLOW

        S_AND JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OAND ; Print opcode for AND
            PUTS         ; ↑
            JMP R5       ; Restart

            BRnzp S_OVERFLOW

S_B JSR GET_C
    LDR R2, R1, #11 ;- R
    JSR CHECK
    BRz S_BR

    BRnzp S_OVERFLOW

    S_BR JSR GET_C
        JSR CHECKNL
        BRnp #3
        LEA R0, OBR ; Print opcode for BR
        PUTS        ; ↑
        JMP R5      ; Restart

        BRnzp S_OVERFLOW

S_J JSR GET_C
    LDR R2, R1, #7  ;- M
    JSR CHECK
    BRz S_JM

    LDR R2, R1, #12 ;- S
    JSR CHECK
    BRz S_JS

    BRnzp S_OVERFLOW

    S_JM JSR GET_C
        LDR R2, R1, #10 ;- P
        JSR CHECK
        BRz S_JMP

        BRnzp S_OVERFLOW

        S_JMP JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OJMP ; Print opcode for JMP
            PUTS         ; ↑
            JMP R5       ; Restart
    
            BRnzp S_OVERFLOW

    S_JS JSR GET_C
        LDR R2, R1, #11 ;- R
        JSR CHECK
        BRz S_JSR

        BRnzp S_OVERFLOW

        S_JSR JSR GET_C
            LDR R2, R1, #11 ;- R
            JSR CHECK
            BRz S_JSRR

            JSR CHECKNL
            BRnp #1 ; Goto S_OVERFLOW
            BRz  #4 ; Print opcode for JSR

            BRnzp S_OVERFLOW
        
        S_JSRR JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OJSR ; Print opcode for JSRR
            PUTS         ; ↑
            JMP R5       ; Restart

            BRnzp S_OVERFLOW

S_L JSR GET_C
    LDR R2, R1, #2  ;- D
    JSR CHECK
    BRz S_LD

    LDR R2, R1, #3  ;- E
    JSR CHECK
    BRz S_LE

    BRnzp S_OVERFLOW

    S_LD JSR GET_C
        LDR R2, R1, #4  ;- I
        JSR CHECK
        BRz S_LDI

        LDR R2, R1, #11 ;- R
        JSR CHECK
        BRz S_LDR

        JSR CHECKNL
        BRnp #3
        LEA R0, OLD ; Print opcode for LDI
        PUTS        ; ↑
        JMP R5      ; Restart

        BRnzp S_OVERFLOW
        
        S_LDI JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OLDI ; Print opcode for LDI
            PUTS         ; ↑
            JMP R5       ; Restart
        
            BRnzp S_OVERFLOW

        S_LDR JSR GET_C
            LDR R2, R1, #11 ;- R
            JSR CHECKNL
            BRnp #3
            LEA R0, OLDR ; Print opcode for LDR
            PUTS         ; ↑
            JMP R5       ; Restart
    
            BRnzp S_OVERFLOW

    S_LE JSR GET_C
        LDR R2, R1, #0 ;- A
        JSR CHECK
        BRz S_LEA

        BRnzp S_OVERFLOW
        
        S_LEA JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OLEA ; Print opcode for LEA
            PUTS         ; ↑
            JMP R5       ; Restart
    
            BRnzp S_OVERFLOW

S_N JSR GET_C
    LDR R2, R1, #9  ;- O
    JSR CHECK
    BRz S_NO

    BRnzp S_OVERFLOW

    S_NO JSR GET_C
        LDR R2, R1, #13 ;- T
        JSR CHECK
        BRz S_NOT

        BRnzp S_OVERFLOW
        
        S_NOT JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, ONOT ; Print opcode for NOT
            PUTS         ; ↑
            JMP R5       ; Restart

            BRnzp S_OVERFLOW

S_R JSR GET_C
    LDR R2, R1, #3  ;- E
    JSR CHECK
    BRz S_RE
    
    LDR R2, R1, #13 ;- T
    JSR CHECK
    BRz S_RT

    BRnzp S_OVERFLOW

    S_RE JSR GET_C
        LDR R2, R1, #13 ;- T
        JSR CHECK
        BRz S_RET

        BRnzp S_OVERFLOW
        
        S_RET JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, ORET ; Print opcode for RET
            PUTS         ; ↑
            JMP R5       ; Restart
    
            BRnzp S_OVERFLOW

    S_RT JSR GET_C
        LDR R2, R1, #4 ;- I
        JSR CHECK
        BRz S_RTI

        BRnzp S_OVERFLOW
        
        S_RTI JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, ORTI ; Print opcode for RTI
            PUTS         ; ↑
            JMP R5       ; Restart
    
            BRnzp S_OVERFLOW

S_S JSR GET_C
    LDR R2, R1, #13 ;- T
    JSR CHECK
    BRz S_ST

    BRnzp S_OVERFLOW

    S_ST JSR GET_C
        LDR R2, R1, #4  ;- I
        JSR CHECK
        BRz S_STI

        LDR R2, R1, #11 ;- R
        JSR CHECK
        BRz S_STR

        JSR CHECKNL
        BRnp #3
        LEA R0, OST ; Print opcode for ST
        PUTS        ; ↑
        JMP R5      ; Restart

        BRnzp S_OVERFLOW
        
        S_STI JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OSTI ; Print opcode for STI
            PUTS         ; ↑
            JMP R5       ; Restart

        BRnzp S_OVERFLOW
        
        S_STR JSR GET_C
            JSR CHECKNL
            BRnp #3
            LEA R0, OSTR ; Print opcode for STR
            PUTS         ; ↑
            JMP R5       ; Restart
    
        BRnzp S_OVERFLOW

S_T JSR GET_C
    LDR R2, R1, #11 ;- R
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    LDR R2, R1, #0  ;- A
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    LDR R2, R1, #10 ;- P
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    JSR CHECKNL
    BRnp #3
    LEA R0, OTRAP ; Print opcode for TRAP
    PUTS          ; ↑
    JMP R5        ; Restart

    BRnzp S_OVERFLOW

S_Q JSR GET_C
    LDR R2, R6, #1  ;- U
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    LDR R2, R1, #4  ;- I
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    LDR R2, R1, #13 ;- T
    JSR CHECK
    BRnp S_OVERFLOW

    JSR GET_C
    JSR CHECKNL
    BRz END

    BRnzp S_OVERFLOW

S_OVERFLOW JSR GET_C
    LDR R2, R1, #14 ;- \n
    JSR CHECK
    BRz ONERRINVLD

    LDR R2, R1, #15 ;- \r
    JSR CHECK
    BRz ONERRINVLD

    BRnzp S_OVERFLOW

; ----------- Sub-Processes -----------
CHECK NOT R2, R2     ; Invert mask
    ADD R2, R2, #1   ; ↑
    ADD R2, R0, R2   ; Input - mask (to check if they are equal)
    RET

CHECKNL AND R3, R3, #0
    ADD R3, R7, #0

    LDR R2, R1, #14 ;- \n
    JSR CHECK
    BRnp #1
    BRz #7

    LDR R2, R1, #15 ;- \r
    JSR CHECK
    BRnp #1
    BRz #3

    AND R7, R7, #0
    ADD R7, R3, #0 ; Return \n, \r not pressed
    RET

    AND R7, R7, #0
    ADD R7, R3, #0
    AND R3, R3, #0 ; Set CC to 0 Return \n, \r pressed
    RET

GET_C AND R3, R3, #0
    ADD R3, R7, #0

    GETC
    OUT

    LD R2, NEGZ           ; Checks if input is greater then Z
    LD R4, TOUPR          ; if yes, subtracts 20
    ADD R2, R0, R2
    BRnz #1
    ADD R0, R0, R4

    AND R7, R7, #0
    ADD R7, R3, #0
    RET

ONERRINVLD LEA R0, ERRINVLD ; Print Error and restart
    PUTS
    JMP R5

; ------------ Stored Data ------------
NEGZ          .FILL    xFFA6                    ; Last uppercase value (Z) (negative)
TOUPR         .FILL    xFFE0                    ; -20(dec) diff of upper and lowercase
ERRINVLD      .STRINGZ "ERROR: Invalid input\n"
OJMP  .STRINGZ "1100\n"
OJSR  .STRINGZ "0100\n"
OLD   .STRINGZ "0010\n"
OLDI  .STRINGZ "1010\n"
OLDR  .STRINGZ "0110\n"
OLEA  .STRINGZ "1110\n"
ONOT  .STRINGZ "1001\n"
ORET  .STRINGZ "1100\n"
ORTI  .STRINGZ "1000\n"
OST   .STRINGZ "0011\n"
OSTI  .STRINGZ "1011\n"
OSTR  .STRINGZ "0111\n"
OTRAP .STRINGZ "1111\n"

END
    .END
