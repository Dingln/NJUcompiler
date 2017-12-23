#include"stdlib.h"
#include"stdio.h"
#include"string.h"
#include"IR.h"
#include"stdarg.h"

int varCount = 1;
int tempCount = 1;
int labelCount = 1;

InterCode IRhead = NULL;
InterCode IRtail = NULL;

void insertIR(InterCode c) 
{
    if(IRhead == NULL) {
        IRhead = c;
        IRhead->pre = IRhead;
        IRhead->next = IRhead;
        IRtail = IRhead;
    }
    else {
        c->next = IRhead;
        c->pre = IRtail;
        IRtail->next = c;
        IRtail = c;
        IRhead->pre = c;
    }
}

void deleteIR(InterCode c)
{
    if(c == IRhead && c==IRtail) {
        IRhead = IRtail = NULL;
        free(c);
    }
    else if(c == IRhead) {
        IRhead = IRhead->next;
        IRhead->pre = IRtail;
        IRtail->next = IRhead;
        free(c);
    }
    else if(c == IRtail) {
        IRtail = IRtail->pre;
        IRtail->next = IRhead;
        IRhead->pre = IRtail;
        free(c);
    }
    else {
        assert(c->pre != NULL && c->next != NULL);
        c->pre->next = c->next;
        c->next->pre = c->pre;
        free(c);
    }
}

void printIR(char *filename)
{
    FILE *fp=fopen(filename, "w");
    if(fp == NULL) {
        printf("Open file %s error!\n", filename);
        return ;
    }
    else {
        InterCode p = IRhead;
        while(p->next != IRhead) {
            switch(p->kind) {
                case ASSIGN: 
                    printOperand(p->u.assign.left, fp);
                    fprintf(fp, " := ");
                    printOperand(p->u.assign.right, fp);
                    break;
                case ADD:
                    printOperand(p->u.binop.result, fp);
                    fprintf(fp, " := ");
                    printOperand(p->u.binop.op1, fp);
                    fprintf(fp, " + ");
                    printOperand(p->u.binop.op2, fp);
                    break;
                case SUB:
                    printOperand(p->u.binop.result, fp);
                    fprintf(fp, " := ");
                    printOperand(p->u.binop.op1, fp);
                    fprintf(fp, " - ");
                    printOperand(p->u.binop.op2, fp);
                    break;
                case MUL:
                    printOperand(p->u.binop.result, fp);
                    fprintf(fp, " := ");
                    printOperand(p->u.binop.op1, fp);
                    fprintf(fp, " * ");
                    printOperand(p->u.binop.op2, fp);
                    break;
                case DIVIDE:
                    printOperand(p->u.binop.result, fp);
                    fprintf(fp, " := ");
                    printOperand(p->u.binop.op1, fp);
                    fprintf(fp, " / ");
                    printOperand(p->u.binop.op2, fp);
                    break;
                case LABELOP:
                    fprintf(fp, "LABEL ");
                    printOperand(p->u.sigop.op, fp);
                    fprintf(fp, " : ");
                    break;
                case GOTO:
                    fprintf(fp, "GOTO ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case RETURNOP:
                    fprintf(fp, "RETURN ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case READ:
                    fprintf(fp, "READ ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case WRITE:
                    fprintf(fp, "WRITE ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case ARG:
                    fprintf(fp, "ARG ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case PARAM:
                    fprintf(fp, "PARAM ");
                    printOperand(p->u.sigop.op, fp);
                    break;
                case IFOP:
                    fprintf(fp, "IF ");
                    printOperand(p->u.ifop.op1, fp);
                    fprintf(fp, " %s ", p->u.ifop.relop);
                    printOperand(p->u.ifop.op2, fp);
                    fprintf(fp, " GOTO ");
                    printOperand(p->u.ifop.label, fp);
                    break;
                case CALL:
                    printOperand(p->u.callop.result, fp);
                    fprintf(fp, " := CALL ");
                    fprintf(fp, "%s", p->u.callop.name);
                    break;
                case FUNCTION:
                    fprintf(fp, "FUNCTION %s :", p->u.funop.name);
                    break;
                case DEC:
                    fprintf(fp, "DEC ");
                    printOperand(p->u.decop.op,fp);
                    printOperand(p->u.decop.size,fp);
                    break;
                default:
                    fprintf(fp, "Unknown IR type!\n");
            }
            fprintf(fp, "\n");
            p = p->next;
        }
    }
    fclose(fp);
}

void printOperand(Operand op, FILE *fp)
{
    if(op == NULL)
        return;
    switch(op->kind) {
        case CONSTANT: fprintf(fp, "#%d", op->u.value); break;
        case VARIABLE: fprintf(fp, "v%d", op->u.var_no); break;
        case ADDRESS: fprintf(fp, "&%d", op->u.var_no); break;
        case TEMP: fprintf(fp, "t%d", op->u.var_no); break;
        case LABEL: fprintf(fp, "label%d", op->u.var_no); break;
        case VPOINTER: fprintf(fp, "*v%d", op->u.var_no); break;
        case TPOINTER: fprintf(fp, "*t%d", op->u.var_no); break;
        default: fprintf(fp, "Unknown operand!\n");
    }

}

Operand createOperand(OperandKind kind, int val)
{
    Operand newOp = (Operand)malloc(sizeof(Operand));
    newOp->kind = kind;
    switch(newOp->kind) {
        case CONSTANT: newOp->u.value = val; break;
        case ADDRESS:
        case VPOINTER:
        case TPOINTER: newOp->u.var_no = val; break;
        case TEMP: 
            newOp->u.var_no = val;
            tempCount++;
            break;
        case LABEL:
            newOp->u.var_no = val;
            labelCount++;
            break;
        case VARIABLE:
            newOp->u.var_no = val;
            varCount++;
            break;
        default: printf("Unknown operand!\n");
    }
    return newOp;
}

Operand createTemp()
{   
    Operand newtemp = (Operand)malloc(sizeof(Operand));
    newtemp->kind = TEMP;
    newtemp->u.var_no = tempCount;
    tempCount++;
    return newtemp;
}

Operand createLable()
{
    Operand newlable = (Operand)malloc(sizeof(Operand));
    newlable->kind = LABEL;
    newlable->u.var_no = labelCount;
    labelCount++;
    return newlable;
}

InterCode IRcodeConcat(int num, ...)
{
    InterCode IRtemphead = NULL, IRtemptail = NULL;
    va_list myarg;
    va_start(myarg, num);
    for(int i = 0; i < num; i++) {
        InterCode code = va_arg(myarg, InterCode);
        if(IRtemphead == NULL) {
            IRtemphead = code;
            IRtemptail = IRtemphead;
            while(IRtemptail->next != NULL)
                IRtemptail = IRtemptail->next;
        }
        else {
            IRtemptail->next = code;
            code->pre = IRtemptail;
            while(IRtemptail->next != NULL)
                IRtemptail = IRtemptail->next;
        }
    }
    va_end(myarg);
    return IRtemphead;
}

InterCode createAssign(IRKind kind, Operand left, Operand right) 
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.assign.left = left;
    ircode->u.assign.right = right;
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createBinop(IRKind kind, Operand result, Operand op1, Operand op2)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.binop.op1 = op1;
    ircode->u.binop.op2 = op2;
    ircode->u.binop.result = result;
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createSigop(IRKind kind, Operand op)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.sigop.op = op;
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createIfop(IRKind kind, Operand op1, Operand op2, Operand label, char *relop)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.ifop.label = label;
    ircode->u.ifop.op1 = op1;
    ircode->u.ifop.op2 = op2;
    strcpy(ircode->u.ifop.relop, relop);
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createFunop(IRKind kind, char *name)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    strcpy(ircode->u.funop.name, name);
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createCallop(IRKind kind, Operand result, char *name)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.callop.result = result;
    strcpy(ircode->u.callop.name, name);
    ircode->pre = ircode->next = NULL;
    return ircode;
}

InterCode createDecop(IRKind kind, Operand op, int size)
{
    InterCode ircode = (InterCode)malloc(sizeof(InterCode));
    ircode->kind = kind;
    ircode->u.decop.op = op;
    ircode->u.decop.size = size;
    ircode->pre = ircode->next = NULL;
    return ircode;
}