#ifndef PRODCONMASTER_BEARTOPINGU_H
#define PRODCONMASTER_BEARTOPINGU_H

#include "ProducerConsumerMaster.h"
#include "BearGenerator.h"

class MaterialPingu : public ProConMaterialMother {
public:
  typedef std::shared_ptr<MaterialPingu> Ptr;
  long beak_size;

  void Log() override {
    std::cout << "beak_size: " << beak_size << std::endl;
  };
};

class BearToPingu {
public:
  typedef std::shared_ptr<BearToPingu> Ptr;
  ProducerConsumerMaster::FuncType GetMethod();

private:
  ProConMaterialMother::Ptr Process(ProConMaterialMother::Ptr bear_mat);

};


#endif //PRODCONMASTER_BEARTOPINGU_H
