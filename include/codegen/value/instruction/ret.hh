#ifndef VIPER_CODEGEN_RET_HH
#define VIPER_CODEGEN_RET_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class RetInst : public Instruction
    {
    friend class Builder;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        RetInst(Value* value, const Module& module);
    private:
        Value* _value;
    };
}

#endif