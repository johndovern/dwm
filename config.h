/* See test LICENSE file for copyright and license details. */
/* testing */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define STEAM "Steam"

/* appearance */
static unsigned int borderpx  = 4;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 10;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = {
            "monospace:size=12",
            "M+ 1c:pixelsize=18:antialias=true:autohint=true",
            "JoyPixels:pixelsize=14:antialias=true:autohint=true"
};
static char normbgcolor[]           = "#212121";
static char normbordercolor[]       = "#212121";
static char normfgcolor[]           = "#eeeeee";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#f44336";
static char selbgcolor[]            = "#212121";
static char *colors[][3] = {
            /*               fg           bg           border   */
            [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
            [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
            const char *name;
            const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
            /* name          cmd  */
            {"spterm",      spcmd1},
            {"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
            /* xprop(1):
             *	WM_CLASS(STRING) = instance, class
             *	WM_NAME(STRING) = title
            */
            /* class              instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
            { TERMCLASS,            NULL,       NULL,             0,            0,          1,          0,        -1 },
            { TERMCLASS,            "tmux",     NULL,             0,            0,          1,          0,        -1 },
            { NULL,                 NULL,       "Event Tester",   0,            0,          0,          1,        -1 },
            { "mpv",                "fmpv",     NULL,             0,            1,          0,          0,        -1 },
            { NULL,                 "spterm",   NULL,             SPTAG(0),     1,          1,          0,        -1 },
            { NULL,                 "spcalc",   NULL,             SPTAG(1),     1,          1,          0,        -1 },
            { TERMCLASS,            TERMINAL,   "pulsemixer",     0,            1,          1,          0,        -1 },
            { STEAM,                STEAM,      STEAM,            1 << 1,       0,          0,          0,        -1 },
            { STEAM,                STEAM,      "Friends List",   1 << 1,       1,          0,          0,        -1 },
            { TERMCLASS,            "anime_lf", NULL,             1 << 2,       0,          1,          0,        -1 },
            { TERMCLASS,            "aniwiki",  NULL,             1 << 2,       0,          1,          0,        -1 },
            { "Lutris",             NULL,       NULL,             1 << 3,       0,          0,          0,        -1 },
            { "fate.exe",           NULL,       NULL,             1 << 4,       1,          0,          0,        -1 },
            { "Transmission-gtk",   NULL,       NULL,             1 << 5,       0,          0,          0,        -1 },
            { "kdenlive",           NULL,       NULL,             1 << 6,       0,          0,          0,        -1 },
            { "Gimp",               NULL,       NULL,             1 << 7,       0,          0,          0,        -1 },
            { "KeePassXC",          NULL,       NULL,             1 << 8,       0,          0,          0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
            /* symbol     arrange function */
            { "[@]",      spiral },   /* Default: Fibonacci spiral */
            { "[]=",      tile },     /* Master on left, slaves on right */
            { "TTT",      bstack },   /* Master on top, slaves on bottom */
            { "[\\]",     dwindle },  /* Decreasing in size right and leftward */
            { "[D]",      deck },     /* Master on left, slaves in monocle-like mode on right */
            { "[M]",      monocle },  /* All windows on top of eachother */
            { "|M|",      centeredmaster },   /* Master in middle, slaves on sides */
            { ">M>",      centeredfloatingmaster },   /* Same but master floats */
            { "><>",      NULL },     /* no layout function means floating behavior */
            { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define MEHKEY (Mod1Mask|ControlMask|ShiftMask)
#define TAGKEYS(KEY,TAG) \
            { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
            { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
            { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
            { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
            { MEHKEY,                       KEY,      focusnthmon,    {.i  = TAG } }, \
            { MEHKEY|MODKEY,                KEY,      tagnthmon,      {.i  = TAG } },

#define STACKKEYS(MOD,ACTION) \
            { MOD,    XK_j,     ACTION##stack,    {.i = INC(+1) } }, \
            { MOD,    XK_k,     ACTION##stack,    {.i = INC(-1) } }, \
            { MOD,    XK_f,     ACTION##stack,    {.i = 0 } }, \
            /* { MOD,    XK_grave, ACTION##stack,    {.i = PREVSEL } }, \ */
            /* { MOD,    XK_a,     ACTION##stack,    {.i = 1 } }, \ */
            /* { MOD,    XK_z,     ACTION##stack,    {.i = 2 } }, \ */
            /* { MOD,    XK_x,     ACTION##stack,    {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, "-n", "tmux", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
            { "colornbg",		      STRING,     &normbgcolor },
            { "colornbord",       STRING,     &normbordercolor },
            { "colornfg",         STRING,     &normfgcolor },
            { "colorselfg",       STRING,     &selfgcolor },
            { "colorselbord",     STRING,     &selbordercolor },
            { "colorselbg",       STRING,     &selbgcolor },
            { "borderpx",         INTEGER,    &borderpx },
            { "snap",             INTEGER,    &snap },
            { "showbar",          INTEGER,    &showbar },
            { "topbar",           INTEGER,    &topbar },
            { "nmaster",          INTEGER,    &nmaster },
            { "resizehints",      INTEGER,    &resizehints },
            { "mfact",            FLOAT,      &mfact },
            { "gappih",           INTEGER,    &gappih },
            { "gappiv",           INTEGER,    &gappiv },
            { "gappoh",           INTEGER,    &gappoh },
            { "gappov",           INTEGER,    &gappov },
            { "swallowfloating",  INTEGER,    &swallowfloating },
            { "smartgaps",        INTEGER,    &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
            /* modifier     key             function        argument */
            /*                              ROW 1                           */
            STACKKEYS(MODKEY,               focus)
            STACKKEYS(MODKEY|ShiftMask,     push)
            /* { MODKEY|ShiftMask, XK_Escape,  spawn,          SHCMD("") }, */
            /* { MODKEY, XK_grave,             spawn,          SHCMD("dmenuunicode") }, */
            /* { MODKEY|ShiftMask, XK_grave,   togglescratch,  SHCMD("") }, */
            TAGKEYS(                        XK_1,           0)
            TAGKEYS(                        XK_2,           1)
            TAGKEYS(                        XK_3,           2)
            TAGKEYS(                        XK_4,           3)
            TAGKEYS(                        XK_5,           4)
            TAGKEYS(                        XK_6,           5)
            TAGKEYS(                        XK_7,           6)
            TAGKEYS(                        XK_8,           7)
            TAGKEYS(                        XK_9,           8)
            { MODKEY,                       XK_0,           view,   {.ui = ~0 } },
            { MODKEY|ShiftMask,             XK_0,           tag,    {.ui = ~0 } },

            /*                              ROW 2                           */
            { MODKEY, XK_Tab,               view,           {0} },
            /* { MODKEY|ShiftMask, XK_Tab,     spawn,          SHCMD("") }, */
            /* { MODKEY, XK_q,                 spawn,          SHCMD("dwm-quit") }, */
            { MODKEY, XK_q,                 killclient,     {0} },
            /* { MODKEY|ShiftMask, XK_q,       killclient,     {0} }, */
            /* { MODKEY|ShiftMask,  XK_q,      spawn,          SHCMD("sysact") }, */
            /* { MODKEY, XK_w,                 spawn,          SHCMD("$BROWSER") }, */
            /* { MODKEY|ShiftMask, XK_w,       spawn,          SHCMD(TERMINAL " -e sudo nmtui") }, */
            /* { MODKEY, XK_e,                 spawn,          SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") }, */
            /* { MODKEY|ShiftMask, XK_e,       spawn,          SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") }, */
            /* { MODKEY, XK_r,                 spawn,          SHCMD(TERMINAL " -n lf_run -e lfrun") }, */
            /* { MODKEY|ShiftMask, XK_r,       spawn,          SHCMD(TERMINAL " -e htop") }, */
            { MODKEY, XK_t,                 setlayout,      {.v = &layouts[2]} }, /* bstack */
            { MODKEY|ShiftMask, XK_t,       setlayout,      {.v = &layouts[1]} }, /* tile */
            { MODKEY, XK_y,                 setlayout,      {.v = &layouts[0]} }, /* spiral */
            { MODKEY|ShiftMask, XK_y,       setlayout,      {.v = &layouts[3]} }, /* dwindle */
            { MODKEY, XK_u,                 setlayout,      {.v = &layouts[4]} }, /* deck */
            { MODKEY|ShiftMask, XK_u,       setlayout,      {.v = &layouts[6]} }, /* centeredmaster */
            { MODKEY, XK_i,                 setlayout,      {.v = &layouts[5]} }, /* monocle */
            { MODKEY|ShiftMask, XK_i,       setlayout,      {.v = &layouts[7]} }, /* centeredfloatingmaster */
            { MODKEY, XK_o,                 incnmaster,     {.i = +1 } },
            { MODKEY|ShiftMask, XK_o,       incnmaster,     {.i = -1 } },
            /* { MODKEY, XK_p,                 spawn,          SHCMD("mpc toggle") }, */
            /* { MODKEY|ShiftMask, XK_p,       spawn,          SHCMD("mpc pause ; pauseallmpv") }, */
            /* { MODKEY, XK_bracketleft,       spawn,          SHCMD("mpc seek -10") }, */
            /* { MODKEY|ShiftMask, XK_bracketleft, spawn,      SHCMD("mpc seek -60") }, */
            /* { MODKEY, XK_bracketright,      spawn,          SHCMD("mpc seek +10") }, */
            /* { MODKEY|ShiftMask, XK_bracketright, spawn,     SHCMD("mpc seek +60") }, */
            { MODKEY, XK_backslash,         view,           {0} },
            /* { MODKEY|ShiftMask, XK_backslash, spawn,        SHCMD("") }, */

            /*                              ROW 3                     			*/
            { MODKEY, XK_a,                 togglegaps,     {0} },
            { MODKEY|ShiftMask, XK_a,       defaultgaps,    {0} },
            { MODKEY, XK_s,                 togglesticky,   {0} },
            /* { MODKEY|ShiftMask, XK_s,       spawn,          SHCMD("") }, */
            /* { MODKEY, XK_d,                 spawn,          SHCMD("dmenu_run") }, */
            /* { MODKEY|ShiftMask, XK_d,       spawn,          SHCMD("passmenu") }, */
            /*          F is automatically bound above in STACKKEYS         */
            { MODKEY|ShiftMask, XK_f,       setlayout,      {.v = &layouts[8]} },
            { MODKEY, XK_g,                 setmfact,       {.f = -0.05} },
            /* { MODKEY, XK_g,                 shiftview,      { .i = -1 } }, */
            /* { MODKEY|ShiftMask, XK_g,       shifttag,       { .i = -1 } }, */
            /* { MODKEY, XK_h,                 setmfact,       {.f = -0.05} }, */
            { MODKEY, XK_h,                 shiftview,      { .i = -1 } },
            { MODKEY|ShiftMask, XK_h,       shifttag,       { .i = -1 } },
            /*       J and K are automatically bound above in STACKEYS      */
            { MODKEY, XK_l,                 shiftview,      { .i = 1 } },
            { MODKEY|ShiftMask, XK_l,       shifttag,       { .i = 1 } },
            /* { MODKEY, XK_l,                 setmfact,       {.f = +0.05} }, */
            /* { MODKEY, XK_semicolon,         shiftview,      { .i = 1 } }, */
            /* { MODKEY|ShiftMask, XK_semicolon, shifttag,     { .i = 1 } }, */
            { MODKEY, XK_semicolon,         setmfact,       {.f = +0.05} },
            { MODKEY, XK_apostrophe,        togglescratch,  {.ui = 1} },
            /* { MODKEY|ShiftMask, XK_apostrophe,  spawn,      SHCMD("") }, */
            { MODKEY, XK_Return,            spawn,          {.v = termcmd } },
            { MODKEY|ShiftMask, XK_Return,  togglescratch,  {.ui = 0} },

            /*                              ROW 4                           */
            { MODKEY, XK_z,                 incrgaps,       {.i = +3 } },
            /* { MODKEY|ShiftMask, XK_z,       spawn,          SHCMD("") }, */
            { MODKEY, XK_x,                 incrgaps,       {.i = -3 } },
            /* { MODKEY|ShiftMask, XK_x,       spawn,          SHCMD("") }, */
            /* { MODKEY, XK_c,                 spawn,          SHCMD("kill $(pidof sxhkd) ; sxhkd") }, */
            /* { MODKEY|ShiftMask, XK_c,       spawn,          SHCMD("") }, */
            { MODKEY, XK_v,                 togglefullscr,  {0} },
            /* { MODKEY|ShiftMask, XK_v,       spawn,          SHCMD("") }, */
            { MODKEY, XK_b,                 togglebar,      {0} },
            /* { MODKEY|ShiftMask, XK_b,       spawn,          SHCMD("") }, */
            /* { MODKEY, XK_n,                 spawn,          SHCMD(TERMINAL " -e nvim -c VimwikiIndex") }, */
            /* { MODKEY|ShiftMask, XK_n,       spawn,          SHCMD(TERMINAL " -e newsboat; pkill -RTMIN+6 dwmblocks") }, */
            /* { MODKEY, XK_m,                 spawn,          SHCMD(TERMINAL " -e ncmpcpp") }, */
            /* { MODKEY|ShiftMask, XK_m,       spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") }, */
            /* { MODKEY, XK_comma,             spawn,          SHCMD("mpc prev") }, */
            /* { MODKEY|ShiftMask, XK_comma,   spawn,          SHCMD("mpc seek 0%") }, */
            /* { MODKEY, XK_period,            spawn,          SHCMD("mpc next") }, */
            /* { MODKEY|ShiftMask, XK_period,  spawn,          SHCMD("mpc repeat") }, */

            { MODKEY, XK_Left,              focusmon,       {.i = -1 } },
            { MODKEY|ShiftMask, XK_Left,    tagmon,         {.i = -1 } },
            { MODKEY, XK_Right,             focusmon,       {.i = +1 } },
            { MODKEY|ShiftMask, XK_Right,   tagmon,         {.i = +1 } },

            { MODKEY, XK_Page_Up,           shiftview,      { .i = -1 } },
            { MODKEY|ShiftMask, XK_Page_Up, shifttag,       { .i = -1 } },
            { MODKEY, XK_Page_Down,         shiftview,      { .i = +1 } },
            { MODKEY|ShiftMask, XK_Page_Down, shifttag,     { .i = +1 } },
            /* { MODKEY, XK_Insert,            spawn,          SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") }, */

            /* { MODKEY, XK_F1,                spawn,          SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") }, */
            /* { MODKEY, XK_F2,                spawn,          SHCMD("tutorialvids") }, */
            /* { MODKEY, XK_F3,                spawn,          SHCMD("displayselect") }, */
            /* { MODKEY, XK_F4,                spawn,          SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") }, */
            /* /1* { MODKEY, XK_F5,                spawn,          SHCMD(""), *1/ */
            /* { MODKEY, XK_F6,                spawn,          SHCMD("torwrap") }, */
            /* { MODKEY, XK_F7,                spawn,          SHCMD("td-toggle") }, */
            /* { MODKEY, XK_F8,                spawn,          SHCMD("mw -Y") }, */
            /* { MODKEY, XK_F9,                spawn,          SHCMD("dmenumount") }, */
            /* { MODKEY, XK_F10,               spawn,          SHCMD("dmenuumount") }, */
            /* { MODKEY, XK_F11,               spawn,          SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") }, */
            /* { MODKEY, XK_F12,               spawn,          SHCMD("remaps & notify-send \\\"⌨️ Keyboard remapping...\\\" \\\"Re-running keyboard defaults for any newly plugged-in keyboards.\\\"") }, */
            { MODKEY, XK_space,             zoom,           {0} },
            { MODKEY|ShiftMask, XK_space,   togglefloating, {0} },

            /* { 0,  XK_Print,                 spawn,          SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") }, */
            /* { ShiftMask,  XK_Print,         spawn,          SHCMD("maimpick") }, */
            /* { MODKEY, XK_Print,             spawn,          SHCMD("dmenurecord") }, */
            /* { MODKEY|ShiftMask, XK_Print,   spawn,          SHCMD("dmenurecord kill") }, */
            /* { MODKEY, XK_Delete,            spawn,          SHCMD("dmenurecord kill") }, */
            /* { MODKEY, XK_Scroll_Lock,       spawn,          SHCMD("killall screenkey || screenkey &") }, */

            /*                              MY KEYS                         */

            /*                              ROW 1                           */
            /* { MEHKEY, XK_1,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_1,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_2,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_2,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_3,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_3,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_4,                 spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_4,                 spawn,          SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") }, */
            /* { MEHKEY|MODKEY, XK_4,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_5,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_5,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_6,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_6,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_7,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_7,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_8,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_8,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_9,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_9,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_0,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_0,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_minus,             spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_minus,      spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_equal,             spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_equal,      spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_BackSpace,         spawn,          SHCMD("groff -mom /home/anon/.local/src/dwm/keys.mom -Tpdf | zathura -") }, */
            /* { MEHKEY|MODKEY, XK_BackSpace,  spawn,          SHCMD("") }, */

            /*                              ROW 2                           */
            /* { MEHKEY, XK_q,                 spawn,          SHCMD("mpv-options quit") }, */
            /* { MEHKEY|MODKEY, XK_q,          spawn,          SHCMD(TERMINAL " -n keepass -e keepasshide ; sleep 1.5 ; st -e total-shutdown") }, */
            /* { MEHKEY, XK_w,                 spawn,          SHCMD("aniwiki") }, */
            /* { MEHKEY|MODKEY, XK_w,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_e,                 spawn,          SHCMD("mpv-options focus") }, */
            /* { MEHKEY|MODKEY, XK_e,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_r,                 spawn,          SHCMD("dautoupdate") }, */
            /* { MEHKEY|MODKEY, XK_r,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_t,                 spawn,          SHCMD(TERMINAL " -e nvim -c VimwikiIndex -c 'VimwikiGoto temp-notes'") }, */
            /* { MEHKEY|MODKEY, XK_t,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_y,                 spawn,          SHCMD("transmission-gtk") }, */
            /* { MEHKEY|MODKEY, XK_y,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_u,                 spawn,          SHCMD("librewolf") }, */
            /* { MEHKEY|MODKEY, XK_u,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_i,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_i,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_o,                 spawn,          SHCMD("mpv $HOME/.config/mpv/playlists/groovy.m3u") }, */
            /* { MEHKEY|MODKEY, XK_o,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_p,                 spawn,          SHCMD("mpv-play-pause") }, */
            /* { MEHKEY|MODKEY, XK_p,          spawn,          SHCMD("") }, */

            /*                              ROW 3                           */
            /* { MEHKEY, XK_a,                 spawn,          SHCMD(TERMINAL " -n anime_lf -e lfrun ~/videos/anime/") }, */
            /* { MEHKEY|MODKEY, XK_a,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_s,                 spawn,          SHCMD("maim -s ~/screenshot-$(date '+%y%m%d-%H%M-%S').png && xdotool mousemove --sync 0 300") }, */
            /* { MEHKEY|MODKEY,  XK_s,         spawn,          SHCMD("mpv-options sticky") }, */
            /* { MEHKEY, XK_d,                 spawn,          SHCMD("sleep 0.5 && mpv-options float") }, */
            /* { MEHKEY|MODKEY, XK_d,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_f,                 spawn,          SHCMD("mpv-options full") }, */
            /* { MEHKEY|MODKEY, XK_f,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_g,                 spawn,          SHCMD("mpv $HOME/.config/mpv/playlists/groove.m3u") }, */
            /* { MEHKEY|MODKEY, XK_g,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_h,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_h,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_j,                 spawn,          SHCMD("xdotool mousemove --sync 50 300 && xdotool mousemove --sync 0 300") }, */
            /* { MEHKEY|MODKEY, XK_j,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_k,                 spawn,          SHCMD("xdotool mousemove --sync 3000 300") }, */
            /* { MEHKEY|MODKEY, XK_k,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_l,                 spawn,          SHCMD(TERMINAL " -n keepass -e keepasshide") }, */
            /* { MEHKEY|MODKEY, XK_l,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_semicolon,         spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_semicolon,  spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_apostrophe,        spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_apostrophe, spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_Return,            spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_Return,     spawn,          SHCMD("") }, */

            /*                              ROW 4                           */
            /* { MEHKEY, XK_z,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_z,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_x,                 spawn,          SHCMD("remaps") }, */
            /* { MEHKEY|MODKEY, XK_x,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_c,                 spawn,          SHCMD("dmenubluetooth reconnect") }, */
            /* { MEHKEY|MODKEY, XK_c,          spawn,          SHCMD("dmenubluetooth turn-on") }, */
            /* { MEHKEY, XK_v,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_v,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_b,                 spawn,          SHCMD("dmenubluetooth disconnect") }, */
            /* { MEHKEY|MODKEY, XK_b,          spawn,          SHCMD("dmenubluetooth opts") }, */
            /* { MEHKEY, XK_space,             spawn,          SHCMD("touchpadtoggle") }, */
            /* { MEHKEY|MODKEY, XK_space,      spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_n,                 spawn,          SHCMD("") }, */
            /* { MEHKEY|MODKEY, XK_n,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_m,                 spawn,          SHCMD(TERMINAL " -n manga_lf -e lfrun ~/documents/manga/") }, */
            /* { MEHKEY|MODKEY, XK_m,          spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_comma,             spawn,          SHCMD("mpv-prev-song") }, */
            /* { MEHKEY|MODKEY, XK_comma,      spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_period,            spawn,          SHCMD("mpv-next-song") }, */
            /* { MEHKEY|MODKEY, XK_period,     spawn,          SHCMD("") }, */
            /* { MEHKEY, XK_Print,             spawn,          SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") }, */

            /*                              XF86 KEYS                       */
            { 0, XF86XK_AudioMute,          spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
            { 0, XF86XK_AudioRaiseVolume,   spawn,          SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
            { 0, XF86XK_AudioLowerVolume,   spawn,          SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
            { 0, XF86XK_AudioPrev,          spawn,          SHCMD("mpc prev") },
            { 0, XF86XK_AudioNext,          spawn,          SHCMD("mpc next") },
            { 0, XF86XK_AudioPause,         spawn,          SHCMD("mpc pause") },
            { 0, XF86XK_AudioPlay,          spawn,          SHCMD("mpc play") },
            { 0, XF86XK_AudioStop,          spawn,          SHCMD("mpc stop") },
            { 0, XF86XK_AudioRewind,        spawn,          SHCMD("mpc seek -10") },
            { 0, XF86XK_AudioForward,       spawn,          SHCMD("mpc seek +10") },
            { 0, XF86XK_AudioMedia,         spawn,          SHCMD(TERMINAL " -e ncmpcpp") },
            { 0, XF86XK_AudioMicMute,       spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
            { 0, XF86XK_PowerOff,           spawn,          SHCMD("sysact") },
            { 0, XF86XK_Calculator,         spawn,          SHCMD(TERMINAL " -e bc -l") },
            { 0, XF86XK_Sleep,              spawn,          SHCMD("sudo -A zzz") },
            { 0, XF86XK_WWW,                spawn,          SHCMD("$BROWSER") },
            { 0, XF86XK_DOS,                spawn,          SHCMD(TERMINAL) },
            { 0, XF86XK_ScreenSaver,        spawn,          SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
            { 0, XF86XK_TaskPane,           spawn,          SHCMD(TERMINAL " -e htop") },
            { 0, XF86XK_Mail,               spawn,          SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
            { 0, XF86XK_MyComputer,         spawn,          SHCMD(TERMINAL " -e lf /") },
            /* { 0, XF86XK_Battery,            spawn,          SHCMD("") }, */
            { 0, XF86XK_Launch1,            spawn,          SHCMD("xset dpms force off") },
            { 0, XF86XK_TouchpadToggle,     spawn,          SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
            { 0, XF86XK_TouchpadOff,        spawn,          SHCMD("synclient TouchpadOff=1") },
            { 0, XF86XK_TouchpadOn,         spawn,          SHCMD("synclient TouchpadOff=0") },
            { 0, XF86XK_MonBrightnessUp,    spawn,          SHCMD("xbacklight -inc 10") },
            { 0, XF86XK_MonBrightnessDown,  spawn,          SHCMD("xbacklight -dec 10") },
            /* { 0, XF86XK_MonBrightnessUp,    spawn,          SHCMD("light -A 5") }, */
            /* { 0, XF86XK_MonBrightnessDown,  spawn,          SHCMD("light -U 5") }, */

            /* { MODKEY|Mod4Mask,              XK_h, incrgaps,     {.i = +1 } }, */
            /* { MODKEY|Mod4Mask,              XK_l, incrgaps,     {.i = -1 } }, */
            /* { MODKEY|Mod4Mask|ShiftMask,    XK_h, incrogaps,    {.i = +1 } }, */
            /* { MODKEY|Mod4Mask|ShiftMask,    XK_l, incrogaps,    {.i = -1 } }, */
            /* { MODKEY|Mod4Mask|ControlMask,  XK_h, incrigaps,    {.i = +1 } }, */
            /* { MODKEY|Mod4Mask|ControlMask,  XK_l, incrigaps,    {.i = -1 } }, */
            /* { MODKEY|Mod4Mask|ShiftMask,    XK_0, defaultgaps,  {0} }, */
            /* { MODKEY,                       XK_y, incrihgaps,   {.i = +1 } }, */
            /* { MODKEY,                       XK_o, incrihgaps,   {.i = -1 } }, */
            /* { MODKEY|ControlMask,           XK_y, incrivgaps,   {.i = +1 } }, */
            /* { MODKEY|ControlMask,           XK_o, incrivgaps,   {.i = -1 } }, */
            /* { MODKEY|Mod4Mask,              XK_y, incrohgaps,   {.i = +1 } }, */
            /* { MODKEY|Mod4Mask,              XK_o, incrohgaps,   {.i = -1 } }, */
            /* { MODKEY|ShiftMask,             XK_y, incrovgaps,   {.i = +1 } }, */
            /* { MODKEY|ShiftMask,             XK_o, incrovgaps,   {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
            /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
            { ClkWinTitle,          0,              Button3,        zoom,           {0} },
            { ClkWinTitle,          0,              Button4,        spawn,          SHCMD("xdotool key 'super+k'") },
            { ClkWinTitle,          0,              Button5,        spawn,          SHCMD("xdotool key 'super+j'") },
            { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
            { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
            { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
            { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
            { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
            { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
            { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
            { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
            { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
            { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
            { ClkClientWin,         MODKEY,         Button4,        incrgaps,       {.i = +1} },
            { ClkClientWin,         MODKEY,         Button5,        incrgaps,       {.i = -1} },
            { ClkTagBar,            0,              Button1,        view,           {0} },
            { ClkTagBar,            0,              Button3,        toggleview,     {0} },
            { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
            { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
            { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
            { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1} },
            { ClkRootWin,           0,              Button2,        togglebar,      {0} },
            { ClkLtSymbol,          0,              Button1,        setlayout,      {.v = &layouts[0]} }, /* spiral */
            { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[5]} }, /* monocle */
};

