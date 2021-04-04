case '$from' << 8 | '$read':
    tape[index] = '$write';
    state = '$goto';
    $move
    break;