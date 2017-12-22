#ifndef _INTERCODE_H
#define _INTERCODE_H

typedef struct Operand_ *Operand;
typedef struct InterCode_* InterCode;

struct Operand_ {
    enum {VARIABLE, CONSTANT, ADDRESS, TEMP, 
          LABEL, VPOINTER, TPOINTER} kind;
    union {
        int var_no;
        int value;
    }u;
    Operand next;
};

struct InterCode_ {
    enum {ASSIGN, ADD, SUB, MUL, DIVIDE, LABELOP, IFOP, GOTO, 
    RETURNOP, READ, WRITE, CALL, ARG, FUNCTION, PARAM, DEC} kind;
    union {
        struct {Operand left, right; }assign; //assign
        struct {Operand result, op1, op2; }binop; //add, sub, mul, divide
		struct {Operand op; }sigop; //goto, return, read, write, arg, label, param
		struct {Operand op1, op2, label; char* relop; }ifop; //ifop
		struct {char name[50]; } funop; //function
		struct {Operand result; char name[50]; }callop; //call
		struct {Operand op; int size; }decop; //dec
    }u;

    InterCode pre, next;
};

// IRcode function
void insertIR(InterCode c);
void deleteIR(InterCode c);
void printIR(char *filename);
void printOperand(Operand op, FILE *fp);

extern InterCode code_h,code_t;
extern int varCount;
extern int labelCount;



#endif