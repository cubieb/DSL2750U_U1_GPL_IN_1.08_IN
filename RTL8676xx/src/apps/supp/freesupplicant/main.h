#ifndef _MAIN_H
#define _MAIN_H
#define MAX_EAPOL_VER 2
#define MAX_NAME_SIZE 32
#define MAX_PASSWD_SIZE 32
#define MAX_CARDIF_SIZE 32
struct config_data{
    char name[MAX_NAME_SIZE];
    char passwd[MAX_PASSWD_SIZE];
    char cardif[MAX_CARDIF_SIZE];
};
#endif
