/*
 * Copyright (C) 2010 Red Hat Inc, Steven Rostedt <srostedt@redhat.com>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License (not later!)
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not,  see <http://www.gnu.org/licenses>
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#ifndef _TRACE_CMD_LOCAL_H
#define _TRACE_CMD_LOCAL_H

/* Local for trace-input.c and trace-output.c */

#include "trace-cmd.h"
#include "event-utils.h"

extern int quiet;

static inline ssize_t
read_intr(int fd, void *buf, size_t count)
{
	ssize_t result;

	do {
		result = read(fd, buf, count);
	} while (result == -1L && errno == EINTR);

	return result;
}

static inline ssize_t
write_intr(int fd, const void *buf, size_t count)
{
	ssize_t result;

	do {
		result = write(fd, buf, count);
	} while (result == -1L && errno == EINTR);

	return result;
}

static inline int
open_intr(const char *pathname, int flags)
{
	int result;

	do {
		result = open(pathname, flags);
	} while (result == -1L && errno == EINTR);

	return result;
}

static inline ssize_t
__do_write(int fd, const void *data, size_t size)
{
	ssize_t tot = 0;
	ssize_t w;

	do {
		w = write_intr(fd, data + tot, size - tot);
		tot += w;

		if (!w)
			break;
		if (w < 0)
			return w;
	} while (tot != size);

	return tot;
}

static inline ssize_t
__do_write_check(int fd, const void *data, size_t size)
{
	ssize_t ret;

	ret = __do_write(fd, data, size);
	if (ret < 0)
		return ret;
	if (ret != size)
		return -1;

	return 0;
}

static inline ssize_t
__do_read(int fd, void *data, size_t size)
{
	ssize_t tot = 0;
	ssize_t r;

	do {
		r = read_intr(fd, data + tot, size - tot);
		tot += r;

		if (!r)
			break;
		if (r < 0)
			return r;
	} while (tot != size);

	return tot;
}

static inline ssize_t
__do_read_check(int fd, void *data, size_t size)
{
	ssize_t ret;

	ret = __do_read(fd, data, size);
	if (ret < 0)
		return ret;
	if (ret != size)
		return -1;

	return 0;
}

#endif /* _TRACE_CMD_LOCAL_H */
