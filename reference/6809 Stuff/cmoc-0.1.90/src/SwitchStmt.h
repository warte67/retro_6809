/*  CMOC - A C-like cross-compiler
    Copyright (C) 2003-2024 Pierre Sarrazin <http://sarrazip.com/>

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

#ifndef _H_SwitchStmt
#define _H_SwitchStmt

#include "TreeSequence.h"


class SwitchStmt : public Tree
{
public:

    enum JumpMode { IF_ELSE, JUMP_TABLE };

    static void forceJumpMode(JumpMode _forcedJumpMode);

    // Represents a case statement or the default statement.
    //
    struct SwitchCase
    {
        bool isDefault;  // false means 'case'
        uint32_t caseValue;  // may be interpreted as a signed value
        std::string lineNo;  // source filename and line number where 'case' or 'default' keyword appears
        std::vector<const Tree *> statements;

        SwitchCase(bool _isDefault, uint32_t _caseValue, const std::string _lineNo)
            : isDefault(_isDefault), caseValue(_caseValue), lineNo(_lineNo), statements() {}
    };

    typedef std::vector<SwitchCase> SwitchCaseList;

    SwitchStmt(Tree *_expression, Tree *_statement);

    virtual ~SwitchStmt();

    virtual CodeStatus emitCode(ASMText &out, bool lValue) const override;

    virtual bool iterate(Functor &f) override;

    virtual void checkSemantics(Functor &f) override;

    virtual void replaceChild(Tree *existingChild, Tree *newChild) override
    {
        if (deleteAndAssign(expression, existingChild, newChild))
            return;
        if (deleteAndAssign(statement, existingChild, newChild))
            return;
        assert(!"child not found");
    }

    virtual bool isLValue() const override { return false; }

    // first = case value; second = index in cases[].
    typedef std::pair<uint32_t, uint32_t> CaseValueAndIndexPair;

private:

    bool isDuplicateCaseValue(uint32_t caseValue, std::string &originalCaseValueLineNumber) const;
    bool compileLabeledStatements(const TreeSequence &statements);
    void getSignedMinAndMaxCaseValues(uint32_t &minValue, uint32_t &maxValue) const;
    void getUnsignedMinAndMaxCaseValues(uint32_t &minValue, uint32_t &maxValue) const;

    static bool signedCaseValueComparator(const CaseValueAndIndexPair &a, const CaseValueAndIndexPair &b);
    static bool unsignedCaseValueComparator(const CaseValueAndIndexPair &a, const CaseValueAndIndexPair &b);
    size_t computeJumpModeCost(JumpMode jumpMode, const std::vector<CaseValueAndIndexPair> &caseValues) const;

    // Forbidden:
    SwitchStmt(const SwitchStmt &);
    SwitchStmt &operator = (const SwitchStmt &);

public:

    Tree *expression;  // may be signed or unsigned; owns the pointed object
    Tree *statement;   // owns the pointed object
    SwitchCaseList cases;  // includes the 'default' clause, if any

    static bool isJumpModeForced;
    static JumpMode forcedJumpMode;

};


#endif  /* _H_SwitchStmt */
