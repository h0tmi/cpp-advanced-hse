export module hse;

import <string>;

namespace hse {
  std::string GetStringPrivate() { return "Deadline is extended"; }
  export auto GetString() { return GetStringPrivate(); }
}
