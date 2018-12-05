#define NEW_STACK_SIZE  // choose your stack size here.
char *old_stack, new_stack[NEW_STACK_SIZE];  // Oh, your stack is on bss.

int main() {
  __asm__ __volatile__ (
      "mov %%rsp, %0\n"
      "mov %1, %%rsp\n"
      : "=&r"(old_stack)
      : "r"(new_stack + NEW_STACK_SIZE)
  );
  // do whatever you like between previous and following lines.
  __asm__ __volatile__ (
      "mov %0, %%rsp\n"
      :
      : "r"(old_stack)
  );
}
