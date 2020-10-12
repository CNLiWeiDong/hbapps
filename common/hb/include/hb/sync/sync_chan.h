//
// Created by 李卫东 on 2019-02-25.
//

#ifndef CPPSERVER_CHAN_H
#define CPPSERVER_CHAN_H

#include <atomic>
#include <deque>
#include <errno.h>
#include <hb/log/log.h>
#include <fcntl.h>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <hb/sync/sync_error.h>
#include <hb/error/exception.h>

using namespace std;
//重新设计一版本。实现>> <<重载。模板可实现任意类型发送。
namespace hb { namespace sycn {
template <typename CHAN_VALUE> 
class sync_chan {
public:
  sync_chan(bool time_out = true) : time_out_check(time_out) {
    if (::pipe(_pipe_sign) != 0)
        hb_throw(hb_sync_exception().msg("init sync_chan pipe sign error!"));
    ::fcntl(_pipe_sign[0], F_SETFL, O_NONBLOCK);
    ::fcntl(_pipe_sign[0], F_SETFD, FD_CLOEXEC);
    //::fcntl(_pipe_sign[1], F_SETFL, O_NONBLOCK);  //写时需要阻塞
    ::fcntl(_pipe_sign[1], F_SETFD, FD_CLOEXEC);
    _sign_write = _pipe_sign[1]; // 1必须用于写
    _sign_read = _pipe_sign[0];  // 0用于读
  }
  ~sync_chan() {
    close(_sign_write);
    close(_sign_read);
  }
  friend void operator<<(sync_chan<CHAN_VALUE> &c, const CHAN_VALUE &data) {
    c.m_send(data);
  }
  friend void operator>>(sync_chan<CHAN_VALUE> &c, CHAN_VALUE &data) {
    c.m_read(data);
  }

  friend void operator>>(const CHAN_VALUE &data, sync_chan<CHAN_VALUE> &c) {
    c.m_send(data);
  }
  friend void operator<<(CHAN_VALUE &data, sync_chan<CHAN_VALUE> &c) {
    c.m_read(data);
  }

private:
  void m_send(const CHAN_VALUE &data) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      chan_queue.push_back(data);
    }
    //先插入数据再通知，放在后面
    const char d = '1';
    write(_sign_write, &d, 1);
  }
  void m_read(CHAN_VALUE &data) {
    wait();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      if (chan_queue.size() == 0)
        hb_throw(hb_sync_exception().msg("empty chan queue!"));
      data = chan_queue.front();
      chan_queue.pop_front();
    }
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
        hb_throw(hb_sync_exception().msg("sync_chan select read sign error!"));
      else if (rv == 0)
        if (time_out_check)
          hb_throw(hb_sync_exception().msg("sync_chan read sign time out error!"));
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
  chan(const chan &&c) = delete;
  chan(const chan &c) = delete;
  void operator=(const chan &c) = delete;
  void operator=(const chan &&c) = delete;

private:
  int _pipe_sign[2];
  int _sign_write;
  int _sign_read;
  deque<CHAN_VALUE> chan_queue;
  mutex queue_mutex;
  bool time_out_check = true;
};

} }// namespace hb

#endif // CPPSERVER_CHAN_H
