#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BUF_SIZE 2048
#define MAX_CMD 64
#define MSG_LEN 256

typedef enum {cont, ask} option;
struct cmd {
    uint8_t msg_len;
    option opt;
    char msg[MSG_LEN];
};

struct rom {
    uint8_t cmds_size;
    struct cmd cmds[MAX_CMD];
} rom;

int load_rom(const char* path)
{
    FILE* file;
    file = fopen(path, "rb");
    uint8_t buffer[BUF_SIZE];

    if (file == NULL)
    {
        printf("failed loading rom: can't open rom\n");
        return 1;
    }

    fread(buffer, BUF_SIZE, 1, file);

    // check magic
    if (strncmp(buffer, "SPGHT", 5) != 0)
    {
        printf("failed loading rom: bad magic\n");
        return 1;
    }

    // finds out how many commands rom contains
    uint8_t entry_count;
    entry_count = buffer[5];

    if (entry_count >= MAX_CMD)
    {
        printf("failed loading rom: too many cmd entries\n");
        return 1;
    }

    rom.cmds_size = entry_count;

    // fills cmds array
    size_t offset = 6;
    for (int i = 0; i < entry_count; i++)
    {
        uint8_t msg_len = buffer[offset];
        
        rom.cmds[i].msg_len = msg_len;
        rom.cmds[i].opt = buffer[offset + 1];
        strncpy(rom.cmds[i].msg, &buffer[offset + 2], msg_len);

        offset += msg_len + 2;
    }

    fclose(file);

    return 0;
}

// weird work-around getchar()
char get_first_char() {
    char *line;
    size_t len = 0;
    getline(&line, &len, stdin);
    return *line;
}

void ask_opt(uint8_t step_nr, const char* msg)
{
    printf("Did you understand the last step in the recipe? (y or n): ");

    char ans = get_first_char();

    if (ans == 'y')
    {
        printf("Great! Let's continue.\n");
    } else if (ans == 'n')
    {
        printf("Let's try that last step once more.\n");
        printf("\nStep %d) %s\n", step_nr+1, msg);
        ask_opt(step_nr, msg);
    } else
    {
        printf("Not a good answer. Let's try that one again.\n");
        printf("\nStep %d) %s\n", step_nr+1, msg);
        ask_opt(step_nr, msg);
    }
}

void cont_opt(void)
{
    printf("Press enter to continue...");
    get_first_char();
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage: spaghetti <path_to_rom>\n");
        return 1;
    }

    printf("loading spaghetti rom... %s\n", argv[1]);

    // checks if successfully loaded rom
    if (load_rom(argv[1])) { return 1; }

    // spaghetti loop
    for (int i = 0; i < rom.cmds_size; i++)
    {
        struct cmd cmd = rom.cmds[i];
        printf("\nStep %d) %s\n", i+1, cmd.msg);

        switch (cmd.opt)
        {
            case cont:
                cont_opt();
                break;
            case ask:
                ask_opt(i, cmd.msg);
                break;
            default:
                printf("invalid option\n");
                break;
        }
    }

    return 0;
}