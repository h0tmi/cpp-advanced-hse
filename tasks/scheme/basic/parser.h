#pragma once

#include <memory>
#include <variant>

#include "error.h"
#include "object.h"
#include "tokenizer.h"

std::shared_ptr<Object> Read(Tokenizer*);
std::shared_ptr<Object> ReadList(Tokenizer*);
std::shared_ptr<Object> ReadQuote(Tokenizer*);
std::shared_ptr<Object> ReadListQuote(Tokenizer*);
