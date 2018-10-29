#include "PinguToKoal.h"

ProConMaterialMother::Ptr
PinguToKoal::Process(ProConMaterialMother::Ptr mat_pingu) {
  auto pingu = std::dynamic_pointer_cast<MaterialPingu>(mat_pingu);
  if (!pingu)
    throw std::runtime_error(
      "Cannot cast ProConMaterialMother to MaterialPingu");


  MaterialKoal::Ptr cute_koal = std::make_shared<MaterialKoal>();
  cute_koal->kaliptus_attraction = pingu->beak_size * 10;
  std::cout << "Converted pingu to cute_koal: "
            << cute_koal->kaliptus_attraction
            << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return cute_koal;
}

ProducerConsumerMaster::FuncType PinguToKoal::GetMethod() {
  using std::placeholders::_1;
  return std::bind(&PinguToKoal::Process, this, _1);
}
