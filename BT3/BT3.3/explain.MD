BT3.3: 2 trạng thái đặc biệt thường gặp trong quản lí tiến trình Zombie và Orphan

1. Zombie process: Zombie process xuất hiện khi tiến trình con đã thực thi xong nhưng tiến trình cha không được biết về sự kiện này, thì khi đó tiến trình con đã làm xong nhưng không được cha ghi nhận.
Điều này xảy ra khi tiến trình con đã gọi exit() nhưng tiến trình cha chưa gọi hoặc ko gọi wait() hoặc waitpid().
Khi đó, mặc dù mã lệnh, memory và tài nguyên của tiến trình con đã được giải phóng nhưng Process Table Entry (bảng tiến trình) vẫn giữ lại thông tin của tiến trình con(PID, mã thoát).
--> Zombie process tồn tại để tiến trình cha có thể lấy mã thoát từ tiến trình con.

2. Ophan process: Ophan Process là tiến trình con mà có tiến trình cha đã kết thúc trước cả tiến trình con.
- Khi cha chết, Linux sẽ gán tiến trình con cho tiến trình cha PID=1 hoặc systemd.
- Orphan process là cơ chế bình thường trên Linux nhằm đảm bảo tiến trình con vẫn thực thi công việc của mình mặc dù tiến trình cha đã chết.
- Khi trở thành orphan, tiến trình con sẽ nhận init/systemd nhận làm cha.
- init sẽ gọi wait để dọn dẹp khi tiến trình con kết thúc --> tránh zombie, do đó tránh rò rỉ bảng tiến trình.