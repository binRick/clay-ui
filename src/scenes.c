#include "ui.h"
#include "clay.h"

#include "raylib.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

// =====================================================================
// Scene 1: Main Menu
// =====================================================================

static const Clay_String MAIN_MENU_ITEMS[] = {
    CLAY_STRING_CONST("NEW GAME"),
    CLAY_STRING_CONST("CONTINUE"),
    CLAY_STRING_CONST("CHARACTERS"),
    CLAY_STRING_CONST("SETTINGS"),
    CLAY_STRING_CONST("QUIT"),
};
static const Clay_String MAIN_MENU_SUBS[] = {
    CLAY_STRING_CONST("begin a fresh save"),
    CLAY_STRING_CONST("last save:  ch.4  17h 22m"),
    CLAY_STRING_CONST("3 saved heroes"),
    CLAY_STRING_CONST("graphics  audio  controls"),
    CLAY_STRING_CONST("return to desktop"),
};
#define MAIN_MENU_COUNT ((int)(sizeof(MAIN_MENU_ITEMS)/sizeof(MAIN_MENU_ITEMS[0])))

void Scene_MainMenu(AppState *state) {
    CLAY(CLAY_ID("MM_Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
        },
        .backgroundColor = COL_BG_DEEP,
    }) {
        // Left half: brand / hero text
        CLAY(CLAY_ID("MM_Left"), {
            .layout = {
                .sizing = { CLAY_SIZING_PERCENT(0.55f), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .padding = { 80, 40, 90, 40 },
                .childGap = 18,
                .childAlignment = { .y = CLAY_ALIGN_Y_TOP },
            },
        }) {
            CLAY_TEXT(CLAY_STRING("AETHERFALL"),
                CLAY_TEXT_CONFIG({ .fontSize = 84, .letterSpacing = 8, .textColor = COL_ACCENT }));
            CLAY_TEXT(CLAY_STRING("a forgotten realms revival"),
                CLAY_TEXT_CONFIG({ .fontSize = 18, .letterSpacing = 4, .textColor = COL_TEXT_DIM }));

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(24) } } }) {}

            // Decorative accent bar
            CLAY_AUTO_ID({
                .layout = { .sizing = { CLAY_SIZING_FIXED(72), CLAY_SIZING_FIXED(4) } },
                .backgroundColor = COL_ACCENT,
            }) {}

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(8) } } }) {}

            CLAY(CLAY_ID("MM_LeftBlurb"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIT(.max = 460), CLAY_SIZING_FIT(0) },
                },
            }) {
                CLAY_TEXT(CLAY_STRING(
                    "Three centuries after the Sundering, the old roads are open again. "
                    "Few who walk them return. Fewer still remember why."),
                    CLAY_TEXT_CONFIG({ .fontSize = 14, .lineHeight = 22, .textColor = COL_TEXT_DIM }));
            }

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

            // News strip at bottom of left column
            CLAY(CLAY_ID("MM_News"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIT(.max = 460), CLAY_SIZING_FIT(0) },
                    .padding = { 14, 14, 12, 12 },
                    .childGap = 6,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
                .backgroundColor = COL_PANEL,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .border = { .color = COL_BORDER, .width = { .left = 3 } },
            }) {
                CLAY_TEXT(CLAY_STRING("PATCH 0.4 - HOLLOW PEAKS"),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 3, .textColor = COL_ACCENT }));
                CLAY_TEXT(CLAY_STRING("New zone, balance pass for ranger, fixed the cave geometry bug."),
                    CLAY_TEXT_CONFIG({ .fontSize = 12, .lineHeight = 18, .textColor = COL_TEXT_DIM }));
            }
        }

        // Right half: menu items
        CLAY(CLAY_ID("MM_Right"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .padding = { 40, 80, 100, 40 },
                .childGap = 8,
                .childAlignment = { .y = CLAY_ALIGN_Y_TOP },
            },
        }) {
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(24) } } }) {}

            for (int i = 0; i < MAIN_MENU_COUNT; i++) {
                Clay_ElementId rowId = CLAY_IDI("MM_Item", i);
                CLAY(rowId, {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(64) },
                        .padding = { 24, 24, 0, 0 },
                        .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                        .childGap = 18,
                    },
                    .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
                    .cornerRadius = CLAY_CORNER_RADIUS(2),
                    .border = {
                        .color = Clay_Hovered() ? COL_ACCENT : COL_BORDER,
                        .width = { .left = Clay_Hovered() ? 6 : 2 },
                    },
                }) {
                    CLAY(CLAY_IDI("MM_ItemNum", i), {
                        .layout = {
                            .sizing = { CLAY_SIZING_FIXED(36), CLAY_SIZING_FIXED(36) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = Clay_Hovered() ? COL_ACCENT : COL_BG_DEEP,
                        .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                    }) {
                        CLAY_TEXT(UI_Fmt("0%d", i + 1),
                            CLAY_TEXT_CONFIG({
                                .fontSize = 14,
                                .letterSpacing = 1,
                                .textColor = Clay_Hovered() ? COL_BG_DEEP : COL_TEXT_DIM,
                            }));
                    }

                    CLAY(CLAY_IDI("MM_ItemTextCol", i), {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .childGap = 2,
                        },
                    }) {
                        CLAY_TEXT(MAIN_MENU_ITEMS[i],
                            CLAY_TEXT_CONFIG({
                                .fontSize = 22,
                                .letterSpacing = 4,
                                .textColor = COL_TEXT,
                            }));
                        CLAY_TEXT(MAIN_MENU_SUBS[i],
                            CLAY_TEXT_CONFIG({
                                .fontSize = 11,
                                .letterSpacing = 2,
                                .textColor = COL_TEXT_FAINT,
                            }));
                    }
                }
            }

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

            CLAY(CLAY_ID("MM_Hint"), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .childAlignment = { .x = CLAY_ALIGN_X_RIGHT },
                    .padding = { 0, 4, 0, 0 },
                },
            }) {
                CLAY_TEXT(CLAY_STRING("[ press 2 to view characters ]"),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));
            }
        }
    }
}

// =====================================================================
// Scene 2: Character select
// =====================================================================

typedef struct {
    Clay_String name;
    Clay_String klass;
    int level;
    int hp;
    int hpMax;
    int mp;
    int mpMax;
    int strength, dexterity, intellect, vitality;
    Clay_String bio;
    Clay_Color accent;
} CharacterSlot;

static const CharacterSlot CHARACTERS[] = {
    {
        CLAY_STRING_CONST("BREA OF VARN"), CLAY_STRING_CONST("Rune Paladin"),
        14, 412, 480, 92, 160, 18, 11, 14, 17,
        CLAY_STRING_CONST("Bound to a half-remembered oath. Wields auroral light with the weight of a question."),
        { 230, 196, 110, 255 },
    },
    {
        CLAY_STRING_CONST("KAERIS"), CLAY_STRING_CONST("Shadowblade"),
        12, 280, 320, 140, 220, 9, 19, 16, 11,
        CLAY_STRING_CONST("Trained in three cities, exiled from all of them. Quiet, precise, never twice in the same alley."),
        { 168, 142, 220, 255 },
    },
    {
        CLAY_STRING_CONST("OREN BLACKMOOR"), CLAY_STRING_CONST("Stormcaller"),
        13, 240, 290, 360, 420, 8, 12, 21, 10,
        CLAY_STRING_CONST("Speaks to the air and listens carefully. The air has opinions and it does not always agree."),
        { 110, 196, 230, 255 },
    },
    {
        CLAY_STRING_CONST("VENN ASH"), CLAY_STRING_CONST("Beastwarden"),
        11, 360, 400, 80, 140, 16, 17, 10, 15,
        CLAY_STRING_CONST("Walks with a wolf called nothing. Knows the names of seventeen northern berries and which ones lie."),
        { 156, 220, 122, 255 },
    },
};
#define CHAR_COUNT ((int)(sizeof(CHARACTERS)/sizeof(CHARACTERS[0])))

void Scene_Character(AppState *state) {
    if (state->selectedCharacter < 0 || state->selectedCharacter >= CHAR_COUNT) {
        state->selectedCharacter = 0;
    }
    const CharacterSlot *sel = &CHARACTERS[state->selectedCharacter];

    CLAY(CLAY_ID("CH_Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .padding = { 32, 32, 28, 28 },
            .childGap = 18,
        },
        .backgroundColor = COL_BG_DEEP,
    }) {
        // Top breadcrumb / title row
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .childGap = 16,
            },
        }) {
            CLAY_TEXT(CLAY_STRING("CHOOSE YOUR HERO"),
                CLAY_TEXT_CONFIG({ .fontSize = 26, .letterSpacing = 6, .textColor = COL_ACCENT }));
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
            CLAY_TEXT(UI_Fmt("slot %d of %d", state->selectedCharacter + 1, CHAR_COUNT),
                CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));
        }
        CLAY_AUTO_ID({
            .layout = { .sizing = { CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(3) } },
            .backgroundColor = COL_ACCENT,
        }) {}

        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .childGap = 22,
            },
        }) {
            // Left column: list of selectable cards
            CLAY(CLAY_ID("CH_List"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(320), CLAY_SIZING_GROW(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 10,
                },
            }) {
                for (int i = 0; i < CHAR_COUNT; i++) {
                    const CharacterSlot *c = &CHARACTERS[i];
                    bool active = (state->selectedCharacter == i);
                    Clay_ElementId id = CLAY_IDI("CH_Card", i);
                    CLAY(id, {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(80) },
                            .padding = { 14, 14, 12, 12 },
                            .childGap = 14,
                            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = active ? COL_PANEL_HOVER : (Clay_Hovered() ? COL_PANEL : COL_BG),
                        .cornerRadius = CLAY_CORNER_RADIUS(4),
                        .border = {
                            .color = active ? c->accent : COL_BORDER,
                            .width = active ? (Clay_BorderWidth){2,2,2,2} : (Clay_BorderWidth){1,1,1,1},
                        },
                    }) {
                        // Portrait stand-in
                        CLAY(CLAY_IDI("CH_Portrait", i), {
                            .layout = {
                                .sizing = { CLAY_SIZING_FIXED(56), CLAY_SIZING_FIXED(56) },
                                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                            },
                            .backgroundColor = COL_BG_DEEP,
                            .border = { .color = c->accent, .width = { 2, 2, 2, 2 } },
                            .cornerRadius = CLAY_CORNER_RADIUS(2),
                        }) {
                            // Initials of the name as the avatar
                            char initials[3] = { c->name.chars[0], ' ', 0 };
                            CLAY_TEXT(UI_Fmt("%c", c->name.chars[0]),
                                CLAY_TEXT_CONFIG({ .fontSize = 28, .letterSpacing = 2, .textColor = c->accent }));
                        }

                        CLAY(CLAY_IDI("CH_CardText", i), {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childGap = 4,
                            },
                        }) {
                            CLAY_TEXT(c->name,
                                CLAY_TEXT_CONFIG({ .fontSize = 14, .letterSpacing = 2, .textColor = COL_TEXT }));
                            CLAY_TEXT(c->klass,
                                CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 1, .textColor = COL_TEXT_DIM }));
                            CLAY_TEXT(UI_Fmt("LV %d", c->level),
                                CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 2, .textColor = COL_ACCENT }));
                        }
                    }
                }

                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

                CLAY(CLAY_ID("CH_NewSlot"), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(44) },
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                    },
                    .backgroundColor = Clay_Hovered() ? COL_PANEL : COL_BG,
                    .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                    .cornerRadius = CLAY_CORNER_RADIUS(2),
                }) {
                    CLAY_TEXT(CLAY_STRING("+   NEW HERO"),
                        CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 4,
                            .textColor = Clay_Hovered() ? COL_ACCENT : COL_TEXT_FAINT }));
                }
            }

            // Right column: preview detail
            CLAY(CLAY_ID("CH_Detail"), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                    .padding = { 32, 32, 28, 24 },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 16,
                },
                .backgroundColor = COL_PANEL,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .border = { .color = sel->accent, .width = { .top = 2 } },
            }) {
                // Big banner row
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .childAlignment = { .y = CLAY_ALIGN_Y_BOTTOM },
                        .childGap = 16,
                    },
                }) {
                    CLAY_AUTO_ID({
                        .layout = {
                            .sizing = { CLAY_SIZING_FIXED(110), CLAY_SIZING_FIXED(110) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = COL_BG_DEEP,
                        .border = { .color = sel->accent, .width = { 3, 3, 3, 3 } },
                    }) {
                        CLAY_TEXT(UI_Fmt("%c", sel->name.chars[0]),
                            CLAY_TEXT_CONFIG({ .fontSize = 64, .letterSpacing = 2, .textColor = sel->accent }));
                    }
                    CLAY_AUTO_ID({
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .childGap = 6,
                        },
                    }) {
                        CLAY_TEXT(sel->name,
                            CLAY_TEXT_CONFIG({ .fontSize = 32, .letterSpacing = 4, .textColor = COL_TEXT }));
                        CLAY_TEXT(UI_Fmt("LEVEL %d  -  %.*s", sel->level, sel->klass.length, sel->klass.chars),
                            CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 3, .textColor = sel->accent }));
                    }
                }

                CLAY_TEXT(sel->bio,
                    CLAY_TEXT_CONFIG({ .fontSize = 13, .lineHeight = 22, .textColor = COL_TEXT_DIM }));

                // Resources
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childGap = 10,
                        .padding = { 0, 0, 8, 0 },
                    },
                }) {
                    StatBar(CLAY_STRING("HEALTH"), (float)sel->hp, (float)sel->hpMax, COL_HP);
                    StatBar(CLAY_STRING("MANA"),   (float)sel->mp, (float)sel->mpMax, COL_MP);
                }

                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(4) } } }) {}
                CLAY_AUTO_ID({
                    .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } },
                    .backgroundColor = COL_BORDER,
                }) {}
                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(4) } } }) {}

                // Stat grid
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .childGap = 12,
                    },
                }) {
                    int stats[4] = { sel->strength, sel->dexterity, sel->intellect, sel->vitality };
                    Clay_String labels[4] = {
                        CLAY_STRING("STR"), CLAY_STRING("DEX"),
                        CLAY_STRING("INT"), CLAY_STRING("VIT"),
                    };
                    for (int s = 0; s < 4; s++) {
                        CLAY(CLAY_IDI("CH_Stat", s), {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(70) },
                                .padding = { 12, 12, 10, 10 },
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childAlignment = { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER },
                                .childGap = 4,
                            },
                            .backgroundColor = COL_BG,
                            .cornerRadius = CLAY_CORNER_RADIUS(2),
                            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                        }) {
                            CLAY_TEXT(labels[s],
                                CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 3, .textColor = COL_TEXT_FAINT }));
                            CLAY_TEXT(UI_Fmt("%d", stats[s]),
                                CLAY_TEXT_CONFIG({ .fontSize = 28, .letterSpacing = 1, .textColor = COL_TEXT }));
                        }
                    }
                }

                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

                // Action buttons
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .childGap = 12,
                    },
                }) {
                    CLAY(CLAY_ID("CH_BtnDelete"), {
                        .layout = {
                            .sizing = { CLAY_SIZING_FIT(0), CLAY_SIZING_FIXED(40) },
                            .padding = { 22, 22, 0, 0 },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = Clay_Hovered() ? COL_DANGER : COL_BG,
                        .border = { .color = COL_DANGER, .width = { 1, 1, 1, 1 } },
                    }) {
                        CLAY_TEXT(CLAY_STRING("DELETE"),
                            CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 4,
                                .textColor = Clay_Hovered() ? COL_TEXT : COL_DANGER }));
                    }
                    CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
                    CLAY(CLAY_ID("CH_BtnEnter"), {
                        .layout = {
                            .sizing = { CLAY_SIZING_FIXED(220), CLAY_SIZING_FIXED(40) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = Clay_Hovered() ? COL_ACCENT : COL_ACCENT_DIM,
                    }) {
                        CLAY_TEXT(CLAY_STRING("ENTER THE WORLD  >"),
                            CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 4, .textColor = COL_BG_DEEP }));
                    }
                }
            }
        }
    }

    // Click handling: switch selection when a card is hovered+pressed
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < CHAR_COUNT; i++) {
            if (Clay_PointerOver(CLAY_IDI("CH_Card", i))) {
                state->selectedCharacter = i;
                break;
            }
        }
    }
}

// =====================================================================
// Scene 3: Inventory / equipment
// =====================================================================

typedef struct {
    Clay_String name;
    Clay_String type;
    int qty;
    int tier; // 0=common,1=uncommon,2=rare,3=epic,4=legendary
    Clay_String flavor;
} Item;

static const Clay_String EQUIP_SLOTS[] = {
    CLAY_STRING_CONST("HEAD"),
    CLAY_STRING_CONST("CLOAK"),
    CLAY_STRING_CONST("CHEST"),
    CLAY_STRING_CONST("HANDS"),
    CLAY_STRING_CONST("WAIST"),
    CLAY_STRING_CONST("LEGS"),
    CLAY_STRING_CONST("FEET"),
    CLAY_STRING_CONST("MAIN HAND"),
    CLAY_STRING_CONST("OFF HAND"),
    CLAY_STRING_CONST("RING I"),
    CLAY_STRING_CONST("RING II"),
    CLAY_STRING_CONST("AMULET"),
};
static const Clay_String EQUIP_VALUES[] = {
    CLAY_STRING_CONST("Helm of the Pale Watch"),
    CLAY_STRING_CONST("Travelworn Mantle"),
    CLAY_STRING_CONST("Brigandine of Saints"),
    CLAY_STRING_CONST("Ironweave Gauntlets"),
    CLAY_STRING_CONST("Buckler Belt"),
    CLAY_STRING_CONST("Greaves of Vahn"),
    CLAY_STRING_CONST("Boots of the Long Road"),
    CLAY_STRING_CONST("Sunsilver Longsword"),
    CLAY_STRING_CONST("Aegis of Morn"),
    CLAY_STRING_CONST("Ring of the Quiet Hour"),
    CLAY_STRING_CONST("-- empty --"),
    CLAY_STRING_CONST("Verdant Sigil"),
};
#define EQUIP_COUNT ((int)(sizeof(EQUIP_SLOTS)/sizeof(EQUIP_SLOTS[0])))

static const Item INV_ITEMS[] = {
    { CLAY_STRING_CONST("Greatsword of Dawn"),   CLAY_STRING_CONST("Two-Handed"), 1,  3, CLAY_STRING_CONST("It hums faintly at sunrise. The sound of it is older than any tongue still spoken.") },
    { CLAY_STRING_CONST("Pale Health Tonic"),    CLAY_STRING_CONST("Consumable"), 12, 0, CLAY_STRING_CONST("Restores 120 HP over 6s. Tastes of salt and pennies.") },
    { CLAY_STRING_CONST("Verdant Sigil"),        CLAY_STRING_CONST("Amulet"),     1,  2, CLAY_STRING_CONST("+8 INT, +4 VIT. Glows faintly near old growth.") },
    { CLAY_STRING_CONST("Bone Arrow"),           CLAY_STRING_CONST("Ammunition"), 84, 0, CLAY_STRING_CONST("Hand-fletched. Pulls a little left.") },
    { CLAY_STRING_CONST("Sunsilver Longsword"),  CLAY_STRING_CONST("One-Handed"), 1,  4, CLAY_STRING_CONST("Legendary. Returned, somehow, from beneath Hollow Peaks.") },
    { CLAY_STRING_CONST("Ration"),               CLAY_STRING_CONST("Consumable"), 6,  0, CLAY_STRING_CONST("Hard biscuit, smoked fish, a little salt.") },
    { CLAY_STRING_CONST("Throwing Knife"),       CLAY_STRING_CONST("Throwable"),  9,  1, CLAY_STRING_CONST("Balanced for the off-hand. +12 DEX while equipped.") },
    { CLAY_STRING_CONST("Manaroot Vial"),        CLAY_STRING_CONST("Consumable"), 4,  1, CLAY_STRING_CONST("Restores 80 MP instantly. Use sparingly: it leaves an ache.") },
    { CLAY_STRING_CONST("Ironweave Gauntlets"),  CLAY_STRING_CONST("Hands"),      1,  2, CLAY_STRING_CONST("+12 ARM. Tightens of its own accord in a fight.") },
    { CLAY_STRING_CONST("Letter to Aren"),       CLAY_STRING_CONST("Quest"),      1,  2, CLAY_STRING_CONST("Sealed in green wax. Do not open. (You have opened it.)") },
    { CLAY_STRING_CONST("Wolf Pelt"),            CLAY_STRING_CONST("Material"),   3,  0, CLAY_STRING_CONST("Tradeable. Worth more in Varnholm than here.") },
    { CLAY_STRING_CONST("Lockpick"),             CLAY_STRING_CONST("Tool"),       7,  0, CLAY_STRING_CONST("Snaps on a 1.") },
    { CLAY_STRING_CONST("Tomb-Glass Shard"),     CLAY_STRING_CONST("Material"),   2,  3, CLAY_STRING_CONST("Cold to the touch. The reflection arrives a moment late.") },
    { CLAY_STRING_CONST("Ember Dust"),           CLAY_STRING_CONST("Reagent"),    14, 1, CLAY_STRING_CONST("Component for fire weaving. Keep away from breath.") },
    { CLAY_STRING_CONST("Cracked Compass"),      CLAY_STRING_CONST("Trinket"),    1,  0, CLAY_STRING_CONST("Points west. Always west. You are not always going west.") },
    { CLAY_STRING_CONST("Helm of the Pale Watch"), CLAY_STRING_CONST("Head"),     1,  3, CLAY_STRING_CONST("Once worn by the last sentinel of Morn. He came down the hill alone.") },
    { CLAY_STRING_CONST("Travelworn Mantle"),    CLAY_STRING_CONST("Cloak"),      1,  1, CLAY_STRING_CONST("+6 ARM. Smells faintly of cedar smoke.") },
    { CLAY_STRING_CONST("Brigandine of Saints"), CLAY_STRING_CONST("Chest"),      1,  3, CLAY_STRING_CONST("Each rivet is etched with a name. None of them yours.") },
    { CLAY_STRING_CONST("Buckler Belt"),         CLAY_STRING_CONST("Waist"),      1,  1, CLAY_STRING_CONST("+4 ARM, +2 DEX.") },
    { CLAY_STRING_CONST("Greaves of Vahn"),      CLAY_STRING_CONST("Legs"),       1,  2, CLAY_STRING_CONST("Worn by a saint who walked the long road twice.") },
};
#define INV_COUNT ((int)(sizeof(INV_ITEMS)/sizeof(INV_ITEMS[0])))

// Inventory filter categories: must match the order of the filter pill labels.
//   0 = ALL, 1 = WEAPONS, 2 = ARMOR, 3 = CONSUMABLES, 4 = QUEST, 5 = MISC
static int ItemCategory(Clay_String type) {
    #define EQ(lit) (type.length == (int)(sizeof(lit) - 1) && \
                    strncmp(type.chars, (lit), sizeof(lit) - 1) == 0)
    if (EQ("Two-Handed") || EQ("One-Handed") || EQ("Throwable") || EQ("Ammunition")) return 1;
    if (EQ("Head")   || EQ("Cloak")  || EQ("Chest") || EQ("Hands")    ||
        EQ("Waist")  || EQ("Legs")   || EQ("Feet")  || EQ("Off Hand") ||
        EQ("Amulet") || EQ("Ring"))                                       return 2;
    if (EQ("Consumable")) return 3;
    if (EQ("Quest"))      return 4;
    return 5;
    #undef EQ
}

static bool ItemMatchesFilter(const Item *it, int filter) {
    return filter == 0 || ItemCategory(it->type) == filter;
}

static Clay_Color TierColor(int tier) {
    switch (tier) {
        case 0: return COL_TEXT_DIM;
        case 1: return (Clay_Color){ 156, 220, 122, 255 };
        case 2: return (Clay_Color){ 110, 196, 230, 255 };
        case 3: return (Clay_Color){ 196, 130, 230, 255 };
        case 4: default: return COL_ACCENT;
    }
}
static Clay_String TierLabel(int tier) {
    switch (tier) {
        case 0: return CLAY_STRING("COMMON");
        case 1: return CLAY_STRING("UNCOMMON");
        case 2: return CLAY_STRING("RARE");
        case 3: return CLAY_STRING("EPIC");
        case 4: default: return CLAY_STRING("LEGENDARY");
    }
}

void Scene_Inventory(AppState *state) {
    if (state->selectedInventoryItem < 0) state->selectedInventoryItem = 0;
    if (state->selectedInventoryItem >= INV_COUNT) state->selectedInventoryItem = INV_COUNT - 1;

    // Update hovered each frame (recomputed below in click logic)
    state->hoveredInventoryItem = -1;

    CLAY(CLAY_ID("INV_Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .padding = { 28, 28, 26, 26 },
            .childGap = 18,
        },
        .backgroundColor = COL_BG_DEEP,
    }) {
        // Equipment paper-doll column
        CLAY(CLAY_ID("INV_Equip"), {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(240), CLAY_SIZING_GROW(0) },
                .padding = { 18, 18, 18, 18 },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 8,
            },
            .backgroundColor = COL_PANEL,
            .cornerRadius = CLAY_CORNER_RADIUS(4),
            .border = { .color = COL_ACCENT, .width = { .top = 2 } },
        }) {
            CLAY_TEXT(CLAY_STRING("EQUIPMENT"),
                CLAY_TEXT_CONFIG({ .fontSize = 16, .letterSpacing = 4, .textColor = COL_ACCENT }));
            CLAY_TEXT(CLAY_STRING("brea of varn  -  rune paladin"),
                CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(10) } } }) {}

            for (int i = 0; i < EQUIP_COUNT; i++) {
                bool empty = (EQUIP_VALUES[i].length >= 2 && EQUIP_VALUES[i].chars[0] == '-');
                CLAY(CLAY_IDI("INV_Slot", i), {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(28) },
                        .padding = { 10, 10, 0, 0 },
                        .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                        .childGap = 10,
                    },
                    .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_BG,
                    .cornerRadius = CLAY_CORNER_RADIUS(2),
                }) {
                    CLAY(CLAY_IDI("INV_SlotDot", i), {
                        .layout = { .sizing = { CLAY_SIZING_FIXED(6), CLAY_SIZING_FIXED(6) } },
                        .backgroundColor = empty ? COL_TEXT_FAINT : COL_ACCENT,
                        .cornerRadius = CLAY_CORNER_RADIUS(3),
                    }) {}
                    CLAY_TEXT(EQUIP_SLOTS[i],
                        CLAY_TEXT_CONFIG({
                            .fontSize = 10, .letterSpacing = 3,
                            .textColor = COL_TEXT_FAINT,
                        }));
                    CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_FIXED(6), CLAY_SIZING_FIXED(1) } } }) {}
                    CLAY_TEXT(EQUIP_VALUES[i],
                        CLAY_TEXT_CONFIG({
                            .fontSize = 11,
                            .textColor = empty ? COL_TEXT_FAINT : COL_TEXT,
                        }));
                }
            }

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

            // Aggregate stats footer
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .padding = { 12, 12, 12, 12 },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 4,
                },
                .backgroundColor = COL_BG_DEEP,
                .cornerRadius = CLAY_CORNER_RADIUS(3),
                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
            }) {
                CLAY_TEXT(CLAY_STRING("ARMOR  142   |   DPS  64   |   WEIGHT  38 / 80"),
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 2, .textColor = COL_TEXT_DIM }));
                CLAY_TEXT(CLAY_STRING("set bonus:  warden's vigil (2/4)"),
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 1, .textColor = COL_ACCENT }));
            }
        }

        // Middle column: scrollable inventory list
        CLAY(CLAY_ID("INV_ListCol"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 10,
            },
        }) {
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                    .childGap = 12,
                    .padding = { 4, 4, 0, 0 },
                },
            }) {
                CLAY_TEXT(CLAY_STRING("INVENTORY"),
                    CLAY_TEXT_CONFIG({ .fontSize = 16, .letterSpacing = 4, .textColor = COL_ACCENT }));
                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
                CLAY_TEXT(UI_Fmt("%d items   .   13.4 / 30.0 kg", INV_COUNT),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));
            }

            // Filter pills
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .childGap = 8,
                    .padding = { 4, 4, 0, 4 },
                },
            }) {
                Clay_String pills[] = {
                    CLAY_STRING("ALL"), CLAY_STRING("WEAPONS"),
                    CLAY_STRING("ARMOR"), CLAY_STRING("CONSUM."),
                    CLAY_STRING("QUEST"), CLAY_STRING("MISC"),
                };
                int pillCount = (int)(sizeof(pills)/sizeof(pills[0]));
                for (int p = 0; p < pillCount; p++) {
                    bool active = (state->inventoryFilter == p);
                    CLAY(CLAY_IDI("INV_Pill", p), {
                        .layout = {
                            .padding = { 12, 12, 6, 6 },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = active ? COL_ACCENT : (Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL),
                        .cornerRadius = CLAY_CORNER_RADIUS(2),
                    }) {
                        CLAY_TEXT(pills[p],
                            CLAY_TEXT_CONFIG({
                                .fontSize = 10, .letterSpacing = 2,
                                .textColor = active ? COL_BG_DEEP : COL_TEXT_DIM,
                            }));
                    }
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (int p = 0; p < pillCount; p++) {
                        if (Clay_PointerOver(CLAY_IDI("INV_Pill", p))) {
                            state->inventoryFilter = p;
                            break;
                        }
                    }
                }
            }

            // Scrollable list
            CLAY(CLAY_ID("INV_Scroll"), {
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = { 2, 2, 2, 2 },
                },
                .backgroundColor = COL_PANEL,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
            }) {
                for (int i = 0; i < INV_COUNT; i++) {
                    const Item *it = &INV_ITEMS[i];
                    if (!ItemMatchesFilter(it, state->inventoryFilter)) continue;
                    bool selected = (state->selectedInventoryItem == i);
                    Clay_ElementId id = CLAY_IDI("INV_Row", i);
                    CLAY(id, {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(46) },
                            .padding = { 14, 14, 0, 0 },
                            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                            .childGap = 14,
                        },
                        .backgroundColor = selected
                            ? COL_PANEL_HOVER
                            : (Clay_Hovered() ? COL_BG : COL_TRANSPARENT),
                        .border = { .color = TierColor(it->tier),
                            .width = selected ? (Clay_BorderWidth){ .left = 4 } : (Clay_BorderWidth){0} },
                    }) {
                        if (Clay_Hovered()) state->hoveredInventoryItem = i;
                        // Tier color swatch
                        CLAY(CLAY_IDI("INV_RowDot", i), {
                            .layout = { .sizing = { CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8) } },
                            .backgroundColor = TierColor(it->tier),
                            .cornerRadius = CLAY_CORNER_RADIUS(4),
                        }) {}

                        CLAY(CLAY_IDI("INV_RowName", i), {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childGap = 2,
                            },
                        }) {
                            CLAY_TEXT(it->name,
                                CLAY_TEXT_CONFIG({ .fontSize = 13, .textColor = TierColor(it->tier) }));
                            CLAY_TEXT(it->type,
                                CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));
                        }

                        CLAY_TEXT(UI_Fmt("x%d", it->qty),
                            CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 1, .textColor = COL_TEXT_DIM }));
                    }
                }
            }
        }

        // Right column: item detail panel
        const Item *detail = &INV_ITEMS[state->selectedInventoryItem];
        CLAY(CLAY_ID("INV_Detail"), {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(280), CLAY_SIZING_GROW(0) },
                .padding = { 22, 22, 22, 22 },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 14,
            },
            .backgroundColor = COL_PANEL,
            .cornerRadius = CLAY_CORNER_RADIUS(4),
            .border = { .color = TierColor(detail->tier), .width = { .top = 3 } },
        }) {
            // Item icon stand-in
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(120) },
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = COL_BG_DEEP,
                .border = { .color = TierColor(detail->tier), .width = { 2, 2, 2, 2 } },
                .cornerRadius = CLAY_CORNER_RADIUS(3),
            }) {
                CLAY_TEXT(UI_Fmt("%c", detail->name.chars[0]),
                    CLAY_TEXT_CONFIG({ .fontSize = 72, .textColor = TierColor(detail->tier) }));
            }

            CLAY_TEXT(detail->name,
                CLAY_TEXT_CONFIG({ .fontSize = 20, .letterSpacing = 2, .textColor = COL_TEXT }));
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .childGap = 10,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                },
            }) {
                CLAY_TEXT(TierLabel(detail->tier),
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 3, .textColor = TierColor(detail->tier) }));
                CLAY_TEXT(CLAY_STRING("|"),
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .textColor = COL_BORDER }));
                CLAY_TEXT(detail->type,
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 2, .textColor = COL_TEXT_DIM }));
            }

            CLAY_AUTO_ID({
                .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } },
                .backgroundColor = COL_BORDER,
            }) {}

            CLAY_TEXT(detail->flavor,
                CLAY_TEXT_CONFIG({ .fontSize = 12, .lineHeight = 20, .textColor = COL_TEXT_DIM }));

            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } } }) {}

            // Action buttons
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .childGap = 8,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                },
            }) {
                Clay_String actions[] = {
                    CLAY_STRING("EQUIP"),
                    CLAY_STRING("USE"),
                    CLAY_STRING("DROP"),
                };
                for (int a = 0; a < 3; a++) {
                    CLAY(CLAY_IDI("INV_Action", a), {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(34) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = (a == 0)
                            ? (Clay_Hovered() ? COL_ACCENT : COL_ACCENT_DIM)
                            : (Clay_Hovered() ? COL_PANEL_HOVER : COL_BG),
                        .border = { .color = (a == 0) ? COL_ACCENT : COL_BORDER, .width = { 1, 1, 1, 1 } },
                    }) {
                        CLAY_TEXT(actions[a],
                            CLAY_TEXT_CONFIG({
                                .fontSize = 12, .letterSpacing = 4,
                                .textColor = (a == 0) ? COL_BG_DEEP : COL_TEXT_DIM,
                            }));
                    }
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (int i = 0; i < INV_COUNT; i++) {
            if (Clay_PointerOver(CLAY_IDI("INV_Row", i))) {
                state->selectedInventoryItem = i;
                break;
            }
        }
    }
}

// =====================================================================
// Scene 4: Settings
// =====================================================================

static const Clay_String SETTINGS_TABS[SETTINGS_TAB_COUNT] = {
    CLAY_STRING_CONST("GRAPHICS"),
    CLAY_STRING_CONST("AUDIO"),
    CLAY_STRING_CONST("CONTROLS"),
};

static void Toggle(int id, Clay_String label, Clay_String help, bool *value) {
    Clay_ElementId rowId = CLAY_IDI("ST_Row", id);
    CLAY(rowId, {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
            .padding = { 16, 16, 14, 14 },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 16,
        },
        .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
        .cornerRadius = CLAY_CORNER_RADIUS(2),
        .border = { .color = COL_BORDER, .width = { .bottom = 1 } },
    }) {
        CLAY(CLAY_IDI("ST_RowText", id), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 4,
            },
        }) {
            CLAY_TEXT(label,
                CLAY_TEXT_CONFIG({ .fontSize = 14, .letterSpacing = 2, .textColor = COL_TEXT }));
            CLAY_TEXT(help,
                CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 1, .textColor = COL_TEXT_FAINT }));
        }
        // Toggle track
        Clay_ElementId toggleId = CLAY_IDI("ST_Toggle", id);
        CLAY(toggleId, {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(46), CLAY_SIZING_FIXED(22) },
                .padding = { 3, 3, 3, 3 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = *value ? COL_ACCENT : COL_BG_DEEP,
            .cornerRadius = CLAY_CORNER_RADIUS(11),
            .border = { .color = *value ? COL_ACCENT : COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            CLAY_AUTO_ID({ .layout = { .sizing = { .width = *value
                ? CLAY_SIZING_GROW(0)
                : CLAY_SIZING_FIXED(0), .height = CLAY_SIZING_FIXED(16) } } }) {}
            CLAY_AUTO_ID({
                .layout = { .sizing = { CLAY_SIZING_FIXED(16), CLAY_SIZING_FIXED(16) } },
                .backgroundColor = COL_BG_DEEP,
                .cornerRadius = CLAY_CORNER_RADIUS(8),
            }) {}
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Clay_PointerOver(rowId)) {
        *value = !*value;
    }
}

static void Slider(int id, Clay_String label, float *value) {
    Clay_ElementId rowId   = CLAY_IDI("ST_SliderRow",   id);
    Clay_ElementId trackId = CLAY_IDI("ST_SliderTrack", id);
    CLAY(rowId, {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
            .padding = { 16, 16, 14, 14 },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 18,
        },
        .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
        .border = { .color = COL_BORDER, .width = { .bottom = 1 } },
    }) {
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(160), CLAY_SIZING_FIT(0) },
            },
        }) {
            CLAY_TEXT(label,
                CLAY_TEXT_CONFIG({ .fontSize = 14, .letterSpacing = 2, .textColor = COL_TEXT }));
        }

        CLAY(trackId, {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(8) },
            },
            .backgroundColor = COL_BG_DEEP,
            .cornerRadius = CLAY_CORNER_RADIUS(3),
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            float v = *value;
            if (v < 0.0f) v = 0.0f;
            if (v > 1.0f) v = 1.0f;
            CLAY_AUTO_ID({
                .layout = { .sizing = { CLAY_SIZING_PERCENT(v), CLAY_SIZING_GROW(0) } },
                .backgroundColor = COL_ACCENT,
                .cornerRadius = CLAY_CORNER_RADIUS(3),
            }) {}
        }

        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(60), CLAY_SIZING_FIT(0) },
                .childAlignment = { .x = CLAY_ALIGN_X_RIGHT },
            },
        }) {
            CLAY_TEXT(UI_Fmt("%3d%%", (int)((*value) * 100.0f + 0.5f)),
                CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 1, .textColor = COL_ACCENT }));
        }
    }

    // Click-to-set on the track: position derived from mouse x within the track bounds.
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && Clay_PointerOver(trackId)) {
        Clay_ElementData data = Clay_GetElementData(trackId);
        if (data.found) {
            Vector2 m = GetMousePosition();
            float t = (m.x - data.boundingBox.x) / data.boundingBox.width;
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;
            *value = t;
        }
    }
}

static void Stepper(int id, Clay_String label, int *value, int min, int max, const Clay_String *options) {
    Clay_ElementId rowId = CLAY_IDI("ST_StepRow", id);
    CLAY(rowId, {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
            .padding = { 16, 16, 14, 14 },
            .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
            .childGap = 18,
        },
        .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
        .border = { .color = COL_BORDER, .width = { .bottom = 1 } },
    }) {
        CLAY_AUTO_ID({
            .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) } },
        }) {
            CLAY_TEXT(label,
                CLAY_TEXT_CONFIG({ .fontSize = 14, .letterSpacing = 2, .textColor = COL_TEXT }));
        }

        Clay_ElementId leftId  = CLAY_IDI("ST_StepLeft",  id);
        Clay_ElementId rightId = CLAY_IDI("ST_StepRight", id);

        CLAY(leftId, {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(28), CLAY_SIZING_FIXED(28) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = Clay_Hovered() ? COL_ACCENT : COL_BG,
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            CLAY_TEXT(CLAY_STRING("<"),
                CLAY_TEXT_CONFIG({ .fontSize = 14, .textColor = Clay_Hovered() ? COL_BG_DEEP : COL_TEXT_DIM }));
        }

        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(140), CLAY_SIZING_FIT(0) },
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
            },
        }) {
            CLAY_TEXT(options[*value],
                CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 2, .textColor = COL_ACCENT }));
        }

        CLAY(rightId, {
            .layout = {
                .sizing = { CLAY_SIZING_FIXED(28), CLAY_SIZING_FIXED(28) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = Clay_Hovered() ? COL_ACCENT : COL_BG,
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
        }) {
            CLAY_TEXT(CLAY_STRING(">"),
                CLAY_TEXT_CONFIG({ .fontSize = 14, .textColor = Clay_Hovered() ? COL_BG_DEEP : COL_TEXT_DIM }));
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (Clay_PointerOver(CLAY_IDI("ST_StepLeft", id))) {
            if (*value > min) (*value)--;
        } else if (Clay_PointerOver(CLAY_IDI("ST_StepRight", id))) {
            if (*value < max) (*value)++;
        }
    }
}

void Scene_Settings(AppState *state) {
    CLAY(CLAY_ID("ST_Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .padding = { 36, 36, 32, 28 },
            .childGap = 22,
        },
        .backgroundColor = COL_BG_DEEP,
    }) {
        CLAY_TEXT(CLAY_STRING("SETTINGS"),
            CLAY_TEXT_CONFIG({ .fontSize = 30, .letterSpacing = 8, .textColor = COL_ACCENT }));
        CLAY_AUTO_ID({
            .layout = { .sizing = { CLAY_SIZING_FIXED(80), CLAY_SIZING_FIXED(3) } },
            .backgroundColor = COL_ACCENT,
        }) {}

        // Tabs
        CLAY(CLAY_ID("ST_Tabs"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .childGap = 4,
            },
            .border = { .color = COL_BORDER, .width = { .bottom = 1 } },
        }) {
            for (int t = 0; t < SETTINGS_TAB_COUNT; t++) {
                bool active = (state->settingsTab == (SettingsTab)t);
                Clay_ElementId tabId = CLAY_IDI("ST_Tab", t);
                CLAY(tabId, {
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(.min = 110, .max = 180), CLAY_SIZING_FIXED(40) },
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                    },
                    .backgroundColor = active ? COL_PANEL : (Clay_Hovered() ? COL_BG : COL_TRANSPARENT),
                    .border = {
                        .color = COL_ACCENT,
                        .width = { .bottom = active ? 3 : 0 },
                    },
                }) {
                    CLAY_TEXT(SETTINGS_TABS[t],
                        CLAY_TEXT_CONFIG({
                            .fontSize = 13, .letterSpacing = 4,
                            .textColor = active ? COL_ACCENT : (Clay_Hovered() ? COL_TEXT : COL_TEXT_DIM),
                        }));
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Clay_PointerOver(tabId)) {
                    state->settingsTab = (SettingsTab)t;
                }
            }
        }

        CLAY(CLAY_ID("ST_TabBody"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
            },
        }) {
            switch (state->settingsTab) {
                case SETTINGS_TAB_GRAPHICS: {
                    static const Clay_String QUALITY_OPTS[] = {
                        CLAY_STRING_CONST("Potato"),
                        CLAY_STRING_CONST("Low"),
                        CLAY_STRING_CONST("Medium"),
                        CLAY_STRING_CONST("High"),
                        CLAY_STRING_CONST("Ultra"),
                    };
                    Stepper(0, CLAY_STRING("Quality Preset"), &state->qualityPreset, 0, 4, QUALITY_OPTS);
                    Toggle(1, CLAY_STRING("Fullscreen"),
                        CLAY_STRING("Borderless on macOS. Restart not required."), &state->fullscreen);
                    Toggle(2, CLAY_STRING("Vertical sync"),
                        CLAY_STRING("Cap framerate to display refresh rate."), &state->vsync);
                    Toggle(3, CLAY_STRING("Screen shake"),
                        CLAY_STRING("Camera responds to nearby impacts."), &state->screenShake);
                    break;
                }
                case SETTINGS_TAB_AUDIO: {
                    Slider(10, CLAY_STRING("Master volume"), &state->masterVolume);
                    Slider(11, CLAY_STRING("Music volume"),  &state->musicVolume);
                    Slider(12, CLAY_STRING("Sound effects"), &state->sfxVolume);
                    Toggle(13, CLAY_STRING("Subtitles"),
                        CLAY_STRING("Display spoken dialogue as captions."), &state->subtitles);
                    break;
                }
                case SETTINGS_TAB_CONTROLS: {
                    static const Clay_String KEYS_LBL[] = {
                        CLAY_STRING_CONST("Move forward"),
                        CLAY_STRING_CONST("Move back"),
                        CLAY_STRING_CONST("Strafe left"),
                        CLAY_STRING_CONST("Strafe right"),
                        CLAY_STRING_CONST("Jump / dodge"),
                        CLAY_STRING_CONST("Interact"),
                        CLAY_STRING_CONST("Inventory"),
                        CLAY_STRING_CONST("Quick map"),
                    };
                    static const Clay_String KEYS_BIND[] = {
                        CLAY_STRING_CONST("W"),
                        CLAY_STRING_CONST("S"),
                        CLAY_STRING_CONST("A"),
                        CLAY_STRING_CONST("D"),
                        CLAY_STRING_CONST("SPACE"),
                        CLAY_STRING_CONST("E"),
                        CLAY_STRING_CONST("I"),
                        CLAY_STRING_CONST("M"),
                    };
                    int n = sizeof(KEYS_LBL)/sizeof(KEYS_LBL[0]);
                    for (int k = 0; k < n; k++) {
                        CLAY(CLAY_IDI("ST_Bind", k), {
                            .layout = {
                                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                                .padding = { 16, 16, 12, 12 },
                                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                            },
                            .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
                            .border = { .color = COL_BORDER, .width = { .bottom = 1 } },
                        }) {
                            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) } } }) {
                                CLAY_TEXT(KEYS_LBL[k],
                                    CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 2, .textColor = COL_TEXT }));
                            }
                            CLAY_AUTO_ID({
                                .layout = {
                                    .sizing = { CLAY_SIZING_FIXED(120), CLAY_SIZING_FIXED(32) },
                                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                                },
                                .backgroundColor = COL_BG_DEEP,
                                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                                .cornerRadius = CLAY_CORNER_RADIUS(2),
                            }) {
                                CLAY_TEXT(KEYS_BIND[k],
                                    CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 3, .textColor = COL_ACCENT }));
                            }
                        }
                    }
                    break;
                }
                default: break;
            }
        }

        // Footer action row
        CLAY_AUTO_ID({
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .childGap = 12,
                .padding = { 0, 0, 12, 0 },
            },
        }) {
            CLAY(CLAY_ID("ST_Defaults"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(160), CLAY_SIZING_FIXED(38) },
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
            }) {
                CLAY_TEXT(CLAY_STRING("RESTORE DEFAULTS"),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 3, .textColor = COL_TEXT_DIM }));
            }
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
            CLAY(CLAY_ID("ST_Cancel"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(120), CLAY_SIZING_FIXED(38) },
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_BG,
                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
            }) {
                CLAY_TEXT(CLAY_STRING("CANCEL"),
                    CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 4, .textColor = COL_TEXT_DIM }));
            }
            CLAY(CLAY_ID("ST_Apply"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(160), CLAY_SIZING_FIXED(38) },
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = Clay_Hovered() ? COL_ACCENT : COL_ACCENT_DIM,
            }) {
                CLAY_TEXT(CLAY_STRING("APPLY"),
                    CLAY_TEXT_CONFIG({ .fontSize = 13, .letterSpacing = 6, .textColor = COL_BG_DEEP }));
            }
        }
    }
}

// =====================================================================
// Scene 5: In-game HUD + pause overlay
// =====================================================================

void Scene_HUD(AppState *state) {
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
        state->paused = !state->paused;
    }

    CLAY(CLAY_ID("HUD_Root"), {
        .layout = {
            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        },
        .backgroundColor = (Clay_Color){ 24, 18, 18, 255 },
    }) {
        // Top strip: world banner / objective
        CLAY(CLAY_ID("HUD_Top"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .padding = { 24, 24, 16, 16 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                .childGap = 14,
            },
        }) {
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0) },
                    .padding = { 10, 10, 6, 6 },
                },
                .backgroundColor = COL_PANEL,
                .border = { .color = COL_ACCENT, .width = { .left = 3 } },
            }) {
                CLAY_TEXT(CLAY_STRING("HOLLOW PEAKS  -  CINDERHOLD"),
                    CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 4, .textColor = COL_ACCENT }));
            }
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}

            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0) },
                    .padding = { 12, 12, 6, 6 },
                    .childGap = 10,
                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                },
                .backgroundColor = COL_PANEL,
                .cornerRadius = CLAY_CORNER_RADIUS(2),
            }) {
                CLAY_AUTO_ID({
                    .layout = { .sizing = { CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8) } },
                    .backgroundColor = COL_GOLD,
                    .cornerRadius = CLAY_CORNER_RADIUS(4),
                }) {}
                CLAY_TEXT(CLAY_STRING("OBJECTIVE:  find the bell-keeper's notes"),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 1, .textColor = COL_TEXT_DIM }));
            }
        }

        // Center play space (decorative placeholder)
        CLAY(CLAY_ID("HUD_Play"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
        }) {
            CLAY_TEXT(CLAY_STRING("[ play area ]"),
                CLAY_TEXT_CONFIG({ .fontSize = 14, .letterSpacing = 4, .textColor = (Clay_Color){ 70, 56, 56, 220 } }));
        }

        // Bottom HUD strip: HP/MP + hotbar + minimap
        CLAY(CLAY_ID("HUD_Bottom"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                .padding = { 22, 22, 14, 18 },
                .childGap = 22,
                .childAlignment = { .y = CLAY_ALIGN_Y_BOTTOM },
            },
        }) {
            // Resources column (HP/MP/XP)
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(260), CLAY_SIZING_FIT(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = { 14, 14, 10, 10 },
                    .childGap = 8,
                },
                .backgroundColor = COL_PANEL,
                .cornerRadius = CLAY_CORNER_RADIUS(3),
                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
            }) {
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                        .childGap = 10,
                    },
                }) {
                    CLAY_AUTO_ID({
                        .layout = { .sizing = { CLAY_SIZING_FIXED(34), CLAY_SIZING_FIXED(34) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER } },
                        .backgroundColor = COL_BG_DEEP,
                        .border = { .color = COL_ACCENT, .width = { 2, 2, 2, 2 } },
                    }) {
                        CLAY_TEXT(CLAY_STRING("B"),
                            CLAY_TEXT_CONFIG({ .fontSize = 18, .textColor = COL_ACCENT }));
                    }
                    CLAY_AUTO_ID({
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .childGap = 2,
                        },
                    }) {
                        CLAY_TEXT(CLAY_STRING("BREA OF VARN"),
                            CLAY_TEXT_CONFIG({ .fontSize = 12, .letterSpacing = 2, .textColor = COL_TEXT }));
                        CLAY_TEXT(CLAY_STRING("LV 14  -  Rune Paladin"),
                            CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 1, .textColor = COL_TEXT_FAINT }));
                    }
                }
                StatBar(CLAY_STRING("HP"), state->playerHp * 480.0f, 480.0f, COL_HP);
                StatBar(CLAY_STRING("MP"), state->playerMp * 160.0f, 160.0f, COL_MP);
                StatBar(CLAY_STRING("XP"), 0.31f * 1000.0f, 1000.0f, COL_XP);
            }

            // Hotbar
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 6,
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
                },
            }) {
                CLAY_AUTO_ID({
                    .layout = {
                        .childGap = 6,
                    },
                }) {
                    Clay_String slots[] = {
                        CLAY_STRING("SWD"), CLAY_STRING("BOW"),
                        CLAY_STRING("PTN"), CLAY_STRING("RUN"),
                        CLAY_STRING("RUN"), CLAY_STRING("DAG"),
                        CLAY_STRING("---"), CLAY_STRING("---"),
                    };
                    Clay_String keys[] = {
                        CLAY_STRING("1"), CLAY_STRING("2"),
                        CLAY_STRING("3"), CLAY_STRING("4"),
                        CLAY_STRING("5"), CLAY_STRING("6"),
                        CLAY_STRING("7"), CLAY_STRING("8"),
                    };
                    int n = sizeof(slots)/sizeof(slots[0]);
                    for (int s = 0; s < n; s++) {
                        bool empty = (slots[s].chars[0] == '-');
                        CLAY(CLAY_IDI("HUD_Slot", s), {
                            .layout = {
                                .sizing = { CLAY_SIZING_FIXED(58), CLAY_SIZING_FIXED(58) },
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                                .childGap = 2,
                                .padding = { 0, 0, 6, 4 },
                            },
                            .backgroundColor = empty ? COL_BG : COL_PANEL,
                            .border = { .color = empty ? COL_BORDER : COL_ACCENT_DIM, .width = { 1, 1, 1, 1 } },
                        }) {
                            CLAY_TEXT(slots[s],
                                CLAY_TEXT_CONFIG({
                                    .fontSize = 14, .letterSpacing = 2,
                                    .textColor = empty ? COL_TEXT_FAINT : COL_TEXT,
                                }));
                            CLAY_TEXT(keys[s],
                                CLAY_TEXT_CONFIG({
                                    .fontSize = 9, .letterSpacing = 1,
                                    .textColor = COL_TEXT_FAINT,
                                }));
                        }
                    }
                }
            }

            // Minimap
            CLAY_AUTO_ID({
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(160), CLAY_SIZING_FIXED(160) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = { 8, 8, 6, 6 },
                    .childGap = 2,
                },
                .backgroundColor = COL_PANEL,
                .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                .cornerRadius = CLAY_CORNER_RADIUS(2),
            }) {
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(0) },
                        .childAlignment = { .y = CLAY_ALIGN_Y_CENTER },
                        .childGap = 6,
                    },
                }) {
                    CLAY_TEXT(CLAY_STRING("MAP"),
                        CLAY_TEXT_CONFIG({ .fontSize = 9, .letterSpacing = 3, .textColor = COL_ACCENT }));
                    CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(1) } } }) {}
                    CLAY_TEXT(CLAY_STRING("N"),
                        CLAY_TEXT_CONFIG({ .fontSize = 9, .textColor = COL_TEXT_FAINT }));
                }
                // grid stand-in
                CLAY_AUTO_ID({
                    .layout = {
                        .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                    },
                    .backgroundColor = COL_BG_DEEP,
                    .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
                }) {
                    CLAY_AUTO_ID({
                        .layout = { .sizing = { CLAY_SIZING_FIXED(8), CLAY_SIZING_FIXED(8) } },
                        .backgroundColor = COL_ACCENT,
                        .cornerRadius = CLAY_CORNER_RADIUS(4),
                    }) {}
                }
            }
        }
    }

    // Pause overlay
    if (state->paused) {
        CLAY(CLAY_ID("PAUSE_Overlay"), {
            .layout = {
                .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) },
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
            .backgroundColor = (Clay_Color){ 8, 8, 14, 200 },
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ROOT,
                .zIndex = 10,
            },
        }) {
            CLAY(CLAY_ID("PAUSE_Card"), {
                .layout = {
                    .sizing = { CLAY_SIZING_FIXED(380), CLAY_SIZING_FIT(0) },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = { 32, 32, 28, 28 },
                    .childGap = 14,
                    .childAlignment = { .x = CLAY_ALIGN_X_CENTER },
                },
                .backgroundColor = COL_BG,
                .border = { .color = COL_ACCENT, .width = { .top = 3 } },
                .cornerRadius = CLAY_CORNER_RADIUS(2),
            }) {
                CLAY_TEXT(CLAY_STRING("PAUSED"),
                    CLAY_TEXT_CONFIG({ .fontSize = 36, .letterSpacing = 12, .textColor = COL_ACCENT }));
                CLAY_TEXT(CLAY_STRING("the world holds its breath"),
                    CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 3, .textColor = COL_TEXT_FAINT }));

                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(10) } } }) {}

                Clay_String pauseItems[] = {
                    CLAY_STRING("RESUME"),
                    CLAY_STRING("SAVE GAME"),
                    CLAY_STRING("LOAD"),
                    CLAY_STRING("SETTINGS"),
                    CLAY_STRING("QUIT TO MENU"),
                };
                int n = sizeof(pauseItems)/sizeof(pauseItems[0]);
                for (int i = 0; i < n; i++) {
                    CLAY(CLAY_IDI("PAUSE_Item", i), {
                        .layout = {
                            .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(40) },
                            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        },
                        .backgroundColor = Clay_Hovered() ? COL_PANEL_HOVER : COL_PANEL,
                        .border = { .color = Clay_Hovered() ? COL_ACCENT : COL_BORDER, .width = { 1, 1, 1, 1 } },
                    }) {
                        CLAY_TEXT(pauseItems[i],
                            CLAY_TEXT_CONFIG({
                                .fontSize = 13, .letterSpacing = 5,
                                .textColor = Clay_Hovered() ? COL_ACCENT : COL_TEXT,
                            }));
                    }
                }

                CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(4) } } }) {}
                CLAY_TEXT(CLAY_STRING("[esc] or [p] to resume"),
                    CLAY_TEXT_CONFIG({ .fontSize = 10, .letterSpacing = 2, .textColor = COL_TEXT_FAINT }));
            }
        }
    } else {
        // Hint at the bottom
        CLAY(CLAY_ID("HUD_PauseHint"), {
            .layout = {
                .sizing = { CLAY_SIZING_FIT(0), CLAY_SIZING_FIT(0) },
                .padding = { 10, 10, 6, 6 },
            },
            .backgroundColor = COL_PANEL,
            .border = { .color = COL_BORDER, .width = { 1, 1, 1, 1 } },
            .floating = {
                .attachTo = CLAY_ATTACH_TO_ROOT,
                .offset = { 24, 24 },
                .zIndex = 5,
            },
        }) {
            CLAY_TEXT(CLAY_STRING("[esc/p] PAUSE"),
                CLAY_TEXT_CONFIG({ .fontSize = 11, .letterSpacing = 3, .textColor = COL_TEXT_DIM }));
        }
    }
}
