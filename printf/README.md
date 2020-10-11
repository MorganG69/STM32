# printf() for STM32

Disable syscalls.c in the compiler
Put the retarget.c/h files in the inc/src folders
Make sure the default uart is enabled as it goes through the ST Link.
Enable with: RetargetInit(&huart2);

You can now use printf as usual and it outputs to a serial terminal.