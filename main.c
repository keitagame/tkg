#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void install_package(const char *pkg)
{
    char cmd[512];

    // パッケージ名
    char name[128];
    strcpy(name, pkg);

    char *dot = strstr(name, ".tar.gz");
    if (dot) *dot = '\0';

    // DBディレクトリ
    snprintf(cmd, sizeof(cmd),
        "mkdir -p db/%s", name);
    system(cmd);

    // ファイル一覧保存
    snprintf(cmd, sizeof(cmd),
        "tar -tzf %s > db/%s/files.txt",
        pkg, name);
    system(cmd);

    // 展開
    snprintf(cmd, sizeof(cmd),
        "tar -xzf %s -C root",
        pkg);
    system(cmd);

    printf("installed: %s\n", name);
}
void remove_package(const char *name)
{
    char path[256];

    snprintf(path, sizeof(path),
        "db/%s/files.txt", name);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("not installed\n");
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp)) {

        line[strcspn(line, "\n")] = 0;

        char full[1024];
        snprintf(full, sizeof(full),
            "root/%s", line);

        remove(full);
    }

    fclose(fp);

    printf("removed: %s\n", name);
}
int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage:\n");
        printf("  tkg install pkg.tar.gz\n");
        printf("  tkg remove name\n");
        return 1;
    }

    if (!strcmp(argv[1], "install")) {
        install_package(argv[2]);
    }
    else if (!strcmp(argv[1], "remove")) {
        remove_package(argv[2]);
    }

    return 0;
}