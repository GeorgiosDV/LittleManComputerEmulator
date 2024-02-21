        BRZ LOOP
LOOP    LDA ITE
        BRZ END
        SUB ONE
        STA ITE
        INP
        STA IN
        SUB FIR
        BRP TESTTWO
        LDA FIR
        STA SEC
        LDA IN
        STA FIR
        BRA LOOP
TESTTWO LDA IN
        SUB SEC
        BRP LOOP
        LDA IN
        STA SEC
        BRA LOOP
END     LDA SEC
        OUT
        HLT
SEC     DAT 999
FIR     DAT 999
IN      DAT 0
ITE     DAT 10
ONE     DAT 1
