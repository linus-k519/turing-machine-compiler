case '$from' << 8 | '$read':
tape_set(&tape, index, '$write');
state = '$goto';
if ('$move' == 'r') {
index++;
} else if ('$move' == 'l') {
index--;
}
break;