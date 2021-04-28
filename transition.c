case '$from' << 8 | '$read':
    tape_set(&tape, index, '$write');
    state = '$goto';
    $move
    break;