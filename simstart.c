/* simstart.c- Switches MARSS-RISCV to simulation mode */

#define MARSS_START_SIM() asm("csrs	0x800,zero")

int
main(int argc, const char** argv)
{
  MARSS_START_SIM();
  return 0;
}