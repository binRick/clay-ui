#include "ui.h"
#include "clay.h"
#include "raylib.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#define UI_STR_ARENA_BYTES 16384
static char  ui_str_arena[UI_STR_ARENA_BYTES];
static int   ui_str_arena_off = 0;

void UI_BeginFrame(void) {
    ui_str_arena_off = 0;
}

Clay_String UI_Fmt(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int remaining = UI_STR_ARENA_BYTES - ui_str_arena_off;
    if (remaining < 2) {
        va_end(ap);
        return (Clay_String){ .length = 0, .chars = "" };
    }
    int n = vsnprintf(ui_str_arena + ui_str_arena_off, remaining, fmt, ap);
    va_end(ap);
    if (n < 0) n = 0;
    if (n >= remaining) n = remaining - 1;
    Clay_String s = {
        .isStaticallyAllocated = false,
        .length = n,
        .chars = ui_str_arena + ui_str_arena_off,
    };
    ui_str_arena_off += n + 1;
    return s;
}

const Clay_Color COL_BG_DEEP     = { 14,  16,  26,  255 };
const Clay_Color COL_BG          = { 22,  24,  36,  255 };
const Clay_Color COL_PANEL       = { 30,  34,  50,  235 };
const Clay_Color COL_PANEL_HOVER = { 46,  52,  74,  255 };
const Clay_Color COL_BORDER      = { 70,  78, 110,  255 };
const Clay_Color COL_ACCENT      = { 255, 184,  72, 255 };
const Clay_Color COL_ACCENT_DIM  = { 178, 122,  44, 255 };
const Clay_Color COL_DANGER      = { 220,  72,  72, 255 };
const Clay_Color COL_GOLD        = { 232, 196, 110, 255 };
const Clay_Color COL_TEXT        = { 232, 232, 240, 255 };
const Clay_Color COL_TEXT_DIM    = { 168, 172, 196, 255 };
const Clay_Color COL_TEXT_FAINT  = { 110, 116, 140, 255 };
const Clay_Color COL_HP          = { 196,  62,  68, 255 };
const Clay_Color COL_MP          = {  72, 130, 220, 255 };
const Clay_Color COL_XP          = { 156, 220,  92, 255 };
const Clay_Color COL_TRANSPARENT = {   0,   0,   0,   0 };

static const Clay_String SCENE_KEYS[SCENE_COUNT] = {
    CLAY_STRING_CONST("1"),
    CLAY_STRING_CONST("2"),
    CLAY_STRING_CONST("3"),
    CLAY_STRING_CONST("4"),
    CLAY_STRING_CONST("5"),
};

static const Clay_String SCENE_LABELS[SCENE_COUNT] = {
    CLAY_STRING_CONST("MAIN MENU"),
    CLAY_STRING_CONST("CHARACTER"),
    CLAY_STRING_CONST("INVENTORY"),
    CLAY_STRING_CONST("SETTINGS"),
    CLAY_STRING_CONST("HUD / PAUSE"),
};

void Sidebar(AppState *state) {
    CLAY(CLAY_ID("Sidebar"), {
        .layout = {
            .sizing = { CLAY_SIZING_FIXED(220), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .padding = { 18, 18, 24, 24 },
            .childGap = 6,
        },
        .backgroundColor = COL_BG,
        .border = { .color = COL_BORDER, .width = { .right = 1 } },
    }) {
        CLAY(CLAY_ID("BrandRow"), {
            .layout = { .padding = { 4, 4, 0, 12 } },
        }) {
            CLAY_TEXT(CLAY_STRING("CLAY  DEMOS"),
                CLAY_TEXT_CONFIG({ .fontId = FONT_BODY, .fontSize = 22, .letterSpacing = 2, .textColor = COL_ACCENT }));
        }
        CLAY(CLAY_ID("BrandSub"), {
            .layout = { .padding = { 4, 4, 0, 18 } },
        }) {
            CLAY_TEXT(CLAY_STRING("game menus"),
                CLAY_TEXT_CONFIG({ .fontId = FONT_BODY, .fontSize = 12, .letterSpacing = 1, .textColor = COL_TEXT_FAINT }));
        }

        for (int i = 0; i < SCENE_COUNT; i++) {
            bool active = (state->scene == (SceneId)i);
            Clay_ElementId rowId = CLAY_IDI("SidebarRow", i);
            CLAY(rowId, {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(36) },
                    .padding = { 12, 8, 0, 0 },
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .childGap = 10,
                },
                .backgroundColor = active ? COL_PANEL_HOVER : (Clay_Hovered() ? COL_PANEL : COL_TRANSPARENT),
                .cornerRadius = CLAY_CORNER_RADIUS(6),
                .border = active
                    ? (Clay_BorderElementConfig){ .color = COL_ACCENT, .width = { .left = 3 } }
                    : (Clay_BorderElementConfig){ 0 },
            }) {
                CLAY(CLAY_IDI("SidebarKey", i), {
                    .layout = {
                        .sizing = { CLAY_SIZING_FIXED(20), CLAY_SIZING_FIXED(20) },
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                    },
                    .backgroundColor = active ? COL_ACCENT : COL_BG_DEEP,
                    .cornerRadius = CLAY_CORNER_RADIUS(4),
                    .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                }) {
                    CLAY_TEXT(SCENE_KEYS[i],
                        CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = active ? COL_BG_DEEP : COL_TEXT_DIM }));
                }
                CLAY_TEXT(SCENE_LABELS[i],
                    CLAY_TEXT_CONFIG({
                        .fontSize = 14,
                        .letterSpacing = 1,
                        .textColor = active ? COL_TEXT : COL_TEXT_DIM,
                    }));
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < SCENE_COUNT; i++) {
                if (Clay_PointerOver(CLAY_IDI("SidebarRow", i))) {
                    state->scene = (SceneId)i;
                    break;
                }
            }
        }

        CLAY(CLAY_ID("SidebarSpacer"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } },
        }) {}

        CLAY(CLAY_ID("SidebarFootBlock"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .padding = { 6, 6, 12, 12 },
                .childGap = 4,
            },
            .backgroundColor = COL_BG_DEEP,
            .cornerRadius = CLAY_CORNER_RADIUS(6),
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            CLAY_TEXT(CLAY_STRING("v0.1.0  build 042"),
                CLAY_TEXT_CONFIG({ .fontSize = 11, .textColor = COL_TEXT_FAINT }));
            CLAY_TEXT(CLAY_STRING("layout: clay 0.14"),
                CLAY_TEXT_CONFIG({ .fontSize = 11, .textColor = COL_TEXT_FAINT }));
            CLAY_TEXT(CLAY_STRING("render: raylib 5.5"),
                CLAY_TEXT_CONFIG({ .fontSize = 11, .textColor = COL_TEXT_FAINT }));
        }
    }
}

void Footer(void) {
    CLAY(CLAY_ID("Footer"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(32) },
            .padding = { 18, 18, 0, 0 },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 18,
        },
        .backgroundColor = COL_BG,
        .border = { .color = COL_BORDER, .width = { .top = 1 } },
    }) {
        CLAY_TEXT(CLAY_STRING("[1-5] switch scene"),
            CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = COL_TEXT_FAINT, .letterSpacing = 1 }));
        CLAY_TEXT(CLAY_STRING("[scroll] inventory list"),
            CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = COL_TEXT_FAINT, .letterSpacing = 1 }));
        CLAY_TEXT(CLAY_STRING("[click] interact"),
            CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = COL_TEXT_FAINT, .letterSpacing = 1 }));
        CLAY(CLAY_ID("FooterSpacer"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } },
        }) {}
        CLAY_TEXT(CLAY_STRING("CLAY x RAYLIB"),
            CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = COL_ACCENT_DIM, .letterSpacing = 3 }));
        CLAY(CLAY_ID("FooterPad"), {
            .layout = { .sizing = { CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8) } },
        }) {}
    }
}

void StatBar(Clay_String label, float value, float maxValue, Clay_Color fillColor) {
    float pct = (maxValue <= 0.0f) ? 0.0f : value / maxValue;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;

    CLAY_AUTO_ID({
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childGap = 4,
        },
    }) {
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
        }) {
            CLAY_TEXT(label,
                CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 2, .textColor = COL_TEXT_DIM }));
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
            CLAY_TEXT(UI_Fmt("%d / %d", (int)value, (int)maxValue),
                CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = COL_TEXT }));
        }
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(10) },
            },
            .backgroundColor = COL_BG_DEEP,
            .cornerRadius = CLAY_CORNER_RADIUS(3),
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_PERCENT(pct), CLAY_SIZING_GROW(0) },
                },
                .backgroundColor = fillColor,
                .cornerRadius = CLAY_CORNER_RADIUS(3),
            }) {}
        }
    }
}
