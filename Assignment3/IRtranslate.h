#ifndef __IRTRANSLATE_
#define __IRTRANSLATE_

#include "IR.h"
#include "semantic.h"

InterCode translate_Program(Node *root);
InterCode translate_ExtDefList(Node *node);
InterCode translate_ExtDef(Node *node);
InterCode translate_ExtDecList(Node *node);
InterCode translate_FunDec(Node *node);
InterCode translate_VarDec(Node *node);
InterCode translate_VarList(Node *node);
InterCode translate_ParamDec(Node *node);

InterCode translate_CompSt(Node *node);
InterCode translate_DefList(Node *node);
InterCode translate_DecList(Node *node);
InterCode translate_StmtList(Node *node);
InterCode translate_Stmt(Node *node);
InterCode translate_Def(Node *node);
InterCode translate_Dec(Node *node);

InterCode translate_Exp(Node *node);
InterCode translate_Cond(Node *node);
InterCode translate_Args(Node *node);


#endif
