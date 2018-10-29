#include "BearGenerator.h"

ProConMaterialMother::Ptr BearGenerator::Generate() {
  BearMaterial::Ptr bear = std::make_shared<BearMaterial>();
  bear->roar_magnitude = current_roar;
  std::cout << "Generated bear: " << current_roar << std::endl;
  current_roar++;
  return bear;
}
