#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    if (fork() == 0) {
        // Tiến trình con: thực hiện sàng lọc số nguyên tố
        close(p[1]); // Đóng đầu ghi vì tiến trình con chỉ cần đọc
        
        while (1) {
            int prime;
            // Đọc số nguyên tố tiếp theo
            if (read(p[0], &prime, sizeof(prime)) == 0) {
                // Không còn dữ liệu để đọc, thoát vòng lặp
                close(p[0]);
                exit(0);
            }

            // In ra số nguyên tố hiện tại
            printf("prime %d\n", prime);

            // Tạo pipe mới cho việc sàng lọc tiếp theo
            int next_p[2];
            pipe(next_p);

            if (fork() == 0) {
                // Tiến trình con tiếp theo: tiếp tục với bộ lọc mới
                close(next_p[1]); // Đóng đầu ghi, chỉ đọc
                close(p[0]);      // Đóng đầu đọc của pipe trước vì tiến trình này không cần nó
                p[0] = next_p[0]; // Gán p[0] cho pipe mới
            } else {
                // Tiến trình cha: lọc và gửi các số không chia hết cho prime
                close(next_p[0]); // Đóng đầu đọc của pipe mới vì tiến trình cha chỉ ghi

                int num;
                while (read(p[0], &num, sizeof(num)) != 0) {
                    if (num % prime != 0) {
                        write(next_p[1], &num, sizeof(num));
                    }
                }

                // Đóng các đầu ghi và đọc khi kết thúc
                close(p[0]);
                close(next_p[1]);
                wait(0); // Đợi tiến trình con hoàn thành
                exit(0); // Thoát khỏi vòng lặp sau khi hoàn thành sàng lọc
            }
        }
    } else {
        // Tiến trình cha ban đầu: gửi các số từ 2 đến 34 vào pipe
        close(p[0]); // Đóng đầu đọc vì tiến trình cha chỉ ghi

        for (int i = 2; i < 281; i++) {
            write(p[1], &i, sizeof(i));
        }

        close(p[1]); // Đóng đầu ghi sau khi ghi xong
        wait(0);     // Đợi tiến trình con kết thúc
        exit(0);     // Kết thúc tiến trình cha
    }
}
