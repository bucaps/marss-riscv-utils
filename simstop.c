/* simstop.c- Switches MARSS-RISCV to emulation mode */

#define MARSS_STOP_SIM() asm("csrs	0x801,zero")

int
main(int argc, const char** argv)
{
  MARSS_STOP_SIM();
  return 0;
}