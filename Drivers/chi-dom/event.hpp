//
// Created by Chiro on 2021/6/15.
//

#include "VNode.hpp"
#include "event.h"

namespace chidom {

class Event {
 public:
  VNode *target{};
  std::string type{};
  explicit Event(VNode *target_, std::string type_)
      : target(target_), type(type_) {}
};

}  // namespace chidom