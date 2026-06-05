savedcmd_mydevice.mod := printf '%s\n'   mydevice.o | awk '!x[$$0]++ { print("./"$$0) }' > mydevice.mod
