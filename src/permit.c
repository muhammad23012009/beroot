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

static int get_group_list(uid_t uid)
{
    struct passwd *pwd = getpwuid(uid);
    gid_t *groups = (gid_t *)malloc(sizeof(gid_t));
    int ngroups;

    if (!pwd)
        return 0;
    
    if (!groups)
        return 0;

    getgrouplist(pwd->pw_name, pwd->pw_gid, groups, &ngroups);
    free(groups);

    return ngroups;
}

/* Check if user is in specified group */
static int check_group(uid_t uid, struct group *group_check)
{
    struct passwd *pwd = getpwuid(uid);
    int ngroups = get_group_list(uid);
    gid_t *groups = (gid_t *)malloc(sizeof(gid_t) * ngroups);

    if (!pwd) {
        return 0;
    }

    if (!group_check) {
        errx(1, "group doesn't exist");
    }

    if (getgrouplist(pwd->pw_name, pwd->pw_gid, groups, &ngroups) == -1) {
        free(groups);
        return 0;
    }

    for (int i = 0; i < ngroups; i++) {
        if (groups[i] == group_check->gr_gid)
            return 1;
    }

    return 0;
}

int check_permitted(uid_t uid)
{
    // TODO: implement proper config checking here
    int status;

    /* Hardcoded group name for now, will be fixed once config checking is in place. */
    status = check_group(uid, getgrnam("wheel"));

    return status;
}