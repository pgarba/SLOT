#include "LLVMFunction.h"
#include "SLOTExceptions.h"
#include <regex>

#ifndef LLMAPPING
#define LLMAPPING std::map<std::string, Value*>
#endif

namespace SLOT
{

    LLVMFunction::LLVMFunction(bool t_shiftToMultiply, context& t_scx, Function* t_contents) : shiftToMultiply(t_shiftToMultiply), scx(t_scx), contents(t_contents)
    {
        scx.set_rounding_mode(RNE);
        for (Argument* arg = contents->arg_begin(); arg < contents->arg_end(); arg++)
        {
            if (arg->getType()->isIntegerTy())
            {
                //1-wide integer --> boolean
                if (arg->getType()->getIntegerBitWidth() == 1)
                {
                    variables.insert(make_pair(arg->getName().str(), scx.bool_const(arg->getName().str().c_str())));
                }
                else
                {
                    //Regular bitvector case
                    variables.insert(make_pair(arg->getName().str(), scx.bv_const(arg->getName().str().c_str(),arg->getType()->getIntegerBitWidth())));
                }
            }
            else if (arg->getType()->isHalfTy())
            {
                variables.insert(make_pair(arg->getName().str(), scx.fpa_const(arg->getName().str().c_str(), 5, 11)));
            }
            else if (arg->getType()->isFloatTy())
            {
                variables.insert(make_pair(arg->getName().str(), scx.fpa_const(arg->getName().str().c_str(), 8, 24)));
            }
            else if (arg->getType()->isDoubleTy())
            {
                variables.insert(make_pair(arg->getName().str(), scx.fpa_const(arg->getName().str().c_str(), 11, 53)));
            }
            else if (arg->getType()->isFP128Ty())
            {
                variables.insert(make_pair(arg->getName().str(), scx.fpa_const(arg->getName().str().c_str(), 15, 113)));
            }
            else
            {
                std::string type_str;
                llvm::raw_string_ostream rso(type_str);
                arg->print(rso);
                throw UnsupportedTypeException("unsupported LLVM variable type", rso.str());
            }
        }
    }

    expr LLVMFunction::ToSMT()
    {
        return LLVMNode::MakeLLVMNode(shiftToMultiply, scx, variables, ((ReturnInst *)contents->getEntryBlock().getTerminator())->getOperand(0))->ToSMT();
    }
}