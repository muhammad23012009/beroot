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

static char* get_password(void) {
    struct termios old_settings, new_settings;
    char* password = NULL;
    size_t len;

    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    while (getline(&password, &len, stdin)) {
        printf("\n");
        break;
    }

    if (!password || strlen(password) == 1) {
        fprintf(stderr, "beroot: failed to get password\n");
        tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
        return NULL;
    }

    // NULL-terminate the password ourselves.
    password[strlen(password)-1] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

    return password;
}

int check_password(uid_t uid)
{
    struct passwd* pw_entry = getpwuid(uid);
    if (!pw_entry) {
        errx(1, "user ID %d doesn't exist", uid);
    }

    printf("Enter password for %s: ", pw_entry->pw_name);
    char* password = get_password();

    if (!password) {
        free(password);
        errx(1, "invalid password");
    }

    if (strcmp(pw_entry->pw_passwd, "x")) {
        return strcmp(pw_entry->pw_passwd, crypt(password, pw_entry->pw_passwd)) == 0 ? 1 : 0;
    } else { // Password is in shadow file
        struct spwd* shadow_entry = getspnam(pw_entry->pw_name);
        if (!shadow_entry) {
            errx(1, "failed to read shadow entry for user %s\n", pw_entry->pw_name);
        }

        return strcmp(shadow_entry->sp_pwdp, crypt(password, shadow_entry->sp_pwdp)) == 0 ? 1 : 0;
    }
}