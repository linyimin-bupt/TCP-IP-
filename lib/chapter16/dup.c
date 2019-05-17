#include <stdio.h>
#include <unistd.h>

int main(void) {

    int dufd1, dufd2;

    dufd1 = dup(1);
    // 指定返回的复制描述符为7, 若复制前描述符已经被占用, 则先关闭起对应的文件
    dufd2 = dup2(1, 7);

    printf("dufd1 = %d, dufd2 = %d\n", dufd1, dufd2);

    char str1[] = "Hi~ \n";
    char str2[] = "It's a nice day~ \n";

    write(dufd1, str1, sizeof(str1));
    write(dufd2, str2, sizeof(str2));

    close(dufd1);
    close(dufd2);

    write(1, str1, sizeof(str1));

    close(1);

    // 所有的描述符都关闭之后,不能在发送新的数据
    write(1, str2, sizeof(str2));

    return 0;
}