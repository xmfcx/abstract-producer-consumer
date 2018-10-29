#ifndef LEO_SLAM_PRODUCERCONSUMERMASTER_H
#define LEO_SLAM_PRODUCERCONSUMERMASTER_H

#include <future>
#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include <iostream>
#include <chrono>
#include <algorithm>

class ProConMaterialMother {
public:
  typedef std::shared_ptr<ProConMaterialMother> Ptr;
  unsigned long count{0};

  virtual void Log() {};

  virtual ~ProConMaterialMother() = default;
};


class ProducerConsumerMaster {
public:
  typedef std::unique_ptr<ProducerConsumerMaster> Ptr;
  typedef std::function<ProConMaterialMother::Ptr(
    ProConMaterialMother::Ptr)> FuncType;

  ProducerConsumerMaster();

  void
  AddWorker(FuncType &function, const std::string &name, const bool &is_end);

  void PushTop(const ProConMaterialMother::Ptr &material);

  void KillAll();

  void WaitUntilAllDone();

  int get_queue_limit() const;

  void set_queue_limit(int queue_limit_);

  void set_debug_queue_size(bool debug_queue_size);

  void set_debug_results(bool debug_results);

  void set_debug_detail(bool debug_detail);

  void set_debug_timing(bool debug_timing);

private:
  std::shared_ptr<std::atomic<bool>> all_should_die;

  struct WorkerMessage {
    typedef std::shared_ptr<WorkerMessage> Ptr;
    std::mutex mutex;
    std::condition_variable con_var;
    std::queue<ProConMaterialMother::Ptr> queue;
  };

  std::vector<WorkerMessage::Ptr> messages_;
  std::vector<std::future<void>> futures_;

  std::mutex mutex_io_;

  void Worker(const int &indice_worker,
              const std::string &name_worker,
              WorkerMessage::Ptr msg_before,
              WorkerMessage::Ptr msg_after,
              const std::shared_ptr<std::atomic<bool>> &should_die,
              FuncType function,
              const bool &is_end);

  int queue_limit_{100};
  unsigned long tally_message{0};
  bool debug_queue_size_{false};
  bool debug_results_{false};
  bool debug_detail_{false};
  bool debug_timing_{false};
};


#endif //LEO_SLAM_PRODUCERCONSUMERMASTER_H
