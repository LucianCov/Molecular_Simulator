#ifndef SPRING_H_
#define SPRING_H_

namespace GLOO {
struct Spring {
  int point_1_idx;
  int point_2_idx;
  float spring_constant;
  float rest_length;
};
}  // namespace GLOO

#endif