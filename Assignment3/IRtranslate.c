#include "IRtranslate.h"
#include <assert.h>
#include <string.h>
#include <malloc.h>

Operand constant_0 = createOperand(CONSTANT, 0);
Operand constant_1 = createOperand(CONSTANT, 1);

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

    // get ID from VarDec
    FieldList *myid = NULL;
    if(strcmp(varNode->Child->type, "ID") == 0) 
        myid = findVar(varNode->Child->value);
    else if(strcmp(varNode->Child->Child->type, "ID") == 0)
        myid = findVar(varNode->Child->Child->value);

    Operand op_param = createOperand(VARIABLE, myid->var_no);
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

InterCode translate_CompSt(Node *node)
{
    if(node == NULL)
        return NULL;

    Node *node_deflist = node->Child->Sibling;
    Node *node_stmtlist = node_deflist->Sibling;
    InterCode code1 = translate_DefList(node_deflist);
    InterCode code2 = translate_StmtList(node_stmtlist);

    return IRcodeConcat(2, code1, code2);
}

InterCode translate_DefList(Node *node)
{
    if(node == NULL)
        result NULL;
    
    Node *node_def = node->Child;
    Node *node_deflist = node_def->Sibling;
    InterCode code1 = translate_Def(node_def);
    InterCode code2 = translate_DefList(node_deflist);

    return IRcodeConcat(2, code1, code2);
}

InterCode translate_Def(Node *node)
{
    if(node == NULL)
        return NULL;
    
    Node *node_declist = node->Child->Sibling;
    InterCode code = translate_DecList(node_declist);

    return code;
}

InterCode translate_DecList(Node *node)
{
    if(node == NULL)
        return NULL;

    Node *node_dec = node->Child;
    InterCode code1 = translate_Dec(node_dec);

    if(node_dec->Sibling != NULL) {
        Node *node_declist = node_dec->Sibling->Sibling;
        InterCode code2 = translate_DecList(node_declist);
        code1 = IRcodeConcat(2, code1, code2); 
    }

    return code1;
}


// TODO: need check, vardec if is struct type
InterCode translate_Dec(Node *node)
{
    if(node == NULL)
        return NULL;
    
    Node *node_vardec = node->Child;
    InterCode code = translate_VarDec(node_vardec);

    if(node_vardec->Sibling != NULL) {
        Node *node_vardec = node->Child;
        Node *node_exp = node_vardec->Sibling->Sibling;

        // get ID from VarDec
        FieldList *myid = NULL;
        if(strcmp(node_vardec->Child->type, "ID") == 0) 
            myid = findVar(node_vardec->Child->value);
        else if(strcmp(node_vardec->Child->Child->type, "ID") == 0)
            myid = findVar(node_vardec->Child->Child->value);

        Operand right = createTemp();
        InterCode code1 = translate_Exp(node_exp, right);
        Operand left = createOperand(VARIABLE, myid->val_no);
        InterCode code2 = createAssign(ASSIGN, left, right);

        code = IRcodeConcat(3, code, code1, code2);
    }
    return code;
}

InterCode translate_StmtList(Node *node)
{
    if(node == NULL)
        return NULL;
    
    Node *node_stmt = node->Child;
    Node *node_stmtlist = node_stmt->Sibling;
    InterCode code1 = translate_Stmt(node_stmt);
    InterCode code2 = translate_StmtList(node_stmtlist);

    return IRcodeConcat(2, code1, code2);
}

InterCode translate_Stmt(Node *node)
{
    if(node == NULL)
        return NULL;

    Node *child1 = node->Child;
    
    // Stmt__Exp_SEMI
    if(strcmp(child1->type, "Exp") == 0) {
        return translate_Exp(child1, NULL);
    }

    // Stmt__Compst
    else if(strcmp(child1->type, "CompSt") == 0) {
        return translate_CompSt(child1);
    }

    // Stmt__RETURN_Exp_SEMI
    else if(strcmp(child1->type, "RETURN") == 0) {
        Operand t1 = createTemp();
        Node *node_exp = child1->Sibling;
        InterCode code1 = translate_Exp(node_exp, t1);
        InterCode code2 = createSigop(RETURNOP, t1);
        return IRcodeConcat(2, code1, code2);
    }

    else if(strcmp(child1->type, "IF") == 0) {
        Node *child5 = child1->Sibling->Sibling->Sibling->Sibling;
        
        Operand label1 = createLable();
        Operand label2 = createLable();
        Node *node_exp = child1->Sibling->Sibling;
        Node *node_stmt = child5;
        InterCode code1 = translate_Cond(node_exp, label1, label2);
        InterCode code2 = translate_Stmt(node_stmt);
        InterCode lablecode1 = createSigop(LABEL, label1);
        InterCode lablecode2 = createSigop(LABEL, label2);
        
        // Stmt__IF_LP_Exp_RP_Stmt
        if(child5->Sibling == NULL) {
            return IRcodeConcat(4, code1, lablecode1, code2, lablecode2);
        }
        
        // Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt
        else {
            Node *node_stmt2 = child5->Sibling->Sibling;
            Operand label3 = createLable();
            InterCode code3 = translate_Stmt(node_stmt2);
            InterCode labelcode3 = createSigop(LABEL, label3);
            InterCode gotocode = createSigop(GOTO, label3);
            return IRcodeConcat(7, code1, lablecode1, code2, gotocode, lablecode2, code3, labelcode3);
        }
    }
    
    // Stmt__WHILE_LP_Exp_RP_Stmt
    else {
        Node *node_exp = child1->Sibling->Sibling;
        Node *node_stmt = node_exp->Sibling->Sibling;

        Operand label1 = createLable();
        Operand label2 = createLable();
        Operand label3 = createLable();
        
        InterCode code1 = translate_Cond(node_exp, label1, label2);
        InterCode code2 = translate_Stmt(node_stmt);
        InterCode lablecode1 = createSigop(LABEL, label1);
        InterCode lablecode2 = createSigop(LABEL, label2);
        InterCode gotocode = createSigop(GOTO, label1);

        return IRcodeConcat(6, lablecode1, code1, labelcode2, code2, gotocode, labelcode3);
    }
}

InterCode translate_Exp(Node *node, Operand place)
{
    if(node == NULL || place == NULL)
        return NULL;
    
    Node *child = node->Child;

    // Exp__INT
    if(strcmp(child->type, "INT") == 0) {
        int val;
        sscanf(child->value, "%d", &val);
        Operand right = createOperand(CONSTANT, val);
        InterCode code = createAssign(ASSIGN, place, right);
        return code;
    }

    // Exp__ID
    else if(strcmp(child->type, "ID") == 0) {
        FieldList *myid = findvar(child->value); 
        Operand right = createOperand(VARIABLE, myid->val_no);
        InterCode code = createAssign(ASSIGN, place, right);
        return code;
    }

    // Exp__Exp_ASSIGNOP_Exp
    else if(strcmp(child->type, "EXP") == 0 &&
            strcmp(child->Sibling->type, "ASSIGNOP") == 0) {
             
                Node *node_exp = child;
                Node *node_exp2 = node_exp->Sibling->Sibling;

                // Exp1 is ID 
                if(node_exp->Child->Sibling == NULL) {
                    FieldList *myid = findVar(node_exp->Child->value);
                    Operand var = createOperand(VARIABLE, myid->val_no);

                    Operand t1 = createTemp();
                    InterCode code1 = translate_Exp(node_exp2, t1);
                    InterCode tempcode1 = createAssign(ASSIGN, var, t1);
                    InterCode tempcode2 = createAssign(ASSIGN, place, var);
                    InterCode code2 = IRcodeConcat(2, tempcode1, tempcode2);
                    
                    return IRcodeConcat(2, code1, code2);
                }

                // TODO: Exp1 is array or struct type
                else 
                    return NULL;
            }
    
    // Exp__Exp_PLUS_Exp, 
    // Exp__Exp_MINUS_Exp, 
    // Exp__Exp_STAR_Exp, 
    // Exp__Exp_DIV_Exp
    else if(strcmp(child->type, "EXP") == 0 && 
            ( strcmp(child->Sibling->type, "PLUS") == 0 ||
              strcmp(child->Sibling->type, "MINUS") == 0 ||
              strcmp(child->Sibling->type, "STAR") == 0 ||
              strcmp(child->Sibling->type, "DIV") == 0 )) {

                Node *node_exp1 = child;
                Node *node_exp2 = node_exp1->Sibling->Sibling;
                  
                Operand t1 = createTemp();
                Operand t2 = createTemp();
                InterCode code1 = translate_Exp(node_exp1, t1);
                InterCode code2 = translate_Exp(node_exp2, t2);
                  
                InterCode code3 = NULL;
                if(strcmp(child->Sibling->type, "PLUS") == 0) 
                    code3 = createBinop(ADD, place, t1, t2);
                else if(strcmp(child->Sibling->type, "MINUS") == 0)
                    code3 = createBinop(SUB, place, t1, t2);
                else if(strcmp(child->Sibling->type, "STAR") == 0)
                    code3 = createBinop(MUL, place, t1, t2);
                else if(strcmp(child->Sibling->type, "DIV") == 0)
                    code3 = createBinop(DIVIDE, place, t1, t2);
                
                return IRcodeConcat(3, code1, code2, code3);
            }
    
    // Exp__MINUS_Exp
    else if(strcmp(child->type, "MINUS") == 0) {
        Node *node_exp = child->Sibling;

        Operand t1 = createTemp();
        InterCode code1 = translate_Exp(node_exp, t1);
        InterCode code2 = createBinop(SUB, place, constant_0, t1);

        return IRcodeConcat(2, code1, code2);
    }

    // Exp__Exp_RELOP_Exp,
    // Exp__NOT_Exp,
    // Exp__Exp_AND_Exp,
    // Exp__Exp_OR_Exp
    else if( strcmp(child->type, "NOT") == 0 || 
            ( strcmp(child->type, "EXP") == 0 && 
             ( strcmp(child->Sibling->type, "AND") == 0 ||
               strcmp(child->Sibling->type, "OR") == 0 ||
               strcmp(child->Sibling->type, "RELOP") == 0 ) )) {

                Operand label1 = createLable();
                Operand label2 = createLable();
                InterCode code0 = createAssign(ASSIGN, place, constant_0);
                InterCode code1 = translate_Cond(node, label1, label2);
                InterCode tempcode1 = createSigop(LABEL, label1);
                InterCode tempcode2 = createAssign(ASSIGN, place, constant_1);
                InterCode code2 = IRcodeConcat(2, tempcode1, tempcode2);
                InterCode label2_code = createSigop(LABEL, label2);

                return IRcodeConcat(4, code0, code1, code2, label2_code);
            }
    
    // Exp__ID_LP_RP
    else if( strcmp(child->type, "ID") == 0  &&
             strcmp( child->Sibling->Sibling->type, "RP") == 0) {
                char * myfunc = child->value;
                // read()
                if(strcmp(myfunc, "READ") == 0) {
                    return createFunop(FUNCTION, place);
                }

                return createCallop(CALL, place, myfunc);
            }

    else if( strcmp(child->type, "ID") == 0  && 
             strcmp(child->Sibling->Sibling->type, "Args") == 0) {
                
                char * myfunc = child->value;
                Node *node_arg = child->Sibling->Sibling;

                Operand arglist = NULL;
                InterCode code1 = translate_Args(node_arg, arglist);
                
                // write()
                if(strcmp(myfunc, "WRITE") == 0) {
                    InterCode tempcode1 = createSigop(WRITE, arglist->next);
                    return IRcodeConcat(2, code1, tempcode1);
                }

                


            }
            



}
