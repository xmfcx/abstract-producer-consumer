#include "BearToPingu.h"

ProConMaterialMother::Ptr
BearToPingu::Process(ProConMaterialMother::Ptr bear_mat) {
  auto bear = std::dynamic_pointer_cast<BearMaterial>(bear_mat);
  if (!bear)
    throw std::runtime_error(
      "Cannot cast ProConMaterialMother to BearMaterial");


  MaterialPingu::Ptr cute_pingu = std::make_shared<MaterialPingu>();
  cute_pingu->beak_size = bear->roar_magnitude % 3;
  std::cout << "Converted bear to cute_pingu: " << cute_pingu->beak_size
            << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  return cute_pingu;
}

ProducerConsumerMaster::FuncType BearToPingu::GetMethod() {
  using std::placeholders::_1;
  return std::bind(&BearToPingu::Process, this, _1);
}
