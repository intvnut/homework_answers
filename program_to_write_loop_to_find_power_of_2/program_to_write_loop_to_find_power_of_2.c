#include <stdio.h>

int main() {
  puts("#include <stdio.h>\n");
  puts("int main() {");
  puts("  int highest = 0;");
  puts("  char buf[256];");
  puts("  for (int i = 0; i < 100000; i++) {");
  puts("    while (1) {");
  puts("      printf(\"Is %d a power of 2? \", i);");
  puts("      fflush(stdout);");
  puts("      fgets(buf, sizeof(buf), stdin);");
  puts("      if (strncmp(buf, \"y\", 1) == 0) {");
  puts("        highest = i;");
  puts("        break;");
  puts("      } else if (strncmp(buf, \"n\", 1) == 0) {");
  puts("        break;");
  puts("      }");
  puts("      puts(\"Enter y or n.\");");
  puts("    }");
  puts("  }");
  puts("  printf(\"The highest power of 2 found: %d\\n\",");
  puts("         highest);");
  puts("  return 0;");
  puts("}");
}
