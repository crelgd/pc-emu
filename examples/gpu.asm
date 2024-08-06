; draw_pixel(int x, int y, int color, int status);
!draw_pixel
    pop r3 ; saved address

    pop r1 ; x
    out 0x80, r1 

    pop r1 ; y
    out 0x81, r1 

    pop r1 ; color
    out 0x82, r1 

    pop r1 ; status 1/0
    out 0x83, r1 

    push r3 ; return to code
    ret 
