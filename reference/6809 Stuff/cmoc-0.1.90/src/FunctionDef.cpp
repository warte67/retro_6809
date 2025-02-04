/*  $Id: FunctionDef.cpp,v 1.89 2024/11/11 03:31:30 sarrazip Exp $

    CMOC - A C-like cross-compiler
    Copyright (C) 2003-2016 Pierre Sarrazin <http://sarrazip.com/>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FunctionDef.h"

#include "Scope.h"
#include "DeclarationSequence.h"
#include "CompoundStmt.h"
#include "TranslationUnit.h"
#include "StringLiteralExpr.h"
#include "FormalParameter.h"
#include "ForStmt.h"
#include "WhileStmt.h"
#include "VariableExpr.h"
#include "BinaryOpExpr.h"
#include "UnaryOpExpr.h"
#include "CastExpr.h"
#include "FunctionCallExpr.h"
#include "ObjectMemberExpr.h"
#include "ClassDef.h"
#include "ConditionalExpr.h"
#include "JumpStmt.h"
#include "ExpressionTypeSetter.h"
#include "ScopeCreator.h"
#include "AssemblerStmt.h"
#include "SemanticsChecker.h"
#include "LabeledStmt.h"

#include <assert.h>
#include <iomanip>

using namespace std;


uint16_t FunctionDef::functionStackSpace = 0;


class Tracer : public Tree::Functor
{
public:
    Tracer() : trace(cerr), ind() {}
    virtual ~Tracer() {}
    virtual bool open(Tree *t) override
    {
        assert(t != NULL);
        t->pushScopeIfExists();
        Scope *cs = TranslationUnit::instance().getCurrentScope();
        trace << ind << "open(" << t
                << ") [current scope is now " << cs << "]\n";
        ind += "  ";
        Scope *scope = t->getScope();
        trace << ind << "scope=" << scope << "\n";
        if (scope != NULL)
        {
            vector<string> v;
            scope->getDeclarationIds(v, false);
            trace << ind << "scope at " << scope << " w/ decls: {";
            for (vector<string>::const_iterator it = v.begin();
                                                it != v.end(); it++)
                trace << " " << *it;
            trace << " }\n";
        }
        Declaration *decl = dynamic_cast<Declaration *>(t);
        if (decl != NULL)
            trace << ind << "declaration: " << decl->getVariableId() << ": " << decl->getLineNo() << "\n";
        const VariableExpr *ve = t->asVariableExpr();
        if (ve != NULL)
            trace << ind << "variable expr: " << ve->getId() << ": " << ve->getLineNo() << "\n";
        TreeSequence *ts = dynamic_cast<TreeSequence *>(t);
        if (ts != NULL)
            trace << ind << "tree sequence with " << ts->size() << " statement(s)\n";
        FunctionCallExpr *fce = dynamic_cast<FunctionCallExpr *>(t);
        if (fce != NULL)
            trace << ind << "function call: " << fce->isCallThroughPointer() << ", " << fce->getIdentifier() << "()\n";
        return true;
    }
    virtual bool close(Tree *t) override
    {
        t->popScopeIfExists();
        ind.erase(ind.length() - 2, 2);
        Scope *cs = TranslationUnit::instance().getCurrentScope();
        trace << ind << "close(" << t << ") [current scope is now "
                << cs << "]\n";
        return true;
    }

    ostream &trace;
    string ind;
};


///////////////////////////////////////////////////////////////////////////////


// Counts the number of return statements in the body of a function.
// Remembers the last two trees that the iteration visits.
//
class ReturnStmtChecker : public Tree::Functor
{
public:
    ReturnStmtChecker() : numReturnStmts(0)
    {
        lastClosedTrees[0] = lastClosedTrees[1] = NULL;
    }
    virtual ~ReturnStmtChecker() {}
    virtual bool close(Tree *t) override
    {
        JumpStmt *jump = dynamic_cast<JumpStmt *>(t);
        if (jump && jump->getJumpType() == JumpStmt::RET)
            ++numReturnStmts;
        lastClosedTrees[1] = lastClosedTrees[0];  // new next-to-last tree
        lastClosedTrees[0] = t;  // new last tree
        return true;
    }

    const Tree *getNextToLastTree() const { return lastClosedTrees[1]; }

    size_t numReturnStmts;
    Tree *lastClosedTrees[2];  // does not own the Tree objects

private:
    ReturnStmtChecker(const ReturnStmtChecker &);
    ReturnStmtChecker &operator = (const ReturnStmtChecker &);
};


///////////////////////////////////////////////////////////////////////////////


// Checks that no ID of a labeled-statement is used more than once
// in the same function body.
//
class IDLabeledStatementChecker : public Tree::Functor
{
public:
    IDLabeledStatementChecker() : seenIDs() {}

    // Processing done in open() instead of close() so that the statements are seen in text order.
    virtual bool open(Tree *t) override
    {
        if (const LabeledStmt *ls = dynamic_cast<LabeledStmt *>(t))
        {
            if (!ls->isId())
                return true;
            string id = ls->getId();
            map<string, string>::const_iterator it = seenIDs.find(id);
            if (it != seenIDs.end())
            {
                ls->errormsg("label `%s' already defined at %s", id.c_str(), it->second.c_str());
                return true;
            }
            seenIDs[id] = ls->getLineNo();
        }
        return true;
    }
private:
    map<string, string> seenIDs;  // key: ID; value: line number where ID first seen
};


///////////////////////////////////////////////////////////////////////////////


string
FunctionDef::makeLabelFromFunctionId(const string &id)
{
    return "_" + id;
}


// A convention keyword in the declaration of a function overrides
// #pragma push_calling_convention <convention>.
//
static CallConvention
getCallConventionFromDSL(const DeclarationSpecifierList *dsl)
{
    if (dsl != NULL && dsl->getCallConvention() != DEFAULT_CMOC_CALL_CONV)
        return dsl->getCallConvention();
    return TranslationUnit::instance().getCurrentDefaultCallConvention();
}


FunctionDef::FunctionDef(const DeclarationSpecifierList *dsl,
                         Declarator &declarator)
  : Tree(),
    functionId(declarator.getId()),
    formalParamList(declarator.detachFormalParamList()),
    functionLabel(makeLabelFromFunctionId(functionId)),
    endLabel(TranslationUnit::instance().generateLabel('L')),
    bodyStmts(NULL),
    formalParamDeclarations(),
    hiddenParamDeclaration(NULL),
    numLocalVariablesAllocated(0),
    minDisplacement(9999),  // positive value means allocateLocalVariables() not called yet
    isISR(dsl && dsl->isInterruptServiceFunction()),
    isStatic(dsl && dsl->isStaticDeclaration()),
    asmOnly(dsl && dsl->isAssemblyOnly()),
    noReturnInstruction(dsl && dsl->hasNoReturnInstruction()),
    called(false),
    callConvention(getCallConventionFromDSL(dsl))
{
    // The "interrupt", "__gcccall" and "_CMOC_fpir_" flags only make sense on function types
    // and function pointer types.
    // Set the return type of the function so that this type does not contain those flags,
    // unless the return type is a function pointer type.
    //
    TypeManager &tm = TranslationUnit::instance().getTypeManager();
    const TypeDesc *returnTD = dsl ? dsl->getTypeDesc() : tm.getIntType(WORD_TYPE, true);  // support absence of return type, as per K&R C
    returnTD = declarator.processPointerLevel(returnTD);
    if (! returnTD->isPtrToFunction() && returnTD->isTypeWithCallingConventionFlags())
        returnTD = tm.getTypeWithoutCallingConventionFlags(returnTD);
    setTypeDesc(returnTD);

    /*cout << "# FunctionDef::FunctionDef: " << declarator.getId() << "(): returns {"
         << getTypeDesc()->toString() << "}, formalParamList=(" << formalParamList->toString() << ")\n";*/
    assert(getTypeDesc()->isPtrToFunction() || ! getTypeDesc()->isTypeWithCallingConventionFlags());
}


template <typename T>
inline void deletePtr(T *ptr)
{
    delete ptr;
}


/*virtual*/
FunctionDef::~FunctionDef()
{
    delete formalParamList;

    delete hiddenParamDeclaration;

    for_each(formalParamDeclarations.begin(), formalParamDeclarations.end(), deletePtr<Declaration>);

    delete bodyStmts;
}


bool
FunctionDef::hasHiddenParam() const
{
    return getType() == CLASS_TYPE;
}


bool
FunctionDef::firstParamIsUnsignedChar() const
{
    if (getNumFormalParams() == 0)
        return false;
    const Tree *firstParam = *formalParamList->begin();
    assert(firstParam);
    return firstParam->getType() == BYTE_TYPE && ! firstParam->isSigned();
}


// Note that hiddenParamDeclaration may not have been initialized yet.
//
bool
FunctionDef::hasHiddenParamAndReceivesItInReg() const
{
    if (!hasHiddenParam())
        return false;

    if (callConvention == GCC6809_CALL_CONV)
    {
        ssize_t firstByteArgIndex = -1, firstWordArgIndex = -1;
        getPrototypeTypeDesc()->getGCCCallConventionInfo(firstByteArgIndex, firstWordArgIndex);
        return firstWordArgIndex == -2;
    }

    return callConvention == FIRST_PARAM_IN_REG_CALL_CONV;
}


string
FunctionDef::getAddressOfReturnValue() const
{
    if (hasHiddenParamAndReceivesItInReg())
    {
        assert(hiddenParamDeclaration);
        return hiddenParamDeclaration ? hiddenParamDeclaration->getFrameDisplacementArg() : "???";
    }
    assert(!hiddenParamDeclaration);
    return intToString(Declaration::FIRST_FUNC_PARAM_FRAME_DISPLACEMENT) + ",U";
}


const TypeDesc *
FunctionDef::getPrototypeTypeDesc() const
{
    const TypeDesc *funcPtrTD = TranslationUnit::instance().getTypeManager().getFunctionPointerType(*this);
    assert(funcPtrTD && funcPtrTD->type == POINTER_TYPE && funcPtrTD->pointedTypeDesc->type == FUNCTION_TYPE);
    return funcPtrTD->pointedTypeDesc;
}


string
FunctionDef::getPrototypeWithFunctionId() const
{
    const TypeDesc *td = getPrototypeTypeDesc();
    stringstream ss;
    TypeDesc::printFunctionSignature(ss, *td, false, false, false, 0, functionId);
    return ss.str();
}


// Generates Declaration objects for each formal parameter.
// Stores them in this function's Scope object.
// Sets the declarations' frame displacement.
// Issues error messages if needed (e.g., two parameters with the same name).
// Must be called before setBody().
//
void
FunctionDef::declareFormalParams()
{
    if (formalParamList == NULL)
        return;  // error message already reported by TranslationUnit::registerFunction()

    assert(getScope() != NULL);

    ssize_t firstByteArgIndex = -1, firstWordArgIndex = -1;
    if (callConvention == GCC6809_CALL_CONV)
    {
        const TypeDesc *prototypeTD = getPrototypeTypeDesc();
        prototypeTD->getGCCCallConventionInfo(firstByteArgIndex, firstWordArgIndex);
    }

    int16_t paramFrameDisplacement = Declaration::FIRST_FUNC_PARAM_FRAME_DISPLACEMENT;

    // If return type is struct/union, receive address of return value
    // as hidden parameter.
    //
    if (hasHiddenParam())
    {
        if (hasHiddenParamAndReceivesItInReg())
        {
            const TypeDesc *voidPtrTypeDesc = TranslationUnit::instance().getTypeManager().getPointerToVoid();
            assert(!hiddenParamDeclaration);
            hiddenParamDeclaration = new Declaration("$hidden", voidPtrTypeDesc, vector<uint16_t>(), false, false);
            hiddenParamDeclaration->copyLineNo(*this);
            if (!getScope()->declareVariable(hiddenParamDeclaration))  // scope keeps copy of 'hiddenParamDeclaration' pointer but does not take ownership
                assert(false);
            // hiddenParamDeclaration to be destroyed by the ~FunctionDef().

            // setFrameDisplacement() now called on 'decl' because Scope::allocateLocalVariables() will do it.
        }
        else  // hidden param received in stack
            paramFrameDisplacement += 2;
    }

    vector<Tree *>::const_iterator it = formalParamList->begin();

    for ( ; it != formalParamList->end(); it++)
    {
        const FormalParameter *fp = dynamic_cast<FormalParameter *>(*it);
        assert(fp != NULL);
        uint16_t argIndex = uint16_t(it - formalParamList->begin()) + 1;  // one-based index

        string fpId = fp->getId();
        if (fpId.empty())
            fpId = "$" + wordToString(argIndex);  // to avoid clash between two unnamed parameters

        const TypeDesc *fpTypeDesc = fp->getTypeDesc();
        const vector<uint16_t> &fpArrayDims = fp->getArrayDimensions();
        //cout << "# FunctionDef::declareFormalParams: fpId=" << fpId << ", fpTypeDesc='" << fpTypeDesc->toString() << "', fpArrayDims=" << vectorToString(fpArrayDims) << endl;

        Declaration *decl = new Declaration(fpId, fpTypeDesc, fpArrayDims, false, false);
        decl->copyLineNo(*fp);
        if (!getScope()->declareVariable(decl))  // scope keeps copy of 'decl' pointer but does not take ownership
            errormsg("function %s() has more than one formal parameter named '%s'",
                                                functionId.c_str(), fpId.c_str());

        // Keep a copy of 'decl' so that ~FunctionDef() destroys them.
        formalParamDeclarations.push_back(decl);

        bool formalParamIsLocalVar = false;
        switch (callConvention)
        {
            case FIRST_PARAM_IN_REG_CALL_CONV:
                // This (visible) parameter is passed in a register if it is the first visible parameter,
                // and the function receives no hidden parameter.
                formalParamIsLocalVar = (it == formalParamList->begin() && !hasHiddenParam());
                break;
            case GCC6809_CALL_CONV:
                // argIndex is one-based, while firstByteArgIndex and firstWordArgIndex are zero-based.
                formalParamIsLocalVar = (ssize_t(argIndex) == firstByteArgIndex + 1 || ssize_t(argIndex) == firstWordArgIndex + 1);
                break;
            case DEFAULT_CMOC_CALL_CONV:
                break;
        }

        if (!formalParamIsLocalVar)
        {
            if (TranslationUnit::instance().getTypeSize(*fp->getTypeDesc()) == 1)  // if byte or 1-byte struct/union
                paramFrameDisplacement++;
            decl->setFrameDisplacement(paramFrameDisplacement);
        }

        // If struct, check that it is defined.
        if (fp->getType() == CLASS_TYPE && TranslationUnit::instance().getClassDef(fp->getTypeDesc()->className) == NULL)
        {
            errormsg("argument %u of %s() receives undefined `%s' by value",
                     argIndex, functionId.c_str(), fp->getTypeDesc()->toString().c_str());
            continue;
        }

        if (!formalParamIsLocalVar)
        {
            uint16_t size = 0;
            if (!decl->getVariableSizeInBytes(size))
                decl->errormsg("failed to get size of `%s'", decl->getVariableId().c_str());
            else
                paramFrameDisplacement += int16_t(size);
        }
    }

    // Require at least one named argument before an ellipsis, as does GCC.
    //
    if (formalParamList->endsWithEllipsis() && ! formalParamList->isEllipsisImplied() && formalParamList->size() == 0)
        errormsg("%s %s() uses `...' but has no named argument before it",
                 bodyStmts ? "function" : "prototype",
                 functionId.c_str());
}


// declareFormalParams() must have been called.
//
void
FunctionDef::setBody(TreeSequence *body)
{
    if (body != NULL)
    {
        if (bodyStmts != NULL)
        {
            body->errormsg("%s() already has a body at %s", functionId.c_str(), bodyStmts->getLineNo().c_str());
            delete body;
        }
        else
            bodyStmts = body;
    }
}

    
const TreeSequence *
FunctionDef::getBody() const
{
    return bodyStmts;
}


TreeSequence *
FunctionDef::getBody()
{
    return bodyStmts;
}


string
FunctionDef::getId() const
{
    return functionId;
}


string
FunctionDef::getLabel() const
{
    return functionLabel;
}


string
FunctionDef::getEndLabel() const
{
    return endLabel;
}


bool
FunctionDef::hasSameReturnTypeAndModifiers(const FunctionDef &fd) const
{
    return *getTypeDesc() == *fd.getTypeDesc()  // deep comparison (the 2 ptrs may differ)
        && isInterruptServiceRoutine() == fd.isInterruptServiceRoutine()
        && getCallConvention() == fd.getCallConvention();
}


bool
FunctionDef::hasSameFormalParams(const FunctionDef &fd) const
{
    const FormalParamList *otherFormalParams = fd.formalParamList;
    if (otherFormalParams == NULL)
        return formalParamList == NULL;
    if (formalParamList == NULL)
        return false;

    if (otherFormalParams->size() != formalParamList->size())
        return false;

    if (otherFormalParams->endsWithEllipsis() != formalParamList->endsWithEllipsis())
        return false;

    vector<Tree *>::const_iterator it = formalParamList->begin();
    vector<Tree *>::const_iterator itOther = otherFormalParams->begin();
    for ( ; it != formalParamList->end(); it++, itOther++)
    {
        FormalParameter *fp = dynamic_cast<FormalParameter *>(*it);
        if (fp == NULL)
        {
            assert(false);
            return false;
        }

        FormalParameter *otherFp = dynamic_cast<FormalParameter *>(*itOther);
        if (otherFp == NULL)
        {
            assert(false);
            return false;
        }

        if (*fp->getTypeDesc() != *otherFp->getTypeDesc())
            return false;
    }

    return true;
}


size_t
FunctionDef::getNumFormalParams() const
{
    return formalParamList ? formalParamList->size() : 0;
}


void
FunctionDef::setCalled()
{
    called = true;
}


bool
FunctionDef::isCalled() const
{
    return called;
}


// Also declares the function's formal parameters in the function's Scope object.
//
/*virtual*/
void
FunctionDef::checkSemantics(Functor &f)
{
    SemanticsChecker &sem = dynamic_cast<SemanticsChecker &>(f);
    if (bodyStmts)
        sem.setCurrentFunctionDef(this);

    /*  Create a scope for the function.
        Make it a child of the global scope.
        This means the global Scope object owns 'scope'.
        When the global Scope will be destroyed, delete will be called on 'scope'.
    */
    assert(getScope() == NULL);
    setScope(new Scope(&TranslationUnit::instance().getGlobalScope(), getLineNo()));
    assert(getScope()->getParent() == &TranslationUnit::instance().getGlobalScope());

    /*  An interrupt service routine is not allowed to receive parameters under most platforms,
        because it is only called by the system, which does not typically provide any parameters.
        Under OS-9, the routine is expected to receive an unsigned char parameter in register B.
        On that platform, a warning is issued if the parameter does not comply with that expectation.
    */
    if (isISR)
    {
        TargetPlatform platform = TranslationUnit::instance().getTargetPlatform();
        if (platform == OS9)
        {
            size_t numParams = getNumFormalParams();
            if (numParams == 0 || (numParams == 1 && !hasHiddenParam() && firstParamIsUnsignedChar() && callConvention != DEFAULT_CMOC_CALL_CONV))
                ;  // ok
            else
                warnmsg("interrupt service routine %s() on targeted platform %s should receive a single `unsigned char' parameter and use the __gcccall keyword",
                            functionId.c_str(),
                            getTargetPlatformName(platform));
        }
    }

    /*  Forbid _CMOC_fpir_ if the function's first visible parameter is a struct or larger than 2 bytes,
        and the function has no hidden parameter.
    */
    if (callConvention == FIRST_PARAM_IN_REG_CALL_CONV && !hasHiddenParam() && formalParamList->size() >= 1)
    {
         const TypeDesc *firstParamTD = (*formalParamList->begin())->getTypeDesc();
         int16_t firstParamSize = TranslationUnit::instance().getTypeSize(*firstParamTD);
         if (firstParamSize > 2 || firstParamTD->type == CLASS_TYPE)
             errormsg("_CMOC_fpir_ not allowed on function whose first parameter is struct, union or larger than 2 bytes");
    }

    /*  main() not allowed to be static.
    */
    if (getId() == "main" && hasInternalLinkage())
        errormsg("main() must not be static");

    /*  Declare the function's formal parameters in 'scope'.
    */
    declareFormalParams();

    if (bodyStmts)
    {
        /*  Create a Scope for each compound statement anywhere in the function's body.
            The function's main braces do not get their own scope. They are part of
            the function's scope.
        */
        {
            ScopeCreator sc(TranslationUnit::instance(), getScope());
            bodyStmts->iterate(sc);
        }  // destroy ScopeCreator here so that it pops all scopes it pushed onto the TranslationUnit's stack

        static const bool debug = (getenv("DEBUG") != 0);
        if (debug)
        {
            Tracer tracer;
            bodyStmts->iterate(tracer);
        }

        if (asmOnly)
        {
            for (vector<Tree *>::const_iterator it = bodyStmts->begin(); it != bodyStmts->end(); ++it)
            {
                if (AssemblerStmt *asmStmt = dynamic_cast<AssemblerStmt *>(*it))
                    asmStmt->setAssemblyOnly(getScope());
                else
                {
                    (*it)->errormsg("body of function %s() contains statement(s) other than inline assembly",
                                    functionId.c_str());
                    return;
                }
            }
            return;
        }
        if (noReturnInstruction && !asmOnly)
            errormsg("`__norts__' must be used with `asm' when defining an asm-only function");

        ExpressionTypeSetter ets;
        bodyStmts->iterate(ets);

        // Check if a non-void returning function contains at least one return statement.
        // (This does not prove that all code paths have a return statement however.)
        //
        // Do not issue a warning if the last statement in the function body is an
        // asm{} statement, because the return value will typically be stored directly
        // in B or D by that inline assembly code.
        //
        if (getType() != VOID_TYPE)
        {
            ReturnStmtChecker rsc;
            bodyStmts->iterate(rsc);
            if (rsc.numReturnStmts == 0 && ! dynamic_cast<const AssemblerStmt *>(rsc.getNextToLastTree()))
                warnmsg("function '%s' is not void but does not have any return statement", functionId.c_str());
        }

        // Check ID-labeled statements.
        {
            IDLabeledStatementChecker checker;
            bodyStmts->iterate(checker);
        }
    }
}


void
FunctionDef::allocateLocalVariables()
{
    assert(minDisplacement > 0);  // must be first call
    assert(getScope() != NULL);
    assert(getScope()->getParent() != NULL);  // function's scope is not the global one

    if (bodyStmts == NULL)
        return;  // no body: nothing to do

    assert(bodyStmts->getScope() == NULL);  // function's top-level braced scope is the one returned by getScope()

    numLocalVariablesAllocated = 0;
    minDisplacement = getScope()->allocateLocalVariables(0, true, numLocalVariablesAllocated);

    assert(minDisplacement <= 0);
}


/*virtual*/
CodeStatus
FunctionDef::emitCode(ASMText &out, bool lValue) const
{
    assert(getScope() != NULL);
    assert(getScope()->getParent() != NULL);  // function's scope is not the global one
    assert(getPrototypeTypeDesc()->getCallConvention() == callConvention);

    if (bodyStmts == NULL)
        return true;

    if (lValue)
        return false;

    assert(minDisplacement <= 0);  // allocateLocalVariables() must have been called

    // Generate code that sets up the function's stack frame:
    
    out.emitSeparatorComment();
    out.emitFunctionStart(functionId, getLineNo());
    out.emitLabel(functionLabel);

    if (hasInternalLinkage())  // emit an extra label to help avoid warnings on multiple defs of static symbols
        out.emitLabel(".static.function." + functionId);

    out.emitComment("Prototype: " + getPrototypeWithFunctionId());

    if (isInterruptServiceRoutine())
        out.emitComment("Interrupt service routine.");

    if (isAssemblyOnly())
        out.emitComment("Assembly-only function.");
    else
        out.emitCallConventionMarker(callConvention);

    // A stack frame is only needed if the function:
    // - receives parameters or declares local variables or returns a struct (including a real number);
    // and:
    // - is not an asm-only function (the point of which is to forego the stack frame).
    //
    // (We use numLocalVariablesAllocated to determine if this functoin has
    // local variables, instead of minDisplacement < 0, because minDisplacement
    // can be 0 if all locals are empty structs.)
    //
    bool needStackFrame = true;
    if (asmOnly)
        needStackFrame = false;
    else if (TranslationUnit::instance().getFramePointerOption() != FramePointerOption::KEEP_ALL)
        needStackFrame = (getNumFormalParams() > 0 || numLocalVariablesAllocated > 0 || getType() == CLASS_TYPE);

    if (needStackFrame)
        out.ins("PSHS", "U");

    // Function-entry stack check, if enabled. This is the point where it is done under OS-9.
    if (!asmOnly && getFunctionStackSpace() > 0)
    {
        // Call a utility routine that receives it argument as a word that follows the call.
        // This avoids trashing a register.
        // The routine (see crt.asm) knows about the argument and adjusts the stacked return address accordingly.
        //
        callUtility(out, "_stkcheck");
        out.ins("FDB", "-" + wordToString(getFunctionStackSpace() - minDisplacement), "argument for _stkcheck");
    }

    if (needStackFrame)
    {
        out.ins("LEAU", ",S");  // takes 4 cycles and 2 bytes; TFR U,S takes 6 cycles
        if (minDisplacement < 0)
            out.ins("LEAS", intToString(minDisplacement) + ",S");
    }

    if (TranslationUnit::instance().isStackOverflowCheckingEnabled())
    {
        callUtility(out, "check_stack_overflow");
    }


    if (!emitParameterSpillInstructions(out))
        return false;


    // Issue comments indicating where the parameters and locals are allocated.

    vector<string> declarationIds;
    getScope()->getDeclarationIds(declarationIds, true);
    if (declarationIds.size() > 0 && !isAssemblyOnly())
    {
        map<int16_t, string> paramsMap, localsMap;  // key: offset from frame ptr; value: description

        // Fill the maps from the non-extern declarationIds.
        //
        for (vector<string>::const_iterator it = declarationIds.begin(); it != declarationIds.end(); ++it)
        {
            const Declaration *decl = getScope()->getVariableDeclaration(*it, false, true);  // look in sub-scopes
            assert(decl);
            if (decl->isExtern || decl->isLocalStatic())
                continue;
            uint16_t sizeInBytes = 0;
            if (!decl->getVariableSizeInBytes(sizeInBytes, false))
                assert(!"Declaration::getVariableSizeInBytes() failed");
            stringstream ss;
            ss << setw(8) << decl->getFrameDisplacementArg(0)
               << ": " << setw(4) << sizeInBytes << " byte" << (sizeInBytes == 1 ? " " : "s")
               << ": " << *it
               << ": " << decl->getTypeDesc()->toString()
               << ": line " << decl->getIntLineNo();
            int16_t displacement = decl->getFrameDisplacement(0);
            (displacement >= 0 ? paramsMap : localsMap)[displacement] = ss.str();
        }

        // Emit the comments from the maps.
        //
        if (paramsMap.size() > 0)
        {
            out.emitComment("Formal parameter(s):");
            for (auto const &pair : paramsMap)
                out.emitComment(pair.second);
        }
        if (localsMap.size() > 0)
        {
            out.emitComment("Local non-static variable(s):");
            for (auto const &pair : localsMap)
                out.emitComment(pair.second);
        }
    }

    // Generate code for the body:

    TranslationUnit::instance().setCurrentFunctionEndLabel(endLabel);
    TranslationUnit::instance().pushScope(const_cast<Scope *>(getScope()));  // const_cast should be removed...
    bool success = bodyStmts->emitCode(out, false);
    out.emitLabel(endLabel, "end of " + functionId + "()");

    if (callConvention == GCC6809_CALL_CONV && getTypeSize() == 2)
        out.ins("TFR", "D,X", "16-bit result returned in X as per GCC6809 convention");

    // The scope must be popped whether or not this function succeeds.
    TranslationUnit::instance().popScope();

    TranslationUnit::instance().setCurrentFunctionEndLabel("");

    if (needStackFrame)
    {
        assert(!asmOnly);
        out.ins("LEAS", ",U");  // takes 4 cycles and 2 bytes; TFR U,S takes 6 cycles
        if (isISR)
        {
            out.ins("PULS", "U");
            out.ins("RTI");
        }
        else
            out.ins("PULS", "U,PC");
    }
    else
    {
        if (!noReturnInstruction)
            out.ins(isISR ? "RTI" : "RTS");
    }

    out.emitFunctionEnd(functionId, getLineNo());

    return success;
}


bool
FunctionDef::iterate(Functor &f)
{
    if (!f.open(this))
        return false;
    if (bodyStmts != NULL && !bodyStmts->iterate(f))
        return false;
    if (!f.close(this))
        return false;
    return true;
}


const FormalParamList *
FunctionDef::getFormalParamList() const
{
    return formalParamList;
}


bool
FunctionDef::isAcceptableNumberOfArguments(size_t numArguments) const
{
    if (formalParamList == NULL)
        return numArguments == 0;

    return formalParamList->isAcceptableNumberOfArguments(numArguments);
}


class IDLabeledStatementFinder : public Tree::Functor
{
public:
    IDLabeledStatementFinder(const string &_targetID) : foundAsmLabel(), targetID(_targetID) {}
    virtual bool close(Tree *t) override
    {
        if (const LabeledStmt *ls = dynamic_cast<LabeledStmt *>(t))
        {
            string asmLabel = ls->getAssemblyLabelIfIDEqual(targetID);
            if (!asmLabel.empty())
            {
                foundAsmLabel = asmLabel;
                return false;  // stop iteration
            }
        }
        return true;
    }

    string foundAsmLabel;
private:
    string targetID;
};


string
FunctionDef::findAssemblyLabelFromIDLabeledStatement(const std::string &id) const
{
    if (!bodyStmts)
        return string();

    IDLabeledStatementFinder finder(id);
    bodyStmts->iterate(finder);
    return finder.foundAsmLabel;
}


void
FunctionDef::setInterruptServiceRoutine(bool _isISR)
{
    isISR = _isISR;
}


bool
FunctionDef::isInterruptServiceRoutine() const
{
    return isISR;
}


void
FunctionDef::setCallConvention(CallConvention callConv)
{
    callConvention = callConv;
}


CallConvention
FunctionDef::getCallConvention() const
{
    return callConvention;
}


bool
FunctionDef::isAssemblyOnly() const
{
    return asmOnly;
}


bool
FunctionDef::hasInternalLinkage() const
{
    return isStatic;
}


uint16_t
FunctionDef::getFunctionStackSpace()
{
    return functionStackSpace;
}


void
FunctionDef::setFunctionStackSpace(uint16_t numBytes)
{
    functionStackSpace = numBytes;
}


// Emits store instructions depending on asmOnly and callConvention.
//
CodeStatus
FunctionDef::emitParameterSpillInstructions(ASMText &out) const
{
    if (asmOnly)
        return true;  // nothing to do

    if (callConvention == FIRST_PARAM_IN_REG_CALL_CONV)
    {
        assert(formalParamList);
        if (hasHiddenParam())
        {
            const Declaration *decl = getScope()->getVariableDeclaration("$hidden", false);
            assert(decl);
            assert(decl->getType() == POINTER_TYPE);
            Register reg = TranslationUnit::instance().getFirstParameterRegister(false);
            out.ins(string("ST") + getRegisterName(reg), decl->getFrameDisplacementArg(0), "spill hidden parameter");
        }
        else if (formalParamList->size() > 0)
        {
            const FormalParameter *fp = dynamic_cast<FormalParameter *>(*formalParamList->begin());
            assert(fp != NULL);
            const Declaration *decl = getScope()->getVariableDeclaration(fp->getId(), false);
            assert(decl);
            Register reg = TranslationUnit::instance().getFirstParameterRegister(fp->getType() == BYTE_TYPE);
            out.ins(string("ST") + getRegisterName(reg), decl->getFrameDisplacementArg(0),
                                    "spill parameter `" + fp->getId() + "', declared at " + decl->getLineNo());
        }
    }
    else if (callConvention == GCC6809_CALL_CONV)
    {
        const TypeDesc *prototypeTD = getPrototypeTypeDesc();

        // Get the indexes of the args to spill to the stack.
        ssize_t firstByteArgIndex, firstWordArgIndex;
        prototypeTD->getGCCCallConventionInfo(firstByteArgIndex, firstWordArgIndex);

        if (firstByteArgIndex >= 0)
        {
            const FormalParameter *fp = dynamic_cast<FormalParameter *>(*(formalParamList->begin() + firstByteArgIndex));
            assert(fp != NULL);
            const Declaration *decl = getScope()->getVariableDeclaration(fp->getId(), false);
            assert(decl);
            out.ins("STB", decl->getFrameDisplacementArg(0),
                                    "spill parameter `" + fp->getId() + "', declared at " + decl->getLineNo());
        }
        if (firstWordArgIndex == -2)  // if hidden parameter received in register
        {
            const Declaration *decl = getScope()->getVariableDeclaration("$hidden", false);
            assert(decl);
            assert(decl->getType() == POINTER_TYPE);
            out.ins("STX", decl->getFrameDisplacementArg(0), "spill hidden parameter");
        }
        else if (firstWordArgIndex >= 0)
        {
            const FormalParameter *fp = dynamic_cast<FormalParameter *>(*(formalParamList->begin() + firstWordArgIndex));
            assert(fp != NULL);
            const Declaration *decl = getScope()->getVariableDeclaration(fp->getId(), false);
            assert(decl);
            out.ins("STX", decl->getFrameDisplacementArg(0),
                                    "spill parameter `" + fp->getId() + "', declared at " + decl->getLineNo());
        }
    }

    return true;
}
