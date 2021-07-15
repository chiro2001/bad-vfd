/*
 * test_chiui.cpp
 *
 *  Created on: 2021年7月11日
 *      Author: Chiro
 */

#include "test_chiui.h"

#include <cstdio>
// #include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "DIALOG.h"
#include "GUI.h"
#include "WM.h"
#include "chi-dom.hpp"

using namespace chidom;

int id_cnt = GUI_ID_USER + 0x20;
const std::set<std::string> components = {"window", "frame", "button"};
const std::unordered_map<std::string, std::pair<int, int>>
    default_component_size = {{"window", {480, 320}},
                              {"frame", {480 / 3, 320 / 3}},
                              {"button", {80, 20}}};

const std::unordered_map<std::string, WM_HMEM (*)(int, int, int, int, WM_HWIN,
                                                  int, int, int)>
    creates = {{"window", (WM_HMEM(*)(int, int, int, int, WM_HWIN, int, int,
                                      int))WINDOW_CreateEx},
               {"frame", (WM_HMEM(*)(int, int, int, int, WM_HWIN, int, int,
                                     int))FRAMEWIN_CreateEx},
               {"button", BUTTON_CreateEx}};

void blanks(int width) {
  while (width-- > 0) printf("  ");
}

static void message_handler(VNode *target, WM_MESSAGE *msg) {
  switch (msg->MsgId) {
    case WM_PAINT:
      printf("message_handler:WM_PAINT\r\n");
      break;
    case WM_NOTIFICATION_CLICKED:
      printf("message_handler:WM_NOTIFICATION_CLICKED\r\n");
      break;
    default:
      break;
  }
}

std::pair<int, int> get_center(std::pair<int, int> start,
                               std::pair<int, int> size) {
  return {start.first + size.first / 2, start.second + size.second / 2};
}

template <typename T1, typename T2>
bool in_map(std::unordered_map<T1, T2> m, T1 val) {
  return (m.find(val) != m.end());
}
bool in_map_string(std::unordered_map<std::string, std::string> m,
                   std::string val) {
  return in_map<std::string, std::string>(m, val);
};

WM_HWIN render_dom(VNode *vnode, int depth = 0) {
  if (vnode == nullptr) {
    // *info = nullptr;
    return 0;
  }
  // 必须用window或者frame做root
  if (!(vnode->sel == "window" || vnode->sel == "frame")) {
    // *info = nullptr;
    return 0;
  }
  WM_HWIN root;
  printf("create(%d, %d)\r\n",
         default_component_size.find(vnode->sel)->second.first,
         default_component_size.find(vnode->sel)->second.second);
  std::pair<int, int> pos_now = {0, 0};
  root = WM_CreateWindow(
      0, 0, default_component_size.find(vnode->sel)->second.first,
      default_component_size.find(vnode->sel)->second.second, WM_CF_SHOW,
      [](WM_MESSAGE *msg) { message_handler(nullptr, msg); }, 0);
  for (const auto child : vnode->children) {
    if (child == nullptr || components.find(child->sel) == components.end())
      continue;
    std::pair<int, int> pos_child = {0, 0};
    std::pair<int, int> size_child = {
        in_map<std::string, std::string>(child->data.attrs, "w")
            ? atoi(child->data.attrs["w"].c_str())
            : default_component_size.find(child->sel)->second.first,
        in_map<std::string, std::string>(child->data.attrs, "h")
            ? atoi(child->data.attrs["h"].c_str())
            : default_component_size.find(child->sel)->second.second};
    printf("creating %s: (%d, %d, %d, %d, ..., %d)\r\n", child->sel.c_str(),
           pos_now.first, pos_now.second, size_child.first, size_child.second,
           id_cnt);
    auto handle = (*creates.find(child->sel)->second)(
        pos_now.first, pos_now.second, size_child.first, size_child.second,
        root, WM_CF_SHOW, 0, (id_cnt++));
    pos_now.second += size_child.second;
    if (child->sel == "button" && !child->children.empty()) {
      BUTTON_SetText(handle, child->children.at(0)->sel.c_str());
    }
  }
}

void scan_dom(VNode *vnode, int depth = 0) {
  if (vnode == nullptr) return;
  blanks(depth);
  if (!vnode->children.empty()) {
    printf("<%s>\n", vnode->sel.c_str());
    if (!vnode->data.callbacks.empty()) {
      for (const auto &f : vnode->data.callbacks) {
        blanks(depth + 1);
        printf("callback: %s\n", f.first.c_str());
        auto event = Event(vnode, f.first);
        f.second(event);
      }
    }
    for (const auto child : vnode->children) {
      if (child == nullptr) continue;
      scan_dom(child, depth + 1);
    }
    blanks(depth);
    printf("</%s>\n", vnode->sel.c_str());
  } else {
    printf("[%s]\n", vnode->sel.c_str());
  }
}

VNode *app() {
  static const State<std::string> value = useState<std::string>("Value TEXT");
  VNode *root =
      (GCCX /*
<window name="test chiui" x="20" y="10" w="320" h="240">
  <button onclick={[](const Event &e) -> void {
    printf("OnClick\r\n");
  }}>
    Button Text
  </button>
  <button w="160">
    Long Button!
  </button>
</window>
*/);
  return root;
}

WM_HWIN test_chiui() {
  WM_SetDesktopColor(GUI_BLACK);
  VNode *dom = app();
  WM_HWIN root = render_dom(dom);
  // scan_dom(dom);
  delete dom;
  return root;
}
