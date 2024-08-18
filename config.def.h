/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "Fira Code Retina:size=9", "Font Awesome 6 Free Solid:size=9", "Font Awesome 5 Free Solid:size=9", "Mona Gothic Wide:size=9" };
static const char dmenufont[]       = "Fira Code Retina:size=9";
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
static const char *tags[] = { "", "", "1", "2", "3", "", "VM", "", "", ""};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "Zathura",            NULL,       NULL,            0,       0,           -1 },
	{ "Gimp",               NULL,       NULL,       1 << 4,       0,           -1 },
	{ "Virt-manager",       NULL,       NULL,       1 << 6,       0,           -1 },
	{ "Firefox",            NULL,       NULL,       1 << 7,       0,           -1 },
	{ "firefox",            NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Chromium",           NULL,       NULL,       1 << 7,       0,           -1 },
	{ "thunderbird",        NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Signal",             NULL,       NULL,       1 << 9,       0,           -1 },
	{ "TelegramDesktop",    NULL,       NULL,       1 << 9,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
{ MOD, XK_v,     ACTION##stack, {.i = 0 } }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,           focus),
	STACKKEYS(MODKEY|ShiftMask, push),
	{ MODKEY|ControlMask,           XK_h,      setmfact,       {.f = -0.005} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ControlMask,           XK_l,      setmfact,       {.f = +0.005} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrgaps,       {.i = +1 } },
	{ MODKEY,                       XK_o,      defaultgaps,    {0} },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	// layout
	{ MODKEY,                       XK_t,      setlayout, {.v = &layouts[0] } }, // tile
	{ MODKEY|ShiftMask,             XK_t,      setlayout, {.v = &layouts[5] } }, // bstack
	{ MODKEY|ShiftMask|ControlMask, XK_t,      setlayout, {.v = &layouts[6] } }, // bstackhoriz
	{ MODKEY,                       XK_g,      setlayout, {.v = &layouts[7] } }, // grid
	{ MODKEY|ShiftMask,             XK_g,      setlayout, {.v = &layouts[9] } }, // horizgrid
	{ MODKEY,                       XK_u,      setlayout, {.v = &layouts[1] } }, // monocle
	{ MODKEY|ShiftMask,             XK_u,      setlayout, {.v = &layouts[4] } }, // deck
	{ MODKEY,                       XK_i,      setlayout, {.v = &layouts[11]} }, // centeredmaster
	{ MODKEY|ShiftMask,             XK_i,      setlayout, {.v = &layouts[12]} }, // centeredfloatingmaster
	// system
	{ MODKEY,                       XK_F2,                    spawn, SHCMD("$HOME/.local/bin/dmnt") },
	{ MODKEY|ShiftMask,             XK_F2,                    spawn, SHCMD("$HOME/.local/bin/dumnt") },
	{ MODKEY,                       XK_F3,                    spawn, SHCMD("$HOME/.local/bin/remaps") },
	{ MODKEY,                       XK_F8,                    spawn, SHCMD("$HOME/.local/bin/displayselect") },
	{ MODKEY,                       XK_s,                     spawn, SHCMD("$HOME/.local/bin/sysact") },
	{ MODKEY,                       XK_x,                     spawn, SHCMD("$HOME/.local/bin/dkill") },
	{ ControlMask,                  XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 1  ; brightnessctl set 1%-") },
	{ 0,                            XF86XK_MonBrightnessDown, spawn, SHCMD("xbacklight -dec 10 ; brightnessctl set 10%-") },
	{ ControlMask,                  XF86XK_MonBrightnessUp,   spawn, SHCMD("xbacklight -inc 1  ; brightnessctl set 1%+") },
	{ 0,                            XF86XK_MonBrightnessUp,   spawn, SHCMD("xbacklight -inc 10 ; brightnessctl set 10%+") },
	// utilities
	{ MODKEY,                       XK_r,                     spawn, SHCMD("st" " -e htop") },
	{ MODKEY,                       XK_e,                     spawn, SHCMD("st" " -c filebrowser -e nnn -deAU") },
	{ MODKEY,                       XK_w,                     spawn, SHCMD("st" " -e nmtui") },
	{ MODKEY,                       XK_n,                     spawn, SHCMD("st" " -c musicplayer -e $HOME/.local/bin/musicplayer") },
	{ MODKEY,                       XK_m,                     spawn, SHCMD("$HOME/.local/bin/mailclient") },
	{ MODKEY,                       XK_F1,                    spawn, SHCMD("$HOME/.local/bin/dpass --type") },
	{ 0,                            XK_Print,                 spawn, SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png && notify-send 'Screenshot taken'") },
	{ ShiftMask,                    XK_Print,                 spawn, SHCMD("$HOME/.local/bin/maimpick") },
	{ MODKEY,                       XK_F9,                    spawn, SHCMD("$HOME/.local/bin/chsink") },
	{ MODKEY,                       XK_Print,                 spawn, SHCMD("$HOME/.local/bin/drecord") },
	{ MODKEY|ShiftMask,             XK_Print,                 spawn, SHCMD("$HOME/.local/bin/drecord kill") },
	{ MODKEY,                       XK_Delete,                spawn, SHCMD("$HOME/.local/bin/drecord kill") },
	{ MODKEY,                       XK_Insert,                spawn, SHCMD("$HOME/.local/bin/bmark") },
	{ MODKEY|ControlMask,           XK_Insert,                spawn, SHCMD("$HOME/.local/bin/bmark add $(xclip -o)") },
	// media
	{ MODKEY|ShiftMask,             XK_n,                     spawn, SHCMD("st" " -c mixer -e ncpamixer") },
	{ 0,                            XF86XK_AudioMute,         spawn, SHCMD("pamixer -t; sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY|ControlMask,           XK_minus,                 spawn, SHCMD("pamixer -d 1;  sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY,                       XK_minus,                 spawn, SHCMD("pamixer -d 5;  sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY|ShiftMask,             XK_minus,                 spawn, SHCMD("pamixer -d 25; sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY|ControlMask,           XK_equal,                 spawn, SHCMD("pamixer -i 1;  sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY,                       XK_equal,                 spawn, SHCMD("pamixer -i 5;  sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY|ShiftMask,             XK_equal,                 spawn, SHCMD("pamixer -i 25; sb-volume ; pkill slstatus ; slstatus") },
	{ ControlMask,                  XF86XK_AudioLowerVolume,  spawn, SHCMD("pamixer -d 1;  sb-volume ; pkill slstatus ; slstatus") },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, SHCMD("pamixer -d 5;  sb-volume ; pkill slstatus ; slstatus") },
	{ ShiftMask,                    XF86XK_AudioLowerVolume,  spawn, SHCMD("pamixer -d 25; sb-volume ; pkill slstatus ; slstatus") },
	{ ControlMask,                  XF86XK_AudioRaiseVolume,  spawn, SHCMD("pamixer -i 1;  sb-volume ; pkill slstatus ; slstatus") },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn, SHCMD("pamixer -i 5;  sb-volume ; pkill slstatus ; slstatus") },
	{ ShiftMask,                    XF86XK_AudioRaiseVolume,  spawn, SHCMD("pamixer -i 25; sb-volume ; pkill slstatus ; slstatus") },
	{ MODKEY,                       XK_p,                     spawn, SHCMD("playerctl play-pause ; mpc toggle") },
	{ MODKEY|ShiftMask,             XK_p,                     spawn, SHCMD("playerctl pause ; mpc pause") },
	{ MODKEY|ControlMask,           XK_bracketleft,           spawn, SHCMD("mpc seek -1") },
	{ MODKEY,                       XK_bracketleft,           spawn, SHCMD("mpc seek -5") },
	{ MODKEY|ShiftMask,             XK_bracketleft,           spawn, SHCMD("mpc seek -30") },
	{ MODKEY|ControlMask,           XK_bracketright,          spawn, SHCMD("mpc seek +1") },
	{ MODKEY,                       XK_bracketright,          spawn, SHCMD("mpc seek +5") },
	{ MODKEY|ShiftMask,             XK_bracketright,          spawn, SHCMD("mpc seek +30") },
	{ 0,                            XF86XK_AudioPrev,         spawn, SHCMD("playerctl previous ;   mpc prev") },
	{ 0,                            XF86XK_AudioNext,         spawn, SHCMD("playerctl next ;       mpc next") },
	{ 0,                            XF86XK_AudioPause,        spawn, SHCMD("playerctl pause ;      mpc pause") },
	{ 0,                            XF86XK_AudioPlay,         spawn, SHCMD("playerctl play-pause ; mpc toggle") },
	{ 0,                            XF86XK_AudioStop,         spawn, SHCMD("playerctl stop ;       mpc stop") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
