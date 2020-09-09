//
// Created by 李卫东 on 2019-02-25.
//

#ifndef CPPSERVER_SIGNAL_H
#define CPPSERVER_SIGNAL_H

#include "fc/logging/logging.h"
#include <atomic>
#include <errno.h>
#include <fcntl.h>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <vector>

using namespace std;
//重新设计一版本。实现>> <<重载。模板可实现任意类型发送。
namespace fc {
class sync_signal {
public:
  sync_signal(bool time_out = true) : time_out_check(time_out) {
    if (::pipe(_pipe_sign) != 0)
      LOG_FATAL("init sync_signal pipe sign error! %s", "");
    ::fcntl(_pipe_sign[0], F_SETFL, O_NONBLOCK);
    ::fcntl(_pipe_sign[0], F_SETFD, FD_CLOEXEC);
    //::fcntl(_pipe_sign[1], F_SETFL, O_NONBLOCK); //写时需要阻塞
    ::fcntl(_pipe_sign[1], F_SETFD, FD_CLOEXEC);
    _sign_write = _pipe_sign[1]; // 1必须用于写
    _sign_read = _pipe_sign[0];  // 0用于读
  }
  ~sync_signal() {
    close(_sign_write);
    close(_sign_read);
  }

  inline void send(const int &num = 1) {
    vector<char> data;
    for (int i = 0; i < num; i++) {
      data.push_back(char(1));
    }
    write(_sign_write, data.data(), num);
  }
  inline void wait(const int &num = 1) {
    vector<char> data(num);
    int readed_length = 0;
    struct timeval timeout;
    timeout.tv_sec = 4;
    timeout.tv_usec = 10000;
    fd_set set;
    int rv;
    do {
      FD_ZERO(&set);            /* clear the set */
      FD_SET(_sign_read, &set); /* add our file descriptor to the set */

      rv = select(_sign_read + 1, &set, NULL, NULL, &timeout);
      if (rv == -1)
        LOG_FATAL("sync_signal select read sign error!"); /* an error accured */
      else if (rv == 0)
        if (time_out_check)
          LOG_FATAL("sync_signal read sign time out error!"); /* a timeout occured */
        else
          continue;
      else
        readed_length += read(_sign_read, data.data(),
                              num - readed_length); /* there was data to read */
      if (readed_length >= num)
        break;
    } while (1);
  }

private:
  //防止复制
  sync_signal(const signal &&c){};
  sync_signal(const signal &c){};
  void operator=(const sync_signal &c) {}
  void operator=(const sync_signal &&c) {}

private:
  int _pipe_sign[2];
  int _sign_write;
  int _sign_read;
  bool time_out_check = true;
};

} // namespace fc

#endif // CPPSERVER_SIGNAL_H
