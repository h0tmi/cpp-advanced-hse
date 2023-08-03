#pragma once

#include <llvm/Support/raw_ostream.h>

enum class Entity {
    kVariable,
    kField,
    kType,
    kConst,
    kFunction
};

std::string Str(Entity entity) {
    switch (entity) {
    case Entity::kVariable: return "variable";
    case Entity::kField: return "field";
    case Entity::kType: return "type";
    case Entity::kConst: return "const";
    case Entity::kFunction: return "function";
    }
}

std::string GetFilename(const std::string& path) {
    size_t i = path.size() - 1;
    while (i < path.size() && path[i] != '/') {
        --i;
    }
    ++i;
    return path.substr(i);
}

void PrintStatistics(
    const std::string& filename,
    int bad_names,
    int mistakes,
    llvm::raw_ostream& os = llvm::outs())
{
    os << "===== Processing " << GetFilename(filename) << " =====\n";
    os << "Bad names found: " << bad_names << "\n";
    os << "Possible mistakes: " << mistakes << "\n\n";
}

void BadName(
    Entity entity,
    const std::string& name, const std::string& filename,
    int line, llvm::raw_ostream& os = llvm::outs())
{
    os << "Entity's name \"" << name << "\" does not meet the requirements (" << Str(entity) << ")\n";
    os << "In " << GetFilename(filename) << " at line " << line << "\n\n";
}

void Mistake(
    const std::string& name,
    const std::string& wrong_word,
    const std::string& ok_word,
    const std::string& filename,
    int line, llvm::raw_ostream& os = llvm::outs())
{
    os << "Probably mistake in variable's \"" << name << "\" name.\n";
    os << "Consider using \"" << ok_word << "\" instead of \"" << wrong_word << "\"\n";
    os << "In " << GetFilename(filename) << " at line " << line << "\n\n";
}
