#ifndef PRODCONMASTER_PINGUTOKOAL_H
#define PRODCONMASTER_PINGUTOKOAL_H

#include "ProducerConsumerMaster.h"
#include "BearToPingu.h"

class MaterialKoal : public ProConMaterialMother {
public:
  typedef std::shared_ptr<MaterialKoal> Ptr;
  long kaliptus_attraction;
};


class PinguToKoal {
public:
  ProducerConsumerMaster::FuncType GetMethod();

private:
  ProConMaterialMother::Ptr Process(ProConMaterialMother::Ptr mat_pingu);

};


#endif //PRODCONMASTER_PINGUTOKOAL_H
