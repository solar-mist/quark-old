#ifndef VIPER_CODEGEN_BUILDER_HH
#define VIPER_CODEGEN_BUILDER_HH
#include <codegen/module.hh>
#include <codegen/value/value.hh>
#include <codegen/value/global/global.hh>
#include <codegen/value/global/function.hh>
#include <codegen/value/constant/intLiteral.hh>
#include <codegen/value/instruction/instruction.hh>
#include <codegen/value/instruction/ret.hh>
#include <codegen/value/basicBlock.hh>
#include <codegen/value/instruction/binaryOperator.hh>
#include <codegen/value/instruction/alloca.hh>
#include <codegen/value/instruction/store.hh>
#include <codegen/value/instruction/load.hh>
#include <codegen/value/instruction/call.hh>
#include <codegen/value/instruction/sext.hh>

namespace Codegen
{
    class Builder
    {
    public:
        Builder(Module& module);

        void SetInsertPoint(BasicBlock* insertPoint);

        Value* CreateIntLiteral(long long value, Type* type);

        Value* CreateRet(Value* value);


        Value* CreateAdd(Value* left, Value* right);
        Value* CreateSub(Value* left, Value* right);
        Value* CreateMul(Value* left, Value* right);

        CallInst* CreateCall(Function* callee, std::vector<Value*> args, bool isStatement);

        AllocaInst* CreateAlloca(Type* type);
        StoreInst* CreateStore(Value* value, Value* ptr, bool isStatement);
        LoadInst* CreateLoad(Value* ptr);

        SExtInst* CreateSExt(Value* value, Type* dstType);

    protected:
        Value* CreateBinOp(Value* left, Instruction::Instructions op, Value* right);
    
    private:
        Module& _module;
        BasicBlock* _insertPoint;
    };
}

#endif