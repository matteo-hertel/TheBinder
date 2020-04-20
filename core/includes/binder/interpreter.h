#pragma once
#include "binder/memory/sparseMemoryPool.h"

namespace binder {

namespace autogen {
class Expr;
}

// used to keep track of the type of our runtime
enum class RuntimeValueType { NUMBER = 0, STRING, NIL, BOOLEAN, INVALID };

class BinderContext;

// this is our generic runtime value and can be any
// of the supported types.
// not a huge fan of crazy modern bananas c++ but I guess this might
// be a chance for std::any to shine?
struct RuntimeValue {
  union {
    double number;
    const char *string;
    bool boolean;
  };
  RuntimeValueType type = RuntimeValueType::INVALID;

  const char *toString(BinderContext *context);
};

// NOTE possibly have an abstract class at the base as
// interface to allow to bolt different interpreters if needed
class ASTInterpreter {
  typedef uint32_t RuntimeValueHandle;

public:
  // poolSize is in number of elements stored in the pool;
  ASTInterpreter(BinderContext *context, int poolSize = 1000)
      : m_context(context), m_pool(poolSize){};
  ~ASTInterpreter() = default;

  RuntimeValue *interpret(autogen::Expr *ASTRoot);
  void flushMemory() { m_pool.clear(); };

private:
  BinderContext *m_context;
  memory::SparseMemoryPool<RuntimeValue> m_pool;
};

} // namespace binder
