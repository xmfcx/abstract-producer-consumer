#include "ProducerConsumerMaster.h"

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::high_resolution_clock::time_point;

ProducerConsumerMaster::ProducerConsumerMaster() {
  all_should_die = std::make_shared<std::atomic<bool>>(false);
  WorkerMessage::Ptr message_first = std::make_unique<WorkerMessage>();
  messages_.push_back(message_first);
}

void ProducerConsumerMaster::AddWorker(FuncType &function,
                                       const std::string &name,
                                       const bool &is_end) {
  WorkerMessage::Ptr message_sub = std::make_unique<WorkerMessage>();
  messages_.push_back(message_sub);
  size_t indice_last = messages_.size() - 1;

  std::future<void> future_of_worker = std::async(
    std::launch::async,
    &ProducerConsumerMaster::Worker, this,
    indice_last,
    name,
    messages_[indice_last - 1],
    messages_[indice_last],
    std::ref(all_should_die),
    function,
    is_end
  );
  futures_.emplace_back(std::move(future_of_worker));
}

void ProducerConsumerMaster::Worker(const int &indice_worker,
                                    const std::string &name_worker,
                                    WorkerMessage::Ptr msg_before,
                                    WorkerMessage::Ptr msg_after,
                                    const std::shared_ptr<std::atomic<bool>> &should_die,
                                    FuncType function,
                                    const bool &is_end) {
  auto ending_procedure = [this, &indice_worker, &name_worker]() {
    std::lock_guard<std::mutex> lock(mutex_io_);
    std::cout << "Worker " << std::to_string(indice_worker) << " "
              << name_worker << " died." << std::endl;
  };
  while (!*should_die) {
    //wait until previous queue has something
    {
      if (debug_detail_) {
        std::lock_guard<std::mutex> lock(mutex_io_);
        std::cout << "Worker " << std::to_string(indice_worker) << " "
                  << name_worker << " waiting." << std::endl;
      }
      std::unique_lock<std::mutex> lock(msg_before->mutex);
      msg_before->con_var.wait(lock);
      if (*should_die) {
        ending_procedure();
        return;
      }
    }

    //work until previous queue is empty
    std::unique_lock<std::mutex> lock_before(msg_before->mutex);
    while (!msg_before->queue.empty()) {
      if (*should_die) {
        ending_procedure();
        return;
      }
      auto queue_data = std::move(msg_before->queue.front());
      msg_before->queue.pop();
      if (debug_queue_size_) {
        std::lock_guard<std::mutex> lock(mutex_io_);
        std::cout << "Queue " << name_worker
                  << " size: " << msg_before->queue.size() << std::endl;
      }
      lock_before.unlock();
      //we can leave queue_before alone
      //and start working on making use of extracted data
      ProConMaterialMother::Ptr result;
      try {
        TimePoint time_point_start;
        if (debug_timing_)
          time_point_start = Clock::now();

        result = std::move(function(std::move(queue_data)));

        std::chrono::duration<double, std::milli> duration{};
        if (debug_timing_) {
          TimePoint time_point_end = Clock::now();
          duration = time_point_end - time_point_start;
        }

        if (debug_results_ || debug_timing_) {
          std::lock_guard<std::mutex> lock(mutex_io_);
          std::cout << "Worker " << indice_worker << " " << name_worker;
          if (debug_timing_)
            std::cout << " took: " << duration.count() << " ms." << std::endl;
          if (debug_results_) {
            std::cout << "Result: " << std::endl;
            result->Log();
            std::cout << std::endl;
          }
        }
      } catch (std::exception &ex) {
        std::lock_guard<std::mutex> lock(mutex_io_);
        std::cerr << "Worker" << name_worker << " Exception: " << ex.what()
                  << std::endl;
        //lock it again because queue.empty() query will occur
        lock_before.lock();
        continue;
      }


      //now pass the result to the next queue!
      if (!is_end) {
        {
          std::lock_guard<std::mutex> lock_guard(msg_after->mutex);
          msg_after->queue.emplace(result);
        }
        msg_after->con_var.notify_all();
      }

      //lock it again because queue.empty() query will occur
      lock_before.lock();
    }
    lock_before.unlock();

  }


  ending_procedure();
}

void
ProducerConsumerMaster::PushTop(const ProConMaterialMother::Ptr &material) {
  material->count = tally_message;
  struct CountIsSmallerThan {
    const int limit;

    explicit CountIsSmallerThan(int n) : limit(n) {}

    bool operator()(const WorkerMessage::Ptr &msg) const {
      return msg->queue.size() < limit;
    }
  };
  if (std::all_of(messages_.begin(), messages_.end(),
                  CountIsSmallerThan(queue_limit_))) {
    WorkerMessage::Ptr message_first = messages_.front();
    std::lock_guard<std::mutex> lock(message_first->mutex);
    message_first->queue.emplace(material);
    message_first->con_var.notify_all();
  } else if (debug_detail_) {
    std::lock_guard<std::mutex> lock(mutex_io_);
    std::cout << "Skipped msg " << material->count << std::endl;
  }
  tally_message++;
}

void ProducerConsumerMaster::WaitUntilAllDone() {
  for (auto &future : futures_) {
    future.get();
  }
  std::cout << "All workers died." << std::endl;
}

void ProducerConsumerMaster::KillAll() {
  *all_should_die = true;
  for (auto &message : messages_)
    message->con_var.notify_all();
}

int ProducerConsumerMaster::get_queue_limit() const {
  return queue_limit_;
}

void ProducerConsumerMaster::set_queue_limit(int queue_limit_) {
  ProducerConsumerMaster::queue_limit_ = queue_limit_;
}

void ProducerConsumerMaster::set_debug_queue_size(bool debug_queue_size) {
  debug_queue_size_ = debug_queue_size;
}

void ProducerConsumerMaster::set_debug_results(bool debug_results) {
  debug_results_ = debug_results;
}

void ProducerConsumerMaster::set_debug_detail(bool debug_detail) {
  debug_detail_ = debug_detail;
}

void ProducerConsumerMaster::set_debug_timing(bool debug_timing) {
  debug_timing_ = debug_timing;
}
