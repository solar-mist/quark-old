// Copyright 2025 solar-mist

#include "symbol/ImportManager.h"

#include "diagnostic/Diagnostic.h"

#include "lexer/Lexer.h"
#include "lexer/Token.h"

#include "parser/SymbolParser.h"

#include <fstream>

ImportManager::ImportManager()
    : mSearchPaths{std::filesystem::current_path()}
{
}

std::vector<Export> ImportManager::getExports(std::string file, Scope* scope)
{
    return mExports;
}

std::vector<std::string> ImportManager::getPendingStructTypeNames()
{
    return mPendingStructTypeNames;
}

void ImportManager::clearExports()
{
    mExports.clear();
    mPendingStructTypeNames.clear();
}

void ImportManager::addPendingStructType(std::string name)
{
    mPendingStructTypeNames.push_back(std::move(name));
}

bool ImportManager::wasExportedTo(std::string root, std::vector<Export>& exports, Export& exp)
{
    std::string from = exp.exportedFrom;
    std::function<bool(std::string)> checkOne;
    checkOne = [&](std::string path) {
        if (path == root) return true;

        auto it = std::find_if(exports.begin(), exports.end(), [&](auto& exp) {
            return exp.exportedFrom == path && !exp.exportedTo.empty();
        });
        if (it != exports.end()) return checkOne(it->exportedTo);
        return false;
    };
    return checkOne(exp.exportedFrom);
}

std::vector<parser::ASTNodePtr> ImportManager::resolveImports(std::filesystem::path path, std::filesystem::path relativeTo, Scope* scope, bool exported)
{
    path += ".vpr";

    std::ifstream stream;
    stream.open(relativeTo.parent_path() / path);
    std::string foundPath;
    if (!stream.is_open())
    {
        for (auto& searchPath : mSearchPaths)
        {
            stream.open(searchPath / path);
            if (stream.is_open())
            {
                foundPath = searchPath;
                break;
            }
        }
    }
    else
    {
        foundPath = relativeTo.parent_path() / path;
    }

    mImportedFiles.push_back(foundPath);

    diagnostic::Diagnostics importerDiag;

    std::stringstream buf;
    buf << stream.rdbuf();
    std::string text = buf.str();

    importerDiag.setText(text);
    importerDiag.setImported(true);

    lexer::Lexer lexer(text, mImportedFiles.back());
    auto tokens = lexer.lex();

    parser::SymbolParser parser(tokens, importerDiag, *this, scope);
    auto ast = parser.parse();

    // Add an export if this was an export import
    if (exported)
        mExports.push_back({foundPath, nullptr, relativeTo.string()});
    
    std::function<std::vector<Export>(Scope*)> collectScope;
    collectScope = [&path, &collectScope, &foundPath](Scope* scope) {
        std::vector<Export> ret;
        for (auto& symbol : scope->symbols)
        {
            ret.push_back({foundPath, &symbol});
        }
        for (auto child : scope->children)
        {
            auto childExports = collectScope(child);
            std::copy(childExports.begin(), childExports.end(), std::back_inserter(ret));
        }
        return ret;
    };
    auto exports = collectScope(scope);
    std::copy(exports.begin(), exports.end(), std::back_inserter(mExports));
    
    return ast;
}

void ImportManager::seizeScope(ScopePtr scope)
{
    mScopes.push_back(std::move(scope));
}