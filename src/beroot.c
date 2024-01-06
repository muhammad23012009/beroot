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

static void usage(void)
{
    fprintf(stderr, "usage: beroot <command> [args]\n");
    exit(1);
}

int main(int argc, char **argv)
{
    const char* cmd = argv[1];
    char **arguments = malloc(argc + 1);
    uid_t real_uid;
    uid_t target_uid = 0;
    int check;

    // Check if we have any arguments
    if (argc < 2) {
        usage();
    }

    // Get current user ID so we can revert back to it.
    real_uid = getuid();

    // Check if we're authorized to go root
    // TODO: implement configs later
    check = check_permitted(real_uid);
    if (!check) {
        errx(errno, "user ID %d not permitted to be root", real_uid);
    }

    check = check_password(real_uid);
    if (!check) {
        errx(errno, "wrong password for user ID %d", real_uid);
    }

    // Prepare arguments to pass to execvp()
    for (int i = 1; i < argc; i++) {
        arguments[i-1] = argv[i];
    }
    arguments[argc + 1] = '\0';

    // Now switch to UID 0 and do root stuff.
    setuid(target_uid);

    // It's go time!
    execvp(cmd, arguments);

    // If we've failed, may as well clean up.
    setuid(real_uid);
    seteuid(real_uid);

    err(errno, "\'%s\'", cmd);
}
