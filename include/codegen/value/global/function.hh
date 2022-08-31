#ifndef VIPER_CODEGEN_FUNCTION_HH
#define VIPER_CODEGEN_FUNCTION_HH
#include <codegen/value/global/global.hh>
#include <codegen/value/basicBlock.hh>

namespace Codegen
{
    class AllocaInst;
    class Function : public Global
    {
    public:
        static Function* Create(const std::string& name, bool isDecl, Module& module);

        std::vector<BasicBlock*>& GetBasicBlockList();
        std::vector<AllocaInst*>& GetArgList();
        std::string GetName() const;
        Type* GetReturnType() const;
        bool IsDecl() const;
        void AddAlloca(AllocaInst* alloca);
    protected:
        Function(const std::string& name, bool isDecl, Module& module);
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    private:
        void SortAllocas();
        std::string _name;
        bool _isDecl;
        std::vector<AllocaInst*> _args;
        std::vector<BasicBlock*> _basicBlockList;
        Type* _returnType;
    };
}

#endif