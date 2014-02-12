#ifndef __EXPRESSIONS_HH__
#define __EXPRESSIONS_HH__

#include <sstream>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include "environment.hh"

using namespace std;

// expression interface provide a method evaluate to be inherited
class Expression {
    public:
        virtual double evaluate(const Environment &env) const = 0;
        virtual ~Expression() {}
};

// simple class used to store value
// it doesn't provide a default construct 
// must construct with a value
class Value : public Expression {
    private:
        double value;
    public:
        Value(double v): value(v) {}
        ~Value() {}

        double evaluate(const Environment &env) const { return value;}
};

// Symbol class used to store symbol name
// evaluate() return symbol's value
class Symbol : public Expression {
    private:
        string sym;
    public:
        Symbol(string s): sym(s) {}
        ~Symbol() {}

        string getSymbol() const
        {
            return sym;
        }

        double evaluate(const Environment &env) const 
        {
            return env.getSymbolValue(sym);
        }
};

// BinaryOperator class is to be inherited by specific operator 
class BinaryOperator : public Expression {
    private:
        Expression *pLHS_;
        Expression *pRHS_;

    public:
        BinaryOperator(Expression *pLHS, Expression *pRHS): pLHS_(pLHS), pRHS_(pRHS) 
        {
            assert(pLHS_);
            assert(pRHS_);
        }
        ~BinaryOperator() 
        {
            delete pLHS_;
            delete pRHS_;
        }

        const Expression* pLHS() const {return pLHS_;}
        const Expression* pRHS() const {return pRHS_;}
};

// AddOper class provides add operation
class AddOper : public BinaryOperator {
    public:
        AddOper(Expression *pLHS, Expression *pRHS): BinaryOperator(pLHS, pRHS) {}
        double evaluate(const Environment &env) const 
        {
            return pLHS()->evaluate(env) + pRHS()->evaluate(env);
        }
};

// SubOper class provides sub operation
class SubOper : public BinaryOperator {
    public:
        SubOper(Expression *pLHS, Expression *pRHS): BinaryOperator(pLHS, pRHS) {}

        double evaluate(const Environment &env) const 
        {
            return pLHS()->evaluate(env) - pRHS()->evaluate(env);
        }
};

// MulOper class provides multiply operation
class MulOper : public BinaryOperator {
    public:
        MulOper(Expression *pLHS, Expression *pRHS): BinaryOperator(pLHS, pRHS) {}

        double evaluate(const Environment &env) const
        {
            return pLHS()->evaluate(env) * pRHS()->evaluate(env);
        }
};

// DivOper class provides divide operation
class DivOper : public BinaryOperator {
    public:
        DivOper(Expression *pLHS, Expression *pRHS): BinaryOperator(pLHS, pRHS) {}

        double evaluate(const Environment &env) const 
        {
            double lhs = pLHS()->evaluate(env);
            double rhs = pRHS()->evaluate(env);
            if (rhs == 0.0) {
                ostringstream err_msg;
                err_msg << "divide by zero: " << lhs << "/" << rhs;
                throw runtime_error(err_msg.str());
            }
            return lhs / rhs;
        }
};

// power class 
class PowOper : public BinaryOperator {
    public:
        PowOper(Expression *pLHS, Expression *pRHS): BinaryOperator(pLHS, pRHS) {}
        double evaluate(const Environment &env) const
        {
            double lhs = pLHS()->evaluate(env);
            double rhs = pRHS()->evaluate(env);
            return pow(lhs, rhs);
        }
};

// general UnaryOperator Class
class UnaryOperator : public Expression {
    private:
        Expression *pExp_;
    public:
        UnaryOperator(Expression *p): pExp_(p) {}
        ~UnaryOperator() {delete pExp_;}

        const Expression* pExp() const {return pExp_;};
};

// negative operation
class NegOper : public UnaryOperator {
    public:
        NegOper(Expression *p): UnaryOperator(p) {}
        double evaluate(const Environment &env) const
        {
            return 0.0 - pExp()->evaluate(env);
        }
};

#endif
