#include "couche2.h"
#include "couche3.h"
#include "couche5.h"
#include "sos_defines.h"

extern virtual_disk_t virtual_disk_sos;
extern session_t session;

void main(){
    int cr;
    pid_t pid;
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "echec fork\n");
        exit(1);
        break;
    case 0:
        execl("mkdir", OS_NAME_REP, NULL);
        fprintf(stderr, "echec rep\n");
        exit(2);
        break;
        
    default:
        wait(&cr);
        break;
    }
    switch (pid = fork())
    {
    case -1:
        fprintf(stderr, "echec fork\n");
        exit(1);
        break;
    case 0:
        execl("cmd_format", "./cmd_format", OS_NAME_REP, "500", NULL);
        fprintf(stderr, "echec disk\n");
        exit(2);
        break;
        
    default:
        wait(&cr);
        break;
    }
    virtual_disk_sos.super_block.number_of_files = 0;
    virtual_disk_sos.super_block.number_of_users = 0;
    virtual_disk_sos.super_block.nb_blocks_used = 0;
    virtual_disk_sos.super_block.first_free_byte = 0;
    update_first_free_byte();
    char login[MAX_WORD];
    do{ 
        printf("login(not 0): ");
        scanf("%s", login);
        while(getchar() != '\n'){}

    }while(!strcmp(login, "0"));

    char password[MAX_WORD];
    char hashRes[SHA256_BLOCK_SIZE*2 + 1];
    printf("password: ");
    fgets(password, MAX_WORD, stdin);

    sha256ofString((BYTE *)password,hashRes);
    create_root(login, hashRes);
    virtual_disk_sos.super_block.number_of_users++;
    char *passwordfile = "passwd";
    init_inode(passwordfile, sizeof(users_table_t), virtual_disk_sos.super_block.first_free_byte);
    virtual_disk_sos.inodes[0].uright = 0;
    write_users_table();

    return 0;
}