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
    // While a modal form is open, swallow scene-switch keys so they
    // can be typed into text inputs.
    if (state->newHeroFormOpen) return state->scene;
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
    // Environment-driven non-interactive screenshot mode.
    // CLAY_SCREENSHOT=path     -> save a PNG after a few frames and exit
    // CLAY_SCENE=0..4          -> start in this scene
    // CLAY_FORM=1              -> open the new-hero form on start
    const char *screenshotPath = getenv("CLAY_SCREENSHOT");
    const char *sceneEnv       = getenv("CLAY_SCENE");
    const char *formEnv        = getenv("CLAY_FORM");
    int  initialScene  = (sceneEnv) ? atoi(sceneEnv) : SCENE_MAIN_MENU;
    bool initialForm   = (formEnv && formEnv[0] == '1');
    if (initialScene < 0 || initialScene >= SCENE_COUNT) initialScene = SCENE_MAIN_MENU;

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
        .newHeroFormOpen = false,
        .newHeroClass = 0,
        .newHeroAccent = 0,
        .newHeroStats = { 10, 10, 10, 10 },
    };
    state.scene = (SceneId)initialScene;
    if (initialForm) {
        state.newHeroFormOpen = true;
        const char *demoName = "Aren of the Long Road";
        int len = 0;
        while (demoName[len] && len < (int)sizeof(state.newHeroNameBuf) - 1) {
            state.newHeroNameBuf[len] = demoName[len];
            len++;
        }
        state.newHeroNameBuf[len] = '\0';
        state.newHeroNameLen = len;
        state.newHeroClass   = 1;
        state.newHeroAccent  = 2;
        state.newHeroStats[0] = 13;
        state.newHeroStats[1] = 11;
        state.newHeroStats[2] = 14;
        state.newHeroStats[3] = 12;
    }

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

    int frame = 0;
    bool shotTaken = false;
    while (!WindowShouldClose()) {
        frame++;
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

        if (screenshotPath && !shotTaken && frame == 60) {
            TakeScreenshot(screenshotPath);
            shotTaken = true;
        }
        if (screenshotPath && shotTaken && frame == 62) {
            break;
        }
    }

    Clay_Raylib_Close();
    free(clayMemory.memory);
    return 0;
}
