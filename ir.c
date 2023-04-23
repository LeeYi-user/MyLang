#include <string.h>
#include "ir.h"

typedef struct {
  int t, t1, t2, label;
  char *s, *op;
} IR;

IR ir[TMAX];
int irTop = 0;

void irNew(IR p) {
  ir[irTop++] = p;
}

void irEmitTs(int t, char *s) {
  irNew((IR) {.op="t=s", .t=t, .s=s});
}

void irEmitSt(char *s, int t) {
  irNew((IR) {.op="s=t", .t=t, .s=s});
}

void irEmitOp2(int t, int t1, char *op, int t2) {
  irNew((IR) {.op=op, .t=t, .t1=t1, .t2=t2});
}

void irEmitLabel(int label) {
  irNew((IR) {.op="label", .label=label});
}

void irEmitGoto(int label) {
  irNew((IR) {.op="goto", .label=label});
}

void irEmitIfGoto(int t, int label) {
  irNew((IR) {.op="if-goto", .t=t, .label=label});
}

void irEmitIfNotGoto(int t, int label) {
  irNew((IR) {.op="ifnot-goto", .t=t, .label=label});
}

void irPrint(IR *p) {
  if (eq(p->op, "s=t")) printf("%s = t%d", p->s, p->t);
  else if (eq(p->op, "t=s")) printf("t%d = %s", p->t, p->s);
  else if (eq(p->op, "label")) printf("(L%d)", p->label);
  else if (eq(p->op, "goto")) printf("goto L%d", p->label);
  else if (eq(p->op, "if-goto")) printf("if t%d goto L%d", p->t, p->label);
  else if (eq(p->op, "ifnot-goto")) printf("ifnot t%d goto L%d", p->t, p->label);
  else printf("t%d = t%d %s t%d", p->t, p->t1, p->op, p->t2);
  printf("\n");
}

void irFprint(FILE *fp, IR *p) {
  if (eq(p->op, "s=t")) fprintf(fp, "%s = t%d", p->s, p->t);
  else if (eq(p->op, "t=s")) fprintf(fp, "t%d = %s", p->t, p->s);
  else if (eq(p->op, "label")) fprintf(fp, "(L%d)", p->label);
  else if (eq(p->op, "goto")) fprintf(fp, "goto L%d", p->label);
  else if (eq(p->op, "if-goto")) fprintf(fp, "if t%d goto L%d", p->t, p->label);
  else if (eq(p->op, "ifnot-goto")) fprintf(fp, "ifnot t%d goto L%d", p->t, p->label);
  else fprintf(fp, "t%d = t%d %s t%d", p->t, p->t1, p->op, p->t2);
  fprintf(fp, "\n");
}

void irDump() {
  printf("============ irDump ============\n");
  for (int i=0; i<irTop; i++) {
    irPrint(&ir[i]);
  }
}

void irToHack(char *fileName) {
  char subtext[SMAX];
  strncpy(subtext, fileName, strlen(fileName) - 2);
  subtext[strlen(subtext) - 1] = '\0';
  strcat(subtext, ".asm");
  FILE *fp = fopen(subtext, "w+");

  fprintf(fp, "// init NF (negative flag mask)\n@32767\nD=A\n@NF\nM=D\n");
  for (int i=0; i<irTop; i++) {
    IR *p = &ir[i];
    fprintf(fp, "// "); irFprint(fp, p);
    if (eq(p->op, "s=t")) fprintf(fp, "@t%d\nD=M\n@%s\nM=D\n", p->t, p->s);
    else if (eq(p->op, "t=s")) {
      char AM = (isdigit(p->s[0])) ? 'A':'M';
      fprintf(fp, "@%s\nD=%c\n@t%d\nM=D\n", p->s, AM, p->t);
    }
    else if (eq(p->op, "label")) fprintf(fp, "(L%d)", p->label);
    else if (eq(p->op, "goto")) fprintf(fp, "@L%d\n0;JMP\n", p->label);
    else if (eq(p->op, "if-goto")) fprintf(fp, "@t%d\nD=M\n@L%d\nD;JNE\n", p->t, p->label);
    else if (eq(p->op, "ifnot-goto")) fprintf(fp, "@t%d\nD=M\n@L%d\nD;JEQ\n", p->t, p->label);
    else {
      char opAsm[100], *op=p->op;
      if (strcmp(op, "<")==0) sprintf(opAsm, "D=D-M\n@NF\nD=D&M\n");             // <
      else if (strcmp(op, "<=")==0) sprintf(opAsm, "D=M-D\n@NF\nD=D&M\nD=!D"); // <=   // > 的相反
      else if (strcmp(op, "==")==0) sprintf(opAsm, "D=D-M\nD=!D\n");                  // ==   // != 的相反
      else if (strcmp(op, "!=")==0) sprintf(opAsm, "D=D-M\n");                        // !=
      else if (strcmp(op, ">")==0) sprintf(opAsm, "D=M-D\n@NF\nD=D&M\n");      // >    // 注意：是 D=M-D
      else if (strcmp(op, ">=")==0) sprintf(opAsm, "D=D-M\n@NF\nD=D&M\nD=!D\n"); // >=   // < 的相反
      else sprintf(opAsm, "D=D%sM", op);
      fprintf(fp, "@t%d\nD=M\n@t%d\n%s\n@t%d\nM=D\n", p->t1, p->t2, opAsm, p->t);
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
}
