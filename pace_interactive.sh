#!/bin/sh
salloc -Jfunnel_interactive --partition=ice-gpu,coc-gpu -N1 --ntasks-per-node=2 -C"intel&core40" --mem-per-cpu=16G --time=00:20:00
exit 0
