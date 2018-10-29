#include <iostream>
#include <memory>
#include "ProducerConsumerMaster.h"
#include "BearGenerator.h"
#include "BearToPingu.h"
#include "PinguToKoal.h"

int main() {
  BearGenerator bear_generator;

  ProducerConsumerMaster::Ptr master = std::make_unique<ProducerConsumerMaster>();

  BearToPingu::Ptr bear_to_pingu = std::make_shared<BearToPingu>();
  auto func = bear_to_pingu->GetMethod();
  master->AddWorker(func, "BearToPingu", false);

  PinguToKoal pingu_to_koal;
  auto func2 = pingu_to_koal.GetMethod();
  master->AddWorker(func2, "PinguToKoal", true);

  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    master->PushTop(bear_generator.Generate());
  }


  std::this_thread::sleep_for(std::chrono::seconds(5));
  master->KillAll();
  master->WaitUntilAllDone();

  return 0;
}