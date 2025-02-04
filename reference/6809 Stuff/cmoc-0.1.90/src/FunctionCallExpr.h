/*  $Id: FunctionCallExpr.h,v 1.33 2024/07/13 21:14:53 sarrazip Exp $

    CMOC - A C-like cross-compiler
    Copyright (C) 2003-2015 Pierre Sarrazin <http://sarrazip.com/>

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

#ifndef _H_FunctionCallExpr
#define _H_FunctionCallExpr

#include "Tree.h"

class TreeSequence;
class Declaration;


class FunctionCallExpr : public Tree
{
public:

    FunctionCallExpr(Tree *func, TreeSequence *args);

    virtual ~FunctionCallExpr();

    /** Checks that this function call is valid and sets the return TypeDesc.
        If errors are detected, they are reported.
        @returns                        true if the call is valid,
                                        false if errors were detected
    */
    bool checkAndSetTypes();

    /** Determines if this call is made as in pf(), (*pf)() or obj.member(),
        or if it is a standard, direct function call.
    */
    bool isCallThroughPointer() const;

    /** If this expression consists solely of an identifier, return it.
        Otherwise, returns an empty string.
        An identifier is used for standard calls and for pointer-to-function calls
        of the form pf().
    */
    std::string getIdentifier() const;

    /** Call this with a non-null Declaration pointer when the function call
        uses a function pointer variable, or with NULL when the call is
        directly to a global function name.
    */
    void setFunctionPointerVariableDeclaration(Declaration *_funcPtrVarDecl);

    bool hasFunctionPointerVariableDeclaration() const { return funcPtrVarDecl != NULL; }

    virtual void checkSemantics(Functor &f) override;

    virtual CodeStatus emitCode(ASMText &out, bool lValue) const override;

    virtual bool iterate(Functor &f) override;

    virtual bool isLValue() const override { return false; }

    enum Diagnostic
    {
        NO_PROBLEM,
        ERROR_MSG,
        WARN_CONST_INCORRECT,
        WARN_NON_PTR_ARRAY_FOR_PTR,
        WARN_PASSING_CHAR_CONSTANT_FOR_PTR,
        WARN_PASSING_NON_ZERO_CONSTANT_FOR_PTR,
        WARN_ARGUMENT_TOO_LARGE,
        WARN_REAL_FOR_INTEGRAL,
        WARN_FUNC_PTR_FOR_PTR,
        WARN_DIFFERENT_SIGNEDNESS,
        WARNING_VOID_POINTER,
        WARN_PTR_FOR_INTEGRAL,
    };
    static Diagnostic paramAcceptsArg(const TypeDesc &paramTD, const Tree &argTree);

private:

    const TypeDesc *getCalledPrototypeTypeDesc() const;

    void checkCallArguments(const std::string &functionId,
                            class Contraption &contraption,
                            const TreeSequence &args) const;
    void checkPrintfArguments(const TreeSequence &args, const std::string &functionId) const;
    bool emitPushSingleArg(ASMText &out, bool passInReg, bool gccCall,
                           Register regContainingArg, uint16_t &numBytesPushed,
                           const std::string &pshsComment,
                           const class FormalParameter *param) const;
    bool emitArgumentPushCode(ASMText &out,
                              const std::string &functionId,
                              CallConvention callConv,
                              uint16_t &numBytesPushed) const;
    bool passesHiddenParam() const;
    bool isCallToConstantAddress(std::string &jsrArg) const;

    // Forbidden:
    FunctionCallExpr(const FunctionCallExpr &);
    FunctionCallExpr &operator = (const FunctionCallExpr &);

private:

    Tree *function;  // IdentifierExpr for f() and for ptrToF(); UnaryOpExpr or ObjectMemberExpr (typically) for (*expr)(); owns the pointed object
    Declaration *funcPtrVarDecl;  // non null when calling through function pointer variable
    TreeSequence *arguments;  // visible (non-hidden) arguments; owns the pointed object
    Declaration *returnValueDeclaration;  // used when return type is struct/union; owns the pointed object
    
    // __gcccall:
    Declaration *tempDeclarationForFirstByteParam;
    uint8_t firstByteParamImmedValue;  // used if tempDeclarationForFirstByteParam is null
    Declaration *tempDeclarationForFirstWordParam;
    uint16_t firstWordParamImmedValue;  // used if tempDeclarationForFirstWordParam is null

};


#endif  /* _H_FunctionCallExpr */
