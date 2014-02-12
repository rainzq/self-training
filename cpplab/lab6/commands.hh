#ifndef __COMMANDS_HH__
#define __CONMANDS_HH__

#include <iostream>
#include "environment.hh"
#include "expressions.hh"

using namespace std;

// abstract Command provide general run method
class Command {
    public:
        virtual void run(Environment &env) = 0;
        virtual ~Command() {}
};

class PrintCommand : public Command {
    private:
        Expression *pExp_;
    public:
        PrintCommand(Expression *pExp): pExp_(pExp) {}
        ~PrintCommand() {delete pExp_;}

        void run(Environment &env)
        {
            double result = pExp_->evaluate(env);
            cout << " = " << result << endl;
        }
};

class AssignCommand : public Command {
    private:
        Symbol *pSym_;
        Expression *pExp_;
    public:
        AssignCommand(Symbol *pSym, Expression *pExp): pSym_(pSym), pExp_(pExp) {}
        ~AssignCommand() 
        {
            delete pSym_;
            delete pExp_;
        }

        void run(Environment &env)
        {
            double result = pExp_->evaluate(env);
            string sym = pSym_->getSymbol();
            env.setSymbolValue(sym, result);
            cout << sym << " = " << result << endl;
        }
};
#endif
