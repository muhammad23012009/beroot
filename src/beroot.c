/*
*   beroot: an extremely barebones setuid-root helper.
*   Copyright (C) 2023  Muhammad <muhammad23012009@hotmail.com>
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "beroot.h"

int main(int argc, char **argv)
{
    const char* cmd = argv[1];
    char *arguments[LINE_MAX];
    uid_t real_uid;
    uid_t target_uid = 0;
    int i;

    // Get current user ID so we can revert back to it.
    real_uid = getuid();

    // Check if we're authorized to go root
    // TODO: implement configs later
    int check = check_permitted();
    if (check != 0) {
        errx(errno, "user ID %d not permitted to be root", real_uid);
        return errno;
    }

    // Prepare arguments to pass to execvp()
    arguments[LINE_MAX-1] = '\0';
    for (i = 1; i < argc; i++) {
        arguments[i-1] = argv[i];
    }

    // Now switch to UID 0 and do root stuff.
    setuid(target_uid);

    // It's go time!
    execvp(cmd, arguments);

    err(errno, NULL);
    return errno;
}
