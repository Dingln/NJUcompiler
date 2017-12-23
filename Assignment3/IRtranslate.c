#include "IRtranslate.h"
#include <assert.h>
#include <string.h>
#include <malloc.h>

InterCode translate_Program(Node *root)
{
    if(root != NULL) 
        return translate_ExtDefList(root->child);
    else 
        return NULL:
}

InterCode translate_ExtDefList(Node *node)
{
    if(node == NULL)
        return NULL;
    Node *node_exdef = node->Child;
    Node *node_exdeflist = node_exdef->Sibling;
    InterCode code1 = translate_ExtDef(node_exdef);
    InterCode code2 = translate_ExtDefList(node_exdeflist);
    return IRcodeConcat(2, code1, code2);
}

InterCode translate_ExtDef(Node *node);
{
    if(node == NULL)
        return NULL;
    
    Node *child2 = node->Child->Sibling;
    
    // ExtDef__Specifier_ExtDecList_SEMI
    if(strcmp(child2->type, "ExtDecList") == 0) {
        return translate_ExtDecList(child2);
    }

    // ExtDef__Specifier_FunDec_Compst
    else if(strcmp(child2->type, "FunDec")==0) {
        InterCode code1 = translate_FunDec(child2);
        InterCode code2 = translate_CompSt(child2->Sibling);
        return IRcodeConcat(2, code1, code2);
    }

    else 
        return NULL;
}

InterCode translate_FunDec(Node *node)
{
    if(node == NULL)
        return NULL:

    Node *Idnode = node->Child;
    InterCode code1 = createFunop(FUNCTION, Idnode->value);

    Node *child3 = Idnode->Sibling->Sibling;
    if(strcmp(child3->type, "VarList") == 0) {
        InterCode code2 = translate_VarList(child3);
        code1 = IRcodeConcat(2, code1, code2);
    }

    return code1;
}

InterCode translate_VarList(Node *node)
{
    if(node == NULL)
        return NULL:
    
    Node *ParamNode = node->Child;
    InterCode code = translate_ParamDec(ParamNode);

    if(ParamNode->Sibling != NULL) {
        child3 = ParamNode->Sibling->Sibling;
        InterCode code2 = translate_VarList(child3);
        code = IRcodeConcat(2, code, code2);
    }
    
    return code;
}

InterCode translate_ParamDec(Node *node)
{
    Node *varNode = node->Child->Sibling;
    InterCode code1 = translate_VarDec(varNode);

    Operand op_param = createOperand(VARIABLE, varCount);
    InterCode code = createSigop(PARAM, op_param);
    return IRcodeConcat(2, code1, code);
}


// TODO: check if always return NULL
InterCode translate_ExtDecList(Node *node)
{
    if(node == NULL)
        return NULL;
    
    Node *varChild = node->Child;
    InterCode code1 = translate_VarDec(varChild);

    Node *child2 = varChild->Sibling;
    if(child2 != NULL) {
        InterCode code2 = translate_ExtDecList(child2->Sibling);
        code1 = IRcodeConcat(2, code1, code2);
    }

    return code1;
}

InterCode translate_VarDec(Node *node)
{
    return NULL:
}




