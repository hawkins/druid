struct ExprNode {
  char* c;
  struct ExprNode operand_lhs;
  struct ExprNode operand_rhs;

  ExprNode(char* token) {
    c = token;
    operand_lhs = operand_rhs = NULL;
  }
}

int main(int argc, char** argv) {
  ExprNode a("Test");

  printf("%s", a.c);
  return 0;
}
