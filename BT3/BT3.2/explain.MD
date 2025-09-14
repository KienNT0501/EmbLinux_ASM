BT2: Thay thế mã thực thi của một process bằng một chương trình khác dùng lệnh exec().

1. Dùng fork() để tạo ra 1 process con.
2. Khai báo 2 biến môi trường để truyền vào hàm execlp(): MY_COMMAND và MY_OPTION.
3. Khi pid == 0, in ra dòng "Before exec, Child Process PID: child_pid".
4. Thực hiện lệnh execlp() - thay thế toàn bộ chương trình của child process bằng mã thực thi của biến môi trường truyền vào.
5. Đó là lý do dòng  "After exec..." không bao giờ được in ra
--> Không gian địa chỉ và mã lệnh của tiến trình con đã hoàn toàn bị thay đổi bằng lệnh execlp().
6. Khi chạy chương trình mới, ở đây là execlp("ls", "ls", "-l", NULL):
6.1 execlp tìm thư mục ls trong biến môi trường PATH và tìm ra /bin/ls.
6.2 Thay đổi toàn bộ child process bao gồm:
+ Child's memory: thay đổi bằng mã lệnh của /bin/ls
+  PID không đổi
6.3 kernel set /bin/ls chạy với hàm main với các tham số đầu vào argv[0] = "ls", argv[1] = "-l", argv[2] = NULL
7. Sau khi ls -l hoàn tất, ls gọi hàm exit(status), ở parent process có gọi wait(NULL) để ngăn child process trở thành zombie process.