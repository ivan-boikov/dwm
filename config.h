/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "St"
/* appearance */
static unsigned int borderpx  = 5;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "LiterationMono Nerd Font:size=9:antialias=true:autohint=true" };
static const char dmenufont[]       = "Liberation Mono:size=9";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
    /*               fg           bg           border   */
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "1", "2", "3", "", "", "", "", ""};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /*
                                                             tags mask
                                                             |   is floating
                                                             |   |  is terminal
                                                             |   |  |  no swallow
                                                             |   |  |  |   monitor
    class                   instance    title                |   |  |  |   |        */
    { TERMCLASS,            NULL,       NULL,                0, 0, 1, 0, -1 },
    { NULL,                 NULL,       "Event Tester",      0, 0, 0, 1, -1 }, /* xev */
    { "Zathura",            NULL,       NULL,                0, 0, 0, 1, -1 },

    //{ "filebrowser",        NULL,       NULL,       	1 << 1, 0, 1, 0, -1 },
    { "gksqt",              NULL,       NULL,       	1 << 3, 0, 1, 0, -1 },

    { "Steam",              NULL,       NULL,       	1 << 4, 0, 0, 0, -1 },
    { "Steam",              NULL,       "Friends",      1 << 4, 0, 1, 0, -1 },
    { "Steam",              NULL,       "Steam - News", 1 << 4, 0, 1, 0, -1 },
    { "Steam",              NULL,       ".* - Chat",    1 << 4, 0, 1, 0, -1 },

    { "Gimp",               NULL,       NULL,       	1 << 4, 0, 0, 0, -1 },
    //{ "musicplayer",        NULL,       NULL,           1 << 5, 0, 0, 0, -1 },
    { "Pulseeffects",       NULL,       NULL,       	1 << 5, 0, 0, 0, -1 },
    { "easyeffects",        NULL,       NULL,       	1 << 5, 0, 0, 0, -1 },
    { "vlc",                NULL,       NULL,       	1 << 5, 0, 0, 0, -1 },
    { "Virt-manager",       NULL,       NULL,       	1 << 6, 0, 0, 0, -1 },
    { "Firefox",            NULL,       NULL,       	1 << 7, 0, 0, 0, -1 },
    { "firefox",            NULL,       NULL,       	1 << 7, 0, 0, 0, -1 },
    { "Chromium",           NULL,       NULL,       	1 << 7, 0, 0, 0, -1 },
    { "ProtonMail Bridge",  NULL,       NULL,       	1 << 8, 0, 0, 0, -1 },
    //{ "emailclient",        NULL,       NULL,       	1 << 8, 0, 0, 0, -1 },
    { "thunderbird",        NULL,       NULL,       	1 << 8, 0, 0, 0, -1 },
    { "Signal",             NULL,       NULL,       	1 << 9, 0, 0, 0, -1 },
    { "TelegramDesktop",    NULL,       NULL,       	1 << 9, 0, 0, 0, -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "TTT",      bstack },

    { "[@]",      spiral },
    { "[\\]",     dwindle },

    { "H[]",      deck },
    { "[M]",      monocle },

    //{ "===",      bstackhoriz },
    //{ "HHH",      grid },
    //{ "###",      nrowgrid },
    //{ "---",      horizgrid },
    //{ ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "color0",                 STRING, 	&normbordercolor },
    { "color4",                 STRING, 	&selbordercolor },
    { "color0",                 STRING, 	&normbgcolor },
    { "color4",                 STRING, 	&normfgcolor },
    { "color4",                 STRING, 	&selbgcolor },
    { "color0",                 STRING, 	&selfgcolor },
    { "borderpx",               INTEGER,        &borderpx },
    { "snap",                   INTEGER,        &snap },
    { "showbar",                INTEGER,        &showbar },
    { "topbar",                 INTEGER,        &topbar },
    { "nmaster",                INTEGER,        &nmaster },
    { "resizehints",            INTEGER,        &resizehints },
    { "mfact",      	        FLOAT,          &mfact },
    { "gappih",                 INTEGER,        &gappih },
    { "gappiv",                 INTEGER,        &gappiv },
    { "gappoh",                 INTEGER,        &gappoh },
    { "gappov",                 INTEGER,        &gappov },
    { "swallowfloating",        INTEGER,        &swallowfloating },
    { "smartgaps",		INTEGER,        &smartgaps },
};


/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }
#define STACKKEYS(MOD,ACTION) \
{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
{ MOD, XK_v,     ACTION##stack, {.i = 0 } }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },

    // tile controls
    { MODKEY,                       XK_q,      killclient,     {0} },
    //{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    //{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    STACKKEYS(MODKEY,           focus),
    STACKKEYS(MODKEY|ShiftMask, push),

    { MODKEY,             XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,             XK_l,      setmfact,       {.f = +0.05} },
    //{ MODKEY,             XK_i,      incnmaster,     {.i = +1 } },
    //{ MODKEY,             XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,             XK_space,  zoom,           {0} },
    { MODKEY|ShiftMask,   XK_space,  togglefloating, {0} },
    { MODKEY,             XK_f,      togglefullscr,  {0} },
    //{ MODKEY,             XK_0,      view,           {.ui = ~0 } },
    //{ MODKEY|ShiftMask,   XK_0,      tag,            {.ui = ~0 } },

    { MODKEY|ShiftMask, XK_o,      incrgaps,       {.i = -1 } },
    { MODKEY,           XK_o,      incrgaps,       {.i = +1 } },
    /*
       { MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
       { MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
       { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
       { MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
       { MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
       { MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
       { MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
       */
    { MODKEY,              XK_g,      togglegaps,     {0} },
    { MODKEY|ShiftMask,    XK_g,      defaultgaps,    {0} },

    { MODKEY, XK_Tab,    view,           {0} },


    // layout control
    //{ MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY,           XK_t, setlayout, {.v = &layouts[0]} },
    { MODKEY|ShiftMask, XK_t, setlayout, {.v = &layouts[1]} },
    { MODKEY,           XK_y, setlayout, {.v = &layouts[2]} },
    { MODKEY|ShiftMask, XK_y, setlayout, {.v = &layouts[3]} },
    { MODKEY,           XK_u, setlayout, {.v = &layouts[4]} },
    { MODKEY|ShiftMask, XK_u, setlayout, {.v = &layouts[5]} },
    { MODKEY,           XK_i, setlayout, {.v = &layouts[6]} },
    { MODKEY|ShiftMask, XK_i, setlayout, {.v = &layouts[7]} },


    // monitor control
    { MODKEY,           XK_Left,  focusmon, {.i = -1 } },
    { MODKEY,           XK_Right, focusmon, {.i = +1 } },
    { MODKEY|ShiftMask, XK_Left,  tagmon,   {.i = -1 } },
    { MODKEY|ShiftMask, XK_Right, tagmon,   {.i = +1 } },

    TAGKEYS(XK_1, 0),
    TAGKEYS(XK_2, 1),
    TAGKEYS(XK_3, 2),
    TAGKEYS(XK_4, 3),
    TAGKEYS(XK_5, 4),
    TAGKEYS(XK_6, 5),
    TAGKEYS(XK_7, 6),
    TAGKEYS(XK_8, 7),
    TAGKEYS(XK_9, 8),
    TAGKEYS(XK_0, 9),

    /*{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
      { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },*/

    // media
    { MODKEY,           XK_minus, spawn, SHCMD("pamixer --allow-boost -d 5;  kill -44 $(pidof dwmblocks)") },
    { MODKEY|ShiftMask,	XK_minus, spawn, SHCMD("pamixer --allow-boost -d 25; kill -44 $(pidof dwmblocks)") },
    { MODKEY,           XK_equal, spawn, SHCMD("pamixer --allow-boost -i 5;  kill -44 $(pidof dwmblocks)") },
    { MODKEY|ShiftMask,	XK_equal, spawn, SHCMD("pamixer --allow-boost -i 25; kill -44 $(pidof dwmblocks)") },


    { MODKEY,		XK_p,		 spawn,	SHCMD("playerctl play-pause ; mpc toggle") },
    { MODKEY|ShiftMask, XK_p,		 spawn,	SHCMD("playerctl pause ; mpc pause ; $HOME/.local/bin/pauseallmpv") },
    { MODKEY,		XK_bracketleft,	 spawn,	SHCMD("mpc seek -5") },
    { MODKEY|ShiftMask,	XK_bracketleft,	 spawn,	SHCMD("mpc seek -30") },
    { MODKEY,		XK_bracketright, spawn,	SHCMD("mpc seek +5") },
    { MODKEY|ShiftMask,	XK_bracketright, spawn,	SHCMD("mpc seek +30") },


    { 0, XF86XK_AudioMute,	  spawn, SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume, spawn, SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioPrev,	  spawn, SHCMD("playerctl previous ; mpc prev") },
    { 0, XF86XK_AudioNext,	  spawn, SHCMD("playerctl next ; mpc next") },
    { 0, XF86XK_AudioPause,	  spawn, SHCMD("playerctl pause ; mpc pause") },
    { 0, XF86XK_AudioPlay,	  spawn, SHCMD("playerctl play-pause ; mpc toggle") },
    { 0, XF86XK_AudioStop,	  spawn, SHCMD("playerctl stop ; mpc stop") },


    { MODKEY, XK_F1, spawn, SHCMD("$HOME/.local/bin/dpass --type") },
    { MODKEY, XK_F2, spawn, SHCMD("$HOME/.local/bin/dmnt") },
    { MODKEY, XK_F3, spawn, SHCMD("$HOME/.local/bin/remaps & notify-send \\\"Keyboard remapping...\\\" \\\"Re-running keyboard defaults for any newly plugged-in keyboards.\\\"") },
    { MODKEY, XK_F5, spawn, SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
    { MODKEY, XK_F8, spawn, SHCMD("$HOME/.local/bin/displayselect") },
    { MODKEY, XK_F9, spawn, SHCMD("$HOME/.local/bin/chsink") },


    { 0, 	        XK_Print,  spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png && notify-send 'Screenshot taken'") },
    { ShiftMask,        XK_Print,  spawn, SHCMD("$HOME/.local/bin/maimpick") },
    { MODKEY,	        XK_Print,  spawn, SHCMD("$HOME/.local/bin/drecord") },
    { MODKEY|ShiftMask,	XK_Print,  spawn, SHCMD("$HOME/.local/bin/drecord kill") },
    { MODKEY,		XK_Delete, spawn, SHCMD("$HOME/.local/bin/drecord kill") },


    // system utilities
    { MODKEY, XK_s, spawn, SHCMD("$HOME/.local/bin/sysact") },
    { MODKEY, XK_r, spawn, SHCMD(TERMINAL " -e htop") },
    { MODKEY, XK_e, spawn, SHCMD(TERMINAL " -c filebrowser -e nnn -d") },
    { MODKEY, XK_w, spawn, SHCMD(TERMINAL " -e nmtui") },
    { MODKEY, XK_m, spawn, SHCMD(TERMINAL " -c email -e neomutt") },
    { MODKEY, XK_n, spawn, SHCMD(TERMINAL " -c musicplayer -e ncmpcpp") },
    //{ MODKEY, XK_m, spawn, SHCMD("if [ -z $(pidof mpd) ]; then notify-send 'Starting mpd'; mpd &; fi; st -c musicplayer -e ncmpcpp") },
    { MODKEY|ShiftMask, XK_n, spawn, SHCMD(TERMINAL " -c mixer -e ncpamixer") },
    { MODKEY, XK_x, spawn, SHCMD("$HOME/.local/bin/dkill") },


    { 0, XF86XK_MonBrightnessUp,   spawn, SHCMD("xbacklight -inc 10 ; brightnessctl set 10%+") },
    { 0, XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 10 ; brightnessctl set 10%-") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
