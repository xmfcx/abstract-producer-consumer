#ifndef PRODCONMASTER_BEARGENERATOR_H
#define PRODCONMASTER_BEARGENERATOR_H

#include "ProducerConsumerMaster.h"

class BearMaterial : public ProConMaterialMother {
public:
  typedef std::shared_ptr<BearMaterial> Ptr;
  long roar_magnitude;
};



class BearGenerator {
public:
  ProConMaterialMother::Ptr Generate();
private:
  int current_roar{0};


};


#endif //PRODCONMASTER_BEARGENERATOR_H
