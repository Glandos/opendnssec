/*
 * Copyright (c) 2009 NLNet Labs. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 * Command handler.
 *
 */

#ifndef DAEMON_CMDHANDLER_H
#define DAEMON_CMDHANDLER_H

#include "config.h"

#include <sys/un.h>

#include "shared/allocator.h"
#include "shared/locks.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ODS_SE_MAX_HANDLERS 5

/* back reference to the engine */
struct engine_struct;

typedef struct cmdhandler_struct cmdhandler_type;
struct cmdhandler_struct {
    allocator_type* allocator;
    struct engine_struct* engine;
    struct sockaddr_un listen_addr;
    ods_thread_type thread_id;
    int listen_fd;
    int client_fd;
    int need_to_exit;
};

/**
 * Create command handler.
 * \param[in] allocator memory allocator
 * \param[in] filename socket file name
 * \return cmdhandler_type* created command handler
 *
 */
cmdhandler_type* cmdhandler_create(allocator_type* allocator,
    const char* filename);

/**
 * Start command handler.
 * \param[in] cmdhandler_type* command handler
 *
 */
void cmdhandler_start(cmdhandler_type* cmdhandler);

/**
 * Cleanup command handler.
 * \param[in] cmdhandler command handler
 *
 */
void cmdhandler_cleanup(cmdhandler_type* cmdhandler);

struct cmd_func_block {
	/* Name of command */
	const char* cmdname;
	/* print usage information */
	void (*usage)(int sockfd);
	/* print help, more elaborate than usage. Allowed to be
	 * NULL to indicate no help is available */
	void (*help)(int sockfd);
	/* 1 if module claims responibility for command
	 * 0 otherwise */
	int (*handles)(const char *cmd, ssize_t n);
	/* 0 command executed, all OK
	 * -1 Errors parsing commandline / missing params
	 * positive error code to return to user.
	 * */
	int (*run)(int sockfd, struct engine_struct* engine,
		const char *cmd, ssize_t n);
};

void cmdhandler_get_usage(int sockfd);
struct cmd_func_block* get_funcblock(const char *cmd, ssize_t n);

#ifdef __cplusplus
}
#endif

#endif /* DAEMON_CMDHANDLER_H */
