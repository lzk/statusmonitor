#ifndef TOECCONFIG_H
#define TOECCONFIG_H

#ifndef SERVER_PATH
#ifdef LOONGSON
#define SERVER_PATH "/var/run/cups/tjgd1zsm.domain"
//#define SERVER_PATH "/var/spool/cups/tmp/tjgd1zsm.domain"
#else
#define SERVER_PATH "/tmp/tjgd1zsm.domain"
#endif
#endif

#endif // TOECCONFIG_H
