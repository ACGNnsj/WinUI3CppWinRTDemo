def add(x,y,base,outbase):
    a = int(x,base) + int(y,base)
    if (outbase == 8):
        return str(oct(a))
    elif (outbase == 10):
        return str(a)
    elif (outbase == 16):
        return str(hex(a))
    else:
        return None