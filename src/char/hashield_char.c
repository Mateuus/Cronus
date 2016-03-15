// Copyright (c) Hashield.\n 
// You don't have the right to modify or share this file.
// (or it will explose).

#include "../common/cbasetypes.h"
#include "../common/core.h"
#include "../common/db.h"
#include "../common/malloc.h"
#include "../common/mapindex.h"
#include "../common/mmo.h"
#include "../common/socket.h"
#include "../common/strlib.h"
#include "../common/timer.h"
#include "../common/utils.h"

#include "char.h"
#include "hashield_char.h"
#include "inter.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_CYAN "\x1b[36m" // Color Start
#define ANSI_COLOR_BLUE "\x1B[34m" // Color Start
#define ANSI_COLOR_END "\x1B[0m" // To flush out prev settings
 
int hashield_fd = 0;
char out[1024];

void ShowHashieldDebug(const char *string, ...) {
	va_list ap; 
	va_start(ap, string);
	vsprintf(out, string, ap);
	printf("%s%s %s%s",ANSI_COLOR_CYAN,"<Hashield> : ",out,ANSI_COLOR_END);
	va_end(ap);
}

#if defined(RATHENA)
extern struct mmo_map_server map_server[MAX_MAP_SERVERS];
#endif

#if defined(EATHENA)
extern struct mmo_map_server{
	int fd;
	uint32 ip;
	uint16 port;
	int users;
	unsigned short map[MAX_MAP_PER_SERVER];
} server[MAX_MAP_SERVERS];
#endif

#ifdef RATHENA
	extern struct CharServ_Config charserv_config;
#endif

#ifdef HERCULES
	extern int mapif_disconnectplayer(int fd, int account_id, int char_id, int reason);
#endif

#ifdef EATHENA
	extern char userid[24];
	extern char passwd[24];
#endif

void disconnect_player_hashield(int account_id){
	int i;

	#ifdef HERCULES
		chr->disconnect_player(account_id);
	#elif defined RATHENA
		char_disconnect_player(account_id);
	#else
		disconnect_player(account_id);
	#endif

	#if defined(EATHENA) 
	// Check all map servers to DC that guy.
	for (i = 0; i < ARRAYLENGTH(server); i++)
	{
		if (server[i].fd > 0)
		{
			// Found a server here.
			mapif_disconnectplayer(server[i].fd, account_id, 0, 200);
		}
	}
	#endif
	
	
	#if defined(RATHENA)
	// Check all map servers to DC that guy.
	for (i = 0; i < ARRAYLENGTH(map_server); i++)
	{
		if (map_server[i].fd > 0)
		{ 
			// Found a server here.
			mapif_disconnectplayer(map_server[i].fd, account_id, 0, 200);
		}
	}
	#endif

	#ifdef HERCULES
	// Check all map servers to DC that guy.
	for (i = 0; i < ARRAYLENGTH(chr->server); i++)
	{
		if (chr->server[i].fd > 0)
		{
			// Found a server here.
			mapif_disconnectplayer(chr->server[i].fd, account_id, 0, 200);
		}
	}
	#endif

}

int parse_fromhashield(int fd)
{

	// only process data from the hashield Server
	if (fd != hashield_fd)
	{
		ShowHashieldDebug("parse_fromhashield: Disconnecting invalid session #%d (is not the Hashield Server)\n", fd);
		do_close(fd);
		return 0;
	}

	if (session[fd]->flag.eof)
	{
		do_close(fd);
		hashield_fd = -1;
		ShowHashieldDebug("Connection to Hashield Server lost.\n\n");
		return 0;
	}

	while (RFIFOREST(fd) >= 2)
	{
		uint16 command = RFIFOW(fd, 0);

		switch (command)
		{
			// hashield-server alive packet
			case 0x4449:
				if (RFIFOREST(fd) < 2)
					return 0;
				RFIFOSKIP(fd, 2);
				break;
				
			
			case 0x4444:
				if (RFIFOREST(fd) < 50)
					return 0; 
				ShowHashieldDebug("The Hashield Server want to connect again??? How is that even possible! Skipping...");
				RFIFOSKIP(fd, 50);
				break;
 
			// acknowledgement of account ban
			case 0x4445:
				if (RFIFOREST(fd) < 6)
					return 0;
				{
					int account_id = RFIFOL(fd, 2);
					ShowHashieldDebug("Hashield Server Request account disconnect. (connection #%d).\n", fd);
					disconnect_player_hashield(account_id);


					RFIFOSKIP(fd, 6);

				}
				break;

			default:
				ShowHashieldDebug("Unknown packet 0x%04x received from Hashield Server, disconnecting.\n", command);
				set_eof(fd);
				return 0;
		}
	}

	RFIFOFLUSH(fd);
	return 0;
}

int hashield_account_connected(int account_id){
	int fd = hashield_fd;

	if (hashield_fd < 0 || session[hashield_fd] == NULL){
		return 0;
	}

	WFIFOHEAD(fd, 10);
	WFIFOL(fd, 0) = 6;
	WFIFOW(fd, 4) = 0x4446;
	WFIFOL(fd, 6) = account_id;
	WFIFOSET(fd, 10);

	return 1;

}

int hashield_connect(int fd){
	char* l_user;
	char* l_pass;

	if (RFIFOREST(fd) < 50){
		return 0;
	}
	
	l_user = (char*)RFIFOP(fd, 2);
	l_pass = (char*)RFIFOP(fd, 26);
	l_user[23] = '\0';
	l_pass[23] = '\0';

	if (hashield_fd > 0 && session[hashield_fd] != NULL){
		ShowHashieldDebug("Hashield Server already connected! \n");

	} 
	else if (
		 
#ifdef HERCULES
		strcmp(l_user, chr->userid) != 0 || 	strcmp(l_pass, chr->passwd) != 0
#elif defined RATHENA
		strcmp(l_user, charserv_config.userid) != 0 || 	strcmp(l_pass, charserv_config.passwd) != 0
#else
		strcmp(l_user, userid) != 0 || 	strcmp(l_pass, passwd) != 0
#endif
		
		)
	{ 
		WFIFOHEAD(fd, 6);
		WFIFOL(fd, 0) = 2;
		WFIFOW(fd, 4) = 0x4447;
		WFIFOSET(fd, 6);
		#ifdef HERCULES
		ShowHashieldDebug("Refused Connection from Hashield Server. Wrong userid/passwd (received: %s/%s, expected: %s/%s).\n", l_user, l_pass, chr->userid, chr->passwd);
		#elif defined RATHENA
		ShowHashieldDebug("Refused Connection from Hashield Server. Wrong userid/passwd (received: %s/%s, expected: %s/%s).\n", l_user, l_pass, charserv_config.userid, charserv_config.passwd);
		#else
		ShowHashieldDebug("Refused Connection from Hashield Server. Wrong userid/passwd (received: %s/%s, expected: %s/%s).\n", l_user, l_pass, userid, passwd);
		#endif

	}
	else {
		WFIFOHEAD(fd, 6);
		WFIFOL(fd, 0) = 2;
		WFIFOW(fd, 4) = 0x4448;
		WFIFOSET(fd, 6);

		hashield_fd = fd;
		session[fd]->func_parse = parse_fromhashield;
		session[fd]->flag.server = 1;
		realloc_fifo(fd, FIFOSIZE_SERVERLINK, FIFOSIZE_SERVERLINK);
		ShowHashieldDebug("Authorized Connection from Hashield Server. \n");


	}

	RFIFOSKIP(fd, 50);

	return 1;
	
}


