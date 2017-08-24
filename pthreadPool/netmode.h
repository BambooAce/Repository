
typedef void (*WORKPROCESS)(int arg);

void select_work(int lisfd);
void poll_work(int lisfd);
void epoll_work(int lisfd);

void acceptHandler();
void readHandler();
void writeHandler();


void IO_multiplexing(WORKPROCESS work_process, void *arg);
