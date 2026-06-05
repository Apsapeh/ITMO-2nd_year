#!/bin/bash
for filename in $PWD/m68k-*.yaml; do
    printf "\n\n%s\n" $filename
    wrench m68k.s --isa m68k -c $filename
done
