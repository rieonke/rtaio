//
// Created by Rieon Ke on 2019-06-28.
//

#ifndef DDL_GEN_GUI_UTILS_H
#define DDL_GEN_GUI_UTILS_H

#include <imgui.h>

namespace dh {
    namespace gui {
        class gui_utils {
        public:
            static void help_mark(const char *desc) {
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted(desc);
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }
        };

    }

}

#endif //DDL_GEN_GUI_UTILS_H
