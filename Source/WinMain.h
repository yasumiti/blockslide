#pragma once
inline constexpr int SCREEN_WIDTH = 1280;
inline constexpr int SCREEN_HEIGHT = 720;

enum SCENE
{
    TITLE = 0,
    SLECT,
    GAME,
    OVER,
    CLEAR,
};

void setScene(int nextScene);
