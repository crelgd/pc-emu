; read_sector(int i/o, int sector, int save_address, int status)
!read_sector
    pop r3 ; address

    pop r1 ; i/o 1-read 2-write
    out 0x16, r1 

    pop r1 ; sector
    out 0x17, r1 

    pop r1 ; save_address
    out 0x18, r1

    pop r1 ; status
    out 0x19, r1 

    push r3 ; return to code
    ret 
