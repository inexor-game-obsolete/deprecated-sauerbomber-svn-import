#ifndef _FPSGAME_MASTERMODES_H
#define _FPSGAME_MASTERMODES_H

enum {
    MM_AUTH = -1,
    MM_OPEN = 0,
    MM_VETO,
    MM_LOCKED,
    MM_PRIVATE,
    MM_PASSWORD,
    MM_START = MM_AUTH
};

static const char * const mastermodenames[] =  {
    "auth",
    "open",
    "veto",
    "locked",
    "private",
    "password"
};

static const char * const mastermodecolors[] = {
    "",
    "\f0",
    "\f2",
    "\f2",
    "\f3",
    "\f3"
};

static const char * const mastermodeicons[] =  {
    "server",
    "server",
    "serverlock",
    "serverlock",
    "serverpriv",
    "serverpriv"
};

#endif
