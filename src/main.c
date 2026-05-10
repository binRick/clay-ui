#define CLAY_IMPLEMENTATION
#include "clay.h"

#include "raylib.h"
#include "raymath.h"
#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../vendor/clay_renderer_raylib.c"

static void HandleClayError(Clay_ErrorData errorData) {
    printf("clay error: %.*s\n", errorData.errorText.length, errorData.errorText.chars);
}

static SceneId NavScene(AppState *state) {
    if (IsKeyPressed(KEY_ONE))   return SCENE_MAIN_MENU;
    if (IsKeyPressed(KEY_TWO))   return SCENE_CHARACTER;
    if (IsKeyPressed(KEY_THREE)) return SCENE_INVENTORY;
    if (IsKeyPressed(KEY_FOUR))  return SCENE_SETTINGS;
    if (IsKeyPressed(KEY_FIVE))  return SCENE_HUD;
    return state->scene;
}

static void Layout(AppState *state) {
    Clay_BeginLayout();

    CLAY(CLAY_ID("Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
        .backgroundColor = COL_BG_DEEP,
    }) {
        Sidebar(state);

        CLAY(CLAY_ID("ContentColumn"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
            CLAY(CLAY_ID("SceneArea"), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .clip = { .horizontal = true, .vertical = true },
            }) {
                switch (state->scene) {
                    case SCENE_MAIN_MENU: Scene_MainMenu(state);  break;
                    case SCENE_CHARACTER: Scene_Character(state); break;
                    case SCENE_INVENTORY: Scene_Inventory(state); break;
                    case SCENE_SETTINGS:  Scene_Settings(state);  break;
                    case SCENE_HUD:       Scene_HUD(state);       break;
                    default: break;
                }
            }
            Footer();
        }
    }
}

int main(void) {
    AppState state = {
        .scene = SCENE_MAIN_MENU,
        .selectedCharacter = 0,
        .selectedInventoryItem = 0,
        .hoveredInventoryItem = -1,
        .settingsTab = SETTINGS_TAB_GRAPHICS,
        .musicVolume = 0.6f,
        .sfxVolume = 0.8f,
        .masterVolume = 0.9f,
        .vsync = true,
        .fullscreen = false,
        .subtitles = true,
        .screenShake = true,
        .qualityPreset = 2,
        .playerHp = 0.72f,
        .playerMp = 0.45f,
    };

    uint32_t clayMemSize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayMemSize, malloc(clayMemSize));

    Clay_Raylib_Initialize(1280, 800, "Clay Game Menus",
        FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    SetWindowMinSize(1024, 680);

    Clay_Initialize(clayMemory,
        (Clay_Dimensions){ (float)GetScreenWidth(), (float)GetScreenHeight() },
        (Clay_ErrorHandler){ HandleClayError, NULL });

    Font fonts[1];
    fonts[FONT_BODY] = GetFontDefault();

    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    while (!WindowShouldClose()) {
        UI_BeginFrame();
        state.time += GetFrameTime();
        state.scene = NavScene(&state);

        Clay_SetLayoutDimensions((Clay_Dimensions){ (float)GetScreenWidth(), (float)GetScreenHeight() });

        Vector2 mouse = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2){ mouse.x, mouse.y }, IsMouseButtonDown(MOUSE_BUTTON_LEFT));

        Vector2 wheel = GetMouseWheelMoveV();
        Clay_UpdateScrollContainers(true, (Clay_Vector2){ wheel.x * 12.0f, wheel.y * 12.0f }, GetFrameTime());

        Layout(&state);
        Clay_RenderCommandArray cmds = Clay_EndLayout(GetFrameTime());

        BeginDrawing();
            ClearBackground((Color){ 12, 12, 18, 255 });
            Clay_Raylib_Render(cmds, fonts);
        EndDrawing();
    }

    Clay_Raylib_Close();
    free(clayMemory.memory);
    return 0;
}
