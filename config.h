/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static char normbordercolor[] = "#444444";
static char normbgcolor[]     = "#222222";
static char normfgcolor[]     = "#bbbbbb";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char selfgcolor[]      = "#eeeeee";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor, selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance           title          tags mask     isfloating   monitor */
	{ "Gimp",          NULL,              NULL,           0,            1,           -1 },
	{ "Firefox",       NULL,              NULL,           1 << 8,       0,           -1 },
	{ "VidyoDesktop",  "VidyoDesktop",    "VidyoDesktop", 0,            1,           -1 },
	{ "ROOT",          "Browser",         NULL,           0,            1,           -1 },
	{ "ROOT",          "Canvas",          NULL,           1 << 8,       0,           -1 },
	{ NULL,            NULL,              "_Editor",      0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts/horizgrid.c"
#include "layouts/tcl.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "HHH",      horizgrid },
	{ "|||",      tcl },
	{ "[M]",      monocle },
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

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *mail[]  = { "st", "-e", "neomutt", NULL };
static const char *ranger[]  = { "st", "-e", "ranger", NULL };
static const char *browser[]  = { "qutebrowser", NULL };
static const char *network[]  = { "networkmanager_dmenu", NULL };
static const char *clipboard[]  = { "clipmenu", NULL };
/* Audio controls */
static const char *v_inc[]  = { "pamixer", "--allow-boost", "-i", "5", NULL };
static const char *v_biginc[]  = { "pamixer", "--allow-boost", "-i", "15", NULL };
static const char *v_dec[]  = { "pamixer", "--allow-boost", "-d", "5", NULL };
static const char *v_bigdec[]  = { "pamixer", "--allow-boost", "-d", "15", NULL };
static const char *v_mute[]  = { "pamixer", "--allow-boost", "-t", NULL };
/* Music controls */
static const char *m_player[]  = { "st", "-e", "ncmpcpp", NULL };
static const char *m_pause[]  = { "mpc", "toggle", NULL };
static const char *m_next[]  = { "mpc", "next", NULL };
static const char *m_prev[]  = { "mpc", "prev", NULL };
static const char *m_lilfor[]  = { "mpc", "seek", "+10", NULL };
static const char *m_bigfor[]  = { "mpc", "seek", "+120", NULL };
static const char *m_lilbak[]  = { "mpc", "seek", "-10", NULL };
static const char *m_bigbak[]  = { "mpc", "seek", "-120", NULL };
/* Display brightness controls */
static const char *b_dec[]  = { "sudo", "brightnessctl", "s", "5%-", NULL };
static const char *b_inc[]  = { "sudo", "brightnessctl", "s", "10%+", NULL };

static Key keys[] = {
	/* modifier                     key                  function        argument */
	{ MODKEY,                       XK_d,                spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,           spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_e,                spawn,          {.v = mail } },
	{ MODKEY,                       XK_w,                spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_w,                spawn,          {.v = network } },
	{ MODKEY,                       XK_c,                spawn,          {.v = clipboard } },
	{ MODKEY,                       XK_r,                spawn,          {.v = ranger } },
	{ MODKEY,                       XK_m,                spawn,          {.v = m_player } },
	{ MODKEY|ShiftMask,             XK_m,                spawn,          {.v = v_mute } },
	{ MODKEY,                       XK_p,                spawn,          {.v = m_pause } },
	{ MODKEY,                       XK_bracketleft,      spawn,          {.v = m_lilbak } },
	{ MODKEY|ShiftMask,             XK_bracketleft,      spawn,          {.v = m_bigbak } },
	{ MODKEY,                       XK_bracketright,     spawn,          {.v = m_lilfor } },
	{ MODKEY|ShiftMask,             XK_bracketright,     spawn,          {.v = m_bigfor } },
	{ MODKEY,                       XK_apostrophe,       spawn,          {.v = m_next } },
	{ MODKEY,                       XK_semicolon,        spawn,          {.v = m_prev } },
	{ MODKEY,                       XK_equal,            spawn,          {.v = v_inc } },
	{ MODKEY,                       XK_minus,            spawn,          {.v = v_dec } },
	{ MODKEY|ShiftMask,             XK_equal,            spawn,          {.v = b_inc } },
	{ MODKEY|ShiftMask,             XK_minus,            spawn,          {.v = b_dec } },
	{ MODKEY,                       XK_b,                togglebar,      {0} },
	{ MODKEY,                       XK_j,                focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,                incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return,           zoom,           {.v = termcmd } },
	{ MODKEY,                       XK_Tab,              view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                killclient,     {0} },
	{ MODKEY,                       XK_t,                setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                setlayout,      {.v = &layouts[3]} },
	/*{ MODKEY,                       XK_m,                setlayout,      {.v = &layouts[2]} },*/
	{ MODKEY|ControlMask,           XK_comma,            cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,           cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,            setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,            togglefloating, {0} },
	{ MODKEY,                       XK_0,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,            focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,           focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,            tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,           tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,               xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                                0)
	TAGKEYS(                        XK_2,                                1)
	TAGKEYS(                        XK_3,                                2)
	TAGKEYS(                        XK_4,                                3)
	TAGKEYS(                        XK_5,                                4)
	TAGKEYS(                        XK_6,                                5)
	TAGKEYS(                        XK_7,                                6)
	TAGKEYS(                        XK_8,                                7)
	TAGKEYS(                        XK_9,                                8)
	{ MODKEY|ShiftMask,             XK_q,                quit,           {0} },
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

static const char *xres = "/home/bakar/.Xresources";
