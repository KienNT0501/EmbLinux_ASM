1. Vai trò của các tham số tham gia pthread_create() và pthread_join():
- pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
Trong đó: 
pthread_t *thread: con trỏ để lưu trữ thread_ID.
attr: thread attributes (NULL for default).
start_routine: function that thread will execute.
arg: argument passed to thread function.
Kiểu trả về int.
Đối với các tham số này ta sẽ truyền tham chiếu (địa chỉ) vào.
- pthread_join(pthread_t tar_thread, void** retval)
Trong đó: 
pthread_t tar_thread: tham số luồng đầu vào mà luồng chính sẽ đợi để luồng này thực thi xong.
void** retval: giá trị exit
