savedcmd_hello_lkm.mod := printf '%s\n'   hello_lkm.o | awk '!x[$$0]++ { print("./"$$0) }' > hello_lkm.mod
