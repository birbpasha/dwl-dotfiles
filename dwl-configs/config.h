/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)    { ((hex >> 24) & 0xFF) / 255.0f, \
                        ((hex >> 16) & 0xFF) / 255.0f, \
                        ((hex >> 8) & 0xFF) / 255.0f, \
                        (hex & 0xFF) / 255.0f }

#include <X11/XF86keysym.h>


/* TO DO: 
 * _________________
 * [X] Add screenshotting
 * [X] Make matching waybar rice
 * [X] Try not to kill yourself
 * [X] Fix SUPER + SHIFT + 2 not working
 * [X] Maybe choose a better wallpaper tf is this
 * [X] Make function keys
 * [X] Add shutting down keybinds
 * [v] Make SUPER + V opening cliphist on fuzzel
 * [idc] Fix waybar not getting workspaces
 * */

/* appearance */
static const int sloppyfocus               = 1;  /* focus follows mouse */
static const int bypass_surface_visibility = 0;  /* 1 means idle inhibitors will disable idle tracking even if it's surface isn't visible  */
static const unsigned int borderpx         = 2;  /* border pixel of windows */
static const float rootcolor[]             = COLOR(0x1f1003ff);
static const float bordercolor[]           = COLOR(0x1f1003ff);
static const float focuscolor[]            = COLOR(0x82562cff);
static const float urgentcolor[]           = COLOR(0xff0000ff);
/* This conforms to the xdg-protocol. Set the alpha to zero to restore the old behavior */
static const float fullscreen_bg[]         = {0.0f, 0.0f, 0.0f, 1.0f}; /* You can also use glsl colors */

/* tagging - TAGCOUNT must be no greater than 31 */
#define TAGCOUNT (9)

/* logging */
static int log_level = WLR_ERROR;

/* NOTE: ALWAYS keep a rule declared even if you don't use rules (e.g leave at least one example) */
static const Rule rules[] = {
	/* app_id             title       tags mask     isfloating   monitor */
	/* examples: */
	{ "Gimp_EXAMPLE",     NULL,       0,            1,           -1 }, /* Start on currently visible tags floating, not tiled */
	{ "firefox_EXAMPLE",  NULL,       1 << 8,       0,           -1 }, /* Start on ONLY tag "9" */
};

/* layout(s) */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* monitors */
/* (x=-1, y=-1) is reserved as an "autoconfigure" monitor position indicator
 * WARNING: negative values other than (-1, -1) cause problems with Xwayland clients
 * https://gitlab.freedesktop.org/xorg/xserver/-/issues/899
*/
/* NOTE: ALWAYS add a fallback rule, even if you are completely sure it won't be used */
static const MonitorRule monrules[] = {
	/* name       mfact  nmaster scale layout       rotate/reflect                x    y */
	/* example of a HiDPI laptop monitor:
	{ "eDP-1",    0.5f,  1,      2,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
	*/
	/* defaults */
	{ NULL,       0.55f, 1,      1,    &layouts[0], WL_OUTPUT_TRANSFORM_NORMAL,   -1,  -1 },
};

/* keyboard */
static const char *xkb_layout = "tr";
static const struct xkb_rule_names xkb_rules = {
	/* can specify fields: rules, model, layout, variant, options */
	/* example:
	.options = "ctrl:nocaps",
	*/
	.options = NULL,
};

static const int repeat_rate = 25;
static const int repeat_delay = 600;

/* Trackpad */
static const int tap_to_click = 1;
static const int tap_and_drag = 1;
static const int drag_lock = 1;
static const int natural_scrolling = 1;
static const int disable_while_typing = 1;
static const int left_handed = 0;
static const int middle_button_emulation = 0;
/* You can choose between:
LIBINPUT_CONFIG_SCROLL_NO_SCROLL
LIBINPUT_CONFIG_SCROLL_2FG
LIBINPUT_CONFIG_SCROLL_EDGE
LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN
*/
static const enum libinput_config_scroll_method scroll_method = LIBINPUT_CONFIG_SCROLL_2FG;

/* You can choose between:
LIBINPUT_CONFIG_CLICK_METHOD_NONE
LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS
LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER
*/
static const enum libinput_config_click_method click_method = LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER;

/* You can choose between:
LIBINPUT_CONFIG_SEND_EVENTS_ENABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED
LIBINPUT_CONFIG_SEND_EVENTS_DISABLED_ON_EXTERNAL_MOUSE
*/
static const uint32_t send_events_mode = LIBINPUT_CONFIG_SEND_EVENTS_ENABLED;

/* You can choose between:
LIBINPUT_CONFIG_ACCEL_PROFILE_FLAT
LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE
*/
static const enum libinput_config_accel_profile accel_profile = LIBINPUT_CONFIG_ACCEL_PROFILE_ADAPTIVE;
static const double accel_speed = 0.0;

/* You can choose between:
LIBINPUT_CONFIG_TAP_MAP_LRM -- 1/2/3 finger tap maps to left/right/middle
LIBINPUT_CONFIG_TAP_MAP_LMR -- 1/2/3 finger tap maps to left/middle/right
*/
static const enum libinput_config_tap_button_map button_map = LIBINPUT_CONFIG_TAP_MAP_LRM;

/* If you want to use the windows key for MODKEY, use WLR_MODIFIER_LOGO */
#define MODKEY WLR_MODIFIER_LOGO

#define TAGKEYS(KEY,SKEY,TAG) \
    { MODKEY,                    KEY,            view,       {.ui = 1 << TAG} }, \
    { MODKEY|WLR_MODIFIER_CTRL,  KEY,            toggleview, {.ui = 1 << TAG} }, \
    { MODKEY|WLR_MODIFIER_SHIFT, SKEY,           tagandview, {.ui = 1 << TAG} }, \
    { MODKEY|WLR_MODIFIER_CTRL|WLR_MODIFIER_SHIFT,SKEY,toggletag,{.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static void tagandview(const Arg *arg) { tag(arg); view(arg); }

/* commands */
static const char *termcmd[] = { "kitty", NULL };
static const char *menucmd[] = { "fuzzel", NULL };
static const char *browser[] = { "zen-browser", NULL };
static const char *emojisl[] = { "bemoji", NULL };
static const char *wllpapr[] = { "waypaper", NULL };
static const char *lockscr[] = { "swaylock", NULL };
static const char *volshut[] = { "shutdown", "-P", "now", NULL };
static const char *volrest[] = { "systemctl", "reboot", NULL };
static const char *fileman[] = { "nautilus", "--new-window", NULL };

static const Key keys[] = {
	/* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
	/* modifier                  key                 function        argument */
	{ MODKEY,                    XKB_KEY_d,          spawn,          {.v = menucmd} },
	{ MODKEY,                    XKB_KEY_Return,     spawn,          {.v = termcmd} },
	{ MODKEY,                    XKB_KEY_l,          spawn,          {.v = lockscr} },
	{ MODKEY,                    XKB_KEY_q,          killclient,     {0} },
	{ MODKEY,                    XKB_KEY_t,          togglefloating, {0} },
	{ MODKEY,                    XKB_KEY_f,          togglefullscreen, {0} },
	{ MODKEY,                    XKB_KEY_w,          spawn,          {.v = wllpapr} },
	{ MODKEY,                    XKB_KEY_period,     spawn,          {.v = emojisl} },
	{ MODKEY,                    XKB_KEY_e,          spawn,          {.v = fileman} },
	{ MODKEY,                    XKB_KEY_b,          spawn,          {.v = browser} },
	{ MODKEY,                    XF86XK_AudioRaiseVolume,     spawn,          {.v = volshut} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XF86XK_AudioRaiseVolume,     spawn,          {.v = volrest} },
	{ MODKEY|WLR_MODIFIER_SHIFT, XKB_KEY_S,     spawn,          SHCMD("$HOME/.config/sway/scripts/scrsht.sh") },
	{ 0,               XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer -d 5") },
	{ 0,               XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer -i 5") },
	{ 0,               XF86XK_AudioMute,        spawn, SHCMD("pamixer --toggle-mute") },
	{ 0,               XF86XK_MonBrightnessUp,  spawn, SHCMD("brightnessctl set +5%") },
	{ 0,               XF86XK_MonBrightnessDown,spawn, SHCMD("brightnessctl set 5%-") },
	{ 0,               XF86XK_KbdBrightnessUp,  spawn, SHCMD("brightnessctl -d *::kbd_backlight set 2%+") },
	{ 0,               XF86XK_KbdBrightnessDown,spawn, SHCMD("brightnessctl -d *::kbd_backlight set 2%-") },
	{ 0,               XF86XK_AudioPlay,spawn, SHCMD("playerctl play-pause") },
	{ 0,               XF86XK_AudioNext,spawn, SHCMD("playerctl next") },
	{ 0,               XF86XK_AudioPrev,spawn, SHCMD("playerctl previous") },
	{ MODKEY,               XKB_KEY_v,spawn, SHCMD("cliphist list | fuzzel --dmenu --width 70 | cliphist decode | wl-copy") },
	TAGKEYS(          XKB_KEY_1, XKB_KEY_exclam,                     0),
	TAGKEYS(          XKB_KEY_2, XKB_KEY_apostrophe,                 1),
	TAGKEYS(          XKB_KEY_3, XKB_KEY_asciicircum,                2),
	TAGKEYS(          XKB_KEY_4, XKB_KEY_plus,                       3),
	TAGKEYS(          XKB_KEY_5, XKB_KEY_percent,                    4),
	TAGKEYS(          XKB_KEY_6, XKB_KEY_ampersand,                  5),
	TAGKEYS(          XKB_KEY_7, XKB_KEY_slash,                      6),
	TAGKEYS(          XKB_KEY_8, XKB_KEY_parenleft,                  7),
	TAGKEYS(          XKB_KEY_9, XKB_KEY_parenright,                 8),
	{ MODKEY|WLR_MODIFIER_ALT,   XKB_KEY_q,          quit,           {0} },

	/* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
	{ WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_Terminate_Server, quit, {0} },
	/* Ctrl-Alt-Fx is used to switch to another VT, if you don't know what a VT is
	 * do not remove them.
	 */
#define CHVT(n) { WLR_MODIFIER_CTRL|WLR_MODIFIER_ALT,XKB_KEY_XF86Switch_VT_##n, chvt, {.ui = (n)} }
	CHVT(1), CHVT(2), CHVT(3), CHVT(4), CHVT(5), CHVT(6),
	CHVT(7), CHVT(8), CHVT(9), CHVT(10), CHVT(11), CHVT(12),
};

static const Button buttons[] = {
	{ MODKEY, BTN_LEFT,   moveresize,     {.ui = CurMove} },
	{ MODKEY, BTN_RIGHT,  moveresize,     {.ui = CurResize} },



};
