Câu hỏi: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?
Vì hàm kill(pid_t pid, int sig) dùng để truyền signal giữa các process với nhau.
- Parent process có thông tin về PID của child process (trả về của hàm fork())
- Child process chờ để nhận signal từ parent process và có hàm handle signal khi có tín hiệu SIGUSR1
Do đó, parent process dùng kill() để thông báo cho child process thực thi hàm handle. 