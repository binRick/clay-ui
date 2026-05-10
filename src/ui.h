#ifndef CLAY_UI_DEMO_UI_H
#define CLAY_UI_DEMO_UI_H

#include "clay.h"
#include <stdbool.h>

typedef enum {
    SCENE_MAIN_MENU = 0,
    SCENE_CHARACTER,
    SCENE_INVENTORY,
    SCENE_SETTINGS,
    SCENE_HUD,
    SCENE_COUNT
} SceneId;

typedef enum {
    SETTINGS_TAB_GRAPHICS = 0,
    SETTINGS_TAB_AUDIO,
    SETTINGS_TAB_CONTROLS,
    SETTINGS_TAB_COUNT
} SettingsTab;

typedef struct {
    SceneId scene;
    bool paused;

    int selectedCharacter;
    int selectedInventoryItem;
    int hoveredInventoryItem;
    int inventoryFilter;       // 0=ALL 1=WEAPONS 2=ARMOR 3=CONSUM 4=QUEST 5=MISC
    SettingsTab settingsTab;

    // New-hero form state
    bool newHeroFormOpen;
    char newHeroNameBuf[24];
    int  newHeroNameLen;
    int  newHeroClass;
    int  newHeroStats[4];      // STR, DEX, INT, VIT
    int  newHeroAccent;

    float musicVolume;
    float sfxVolume;
    float masterVolume;
    bool vsync;
    bool fullscreen;
    bool subtitles;
    bool screenShake;
    int qualityPreset;

    float time;
    float playerHp;
    float playerMp;
} AppState;

extern const Clay_Color COL_BG_DEEP;
extern const Clay_Color COL_BG;
extern const Clay_Color COL_PANEL;
extern const Clay_Color COL_PANEL_HOVER;
extern const Clay_Color COL_BORDER;
extern const Clay_Color COL_ACCENT;
extern const Clay_Color COL_ACCENT_DIM;
extern const Clay_Color COL_DANGER;
extern const Clay_Color COL_GOLD;
extern const Clay_Color COL_TEXT;
extern const Clay_Color COL_TEXT_DIM;
extern const Clay_Color COL_TEXT_FAINT;
extern const Clay_Color COL_HP;
extern const Clay_Color COL_MP;
extern const Clay_Color COL_XP;
extern const Clay_Color COL_TRANSPARENT;

#define FONT_BODY 0

void UI_BeginFrame(void);
Clay_String UI_Fmt(const char *fmt, ...);

void Sidebar(AppState *state);
void Footer(void);
void StatBar(Clay_String label, float value, float maxValue, Clay_Color fillColor);

void Scene_MainMenu(AppState *state);
void Scene_Character(AppState *state);
void Scene_Inventory(AppState *state);
void Scene_Settings(AppState *state);
void Scene_HUD(AppState *state);

#endif
