// Copyright (c) Adelays.\n 
// You don't have the right to modify or share this file.
// (or it will explose).

#define HERCULES 1
//#define RATHENA
//#define EATHENA

#ifndef _HASHIELD_H_
#define _HASHIELD_H_
#include "char.h"

#ifdef RATHENA
extern void char_disconnect_player(uint32 account_id);
#endif

#ifdef EATHENA
extern void disconnect_player(int account_id);
#endif

int parse_fromhashield(int fd);
int hashield_connect(int fd);
int hashield_account_connected(int account_id);
int hashield_fd;
#endif /* _HASHIELD_H_ */
