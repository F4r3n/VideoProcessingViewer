#pragma once

#include <string>

#include "imgui.h"

namespace sf {
  class Texture;
}

struct View {
    std::string ID;

    float zoom;
    ImVec2 center;
    bool shouldRescale;

    View();

    void resetZoom();
    void changeZoom(float zoom);
    void setOptimalZoom(ImVec2 winSize, ImVec2 texSize, float zoomfactor);

    ImVec2 image2window(const ImVec2& im, const ImVec2& imSize, const ImVec2& winSize, float zoomfactor) const;
    ImVec2 window2image(const ImVec2& win, const ImVec2& imSize, const ImVec2& winSize, float zoomfactor) const;

    void displaySettings();
};

