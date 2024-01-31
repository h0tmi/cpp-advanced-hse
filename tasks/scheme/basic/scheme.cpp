#include "scheme.h"

#include <sstream>

#include "parser.h"
#include "tokenizer.h"

std::string Interpreter::Run(const std::string &s) {
    std::stringstream in(s);
    Tokenizer tokenizer(&in);
    auto ast = Read(&tokenizer);
    return ast->Evaluate()->ToString();
}
